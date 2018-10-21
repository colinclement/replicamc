#include "Python.h"
#include <stdlib.h>
#define NPY_NO_DEPRECATED_API NPY_1_9_API_VERSION
#include "arrayobject.h"
#include <math.h>
#include <cuda.h>
#include <cublas_v2.h>
//#include <helper_cuda.h> //In samples/common/inc
//#include <thrust/device_vector.h>
//#include <thrust/functional.h>
//#include <thrust/transform_reduce.h>

#define CUDA_DEVICE 0

void init_cuda();
static PyObject *superreg_chebpoly(PyObject *self, PyObject *args);

const char docstring[] = ""
"Evaluate all terms of a chebyshev interpolant, but not summing the terms\n"
"when done. This is useful for evaluating fit Jacobians. This function only\n"
"applies to 2D images which only involve shifting the same set of coeff.\n"
"\n"
"Parameters\n"
"----------\n"
"coefficients : ndarray\n"
"   Cheb coefficients in the format [Cy, Cx]\n"
"\n"
"coordinates : ndarray\n"
"   Coordinates over which to evaluate in the shape [images, 2, ny, nx]\n"
"\n"
"devicearray : ndarray\n"
"   Array of pointers to device memory, created with `device_arrays`\n"
"\n"
"Returns\n"
"-------\n"
"polyterms : ndarray\n"
"   Each cheb term in the shape [Cy, Cx, images, ny, nx]\n";

// Method definition object for this extension, these argumens mean:
// ml_name: The name of the method
// ml_meth: Function pointer to the method implementation
// ml_flags: Flags indicating special features of this method, such as
//          accepting arguments, accepting keyword arguments, being a
//          class method, or being a static method of a class.
// ml_doc:  Contents of this method's docstring
static PyMethodDef superreg_methods[] = {
    {"chebpoly", superreg_chebpoly, METH_VARARGS, docstring},
    {NULL, NULL, 0, NULL}
};

static struct PyModuleDef superreg_definition = {
    PyModuleDef_HEAD_INIT,
    "superreg_chebpoly",
    "A Python module using CUDA for Super Registration",
    -1,
    superreg_methods
};

PyMODINIT_FUNC PyInit__superreg(void) {
    PyObject *module = PyModule_Create(&superreg_definition);
    import_array();
    init_cuda();
	return module;
}

#define COEFFICIENT_CACHE_SIZE 16

/*
bdimx, bdimy determined by CI*CJ, limited to 512 in each dimension (ideally
multiple of 32)
gdimx, gdimy determined by NX*NY*L / CI*CJ, limited to 2**16 in each dimesion
*/
__global__ void cu_chebpoly(float *coef, float *coordx, float *coordy,
        float *terms, int L, int CI, int CJ, int NX, int NY){
    /* calculate all the block indentifier stuff */
    int tidx = threadIdx.x, tidy = threadIdx.y;
    int bdimx = blockDim.x, bdimy = blockDim.y;
    int bidx = blockIdx.x, bidy = blockIdx.y;
    int gdimx = gridDim.x;

    //int threadId = (tidy*bdimx) + tidx;
    int blockId = bidx + gdimx*bidy;
    int gthreadId = blockId*(bdimx*bdimy) + (tidy*bdimx) + tidx;

    /* finally, we have the pixel ID "pix" */
    int pix = gthreadId;
    int npix = NX * NY * L;

    if (pix >= npix)
        return;

    float x = coordx[pix];
    float y = coordy[pix];

    int tindex = 0;
    float vx[COEFFICIENT_CACHE_SIZE];
    float vy[COEFFICIENT_CACHE_SIZE];

    int ix = 0; vx[0] = 1.0f; vx[1] = x;
    for (int i=0; i<CI; i++){
        vx[ix] = vx[ix]*(ix<2) + (2.0f*vx[ix-1]*x - vx[ix-2])*(ix>=2);

        int iy = 0; vy[0] = 1.0f; vy[1] = y;
        for (int j=0; j<CJ; j++){
            vy[iy] = vy[iy]*(iy<2) + (2.0f*vy[iy-1]*y - vy[iy-2])*(iy>=2);

            tindex = i + j*CI;
            terms[tindex*npix + pix] = coef[tindex] * vx[ix] * vy[iy];

            iy++;
            // shuffle the polynomial back to the array start
            if (iy >= COEFFICIENT_CACHE_SIZE){
                vy[0] = vy[COEFFICIENT_CACHE_SIZE-2];
                vy[1] = vy[COEFFICIENT_CACHE_SIZE-1];
                iy = 2;
            }
        }

        ix++;
        // shuffle the polynomial back to the array start
        if (ix >= COEFFICIENT_CACHE_SIZE){
            vx[0] = vx[COEFFICIENT_CACHE_SIZE-2];
            vx[1] = vx[COEFFICIENT_CACHE_SIZE-1];
            ix = 2;
        }
    }
}

void init_cuda(){
    int deviceCount = 0;
    cudaGetDeviceCount(&deviceCount);
    if(!deviceCount){
        fprintf(stderr, "Error: No CUDA supporting devices.\n");
        exit(EXIT_FAILURE);
    }
    cudaSetDevice(CUDA_DEVICE);
    struct cudaDeviceProp properties;
    cudaGetDeviceProperties(&properties, CUDA_DEVICE);

    // just to get the card warm...
    float *test;
    checkCudaErrors(cudaMalloc((void**)&test, sizeof(float)));
}

static PyObject *superreg_chebpoly(PyObject *self, PyObject *args){
    PyArrayObject *coef, *coordx, *coordy;

    /* Parse parameters and verify input */
    if (!PyArg_ParseTuple(args, "O!O!O!", &PyArray_Type, &coef,
                &PyArray_Type, &coordx, &PyArray_Type, &coordy)){
        //die("Failed to parse python argument\n");
        return NULL;
    }

    struct cudaDeviceProp properties;
    cudaGetDeviceProperties(&properties, CUDA_DEVICE);

    int CI = PyArray_DIM(coef, 0);
    int CJ = PyArray_DIM(coef, 1);
    int L = PyArray_DIM(coordx, 0);
    int NY = PyArray_DIM(coordx, 1);
    int NX = PyArray_DIM(coordx, 2);

    //printf("%i %i %i\n", L, NY, NX);
    int ncoef = CI * CJ;
    int npix = L * NX * NY;

    int Ncoef = PyArray_SIZE(coef);
    int Ncoord = PyArray_SIZE(coordx);
    uint64_t Nterms = ncoef * npix;
    int NBcoef = sizeof(float)*Ncoef;
    int NBcoord = sizeof(float)*Ncoord;
    uint64_t NBterms = sizeof(float)*Nterms;

    //printf("%i %i | %i %i | %lu\n", Ncoef, Ncoord, NBcoef, NBcoord, NBterms);

    float *h_coef, *h_coordx, *h_coordy;
    float *d_coef, *d_coordx, *d_coordy, *d_terms;
    h_coef = (float*)PyArray_DATA(coef);
    h_coordx = (float*)PyArray_DATA(coordx);
    h_coordy = (float*)PyArray_DATA(coordy);

    checkCudaErrors(cudaMalloc((void**)&d_coef, NBcoef));
    checkCudaErrors(cudaMalloc((void**)&d_coordx, NBcoord));
    checkCudaErrors(cudaMalloc((void**)&d_coordy, NBcoord));
    checkCudaErrors(cudaMalloc((void**)&d_terms, NBterms));
    checkCudaErrors(cudaMemcpy(d_coef, h_coef,  NBcoef, cudaMemcpyHostToDevice));
    checkCudaErrors(cudaMemcpy(d_coordx, h_coordx, NBcoord, cudaMemcpyHostToDevice));
    checkCudaErrors(cudaMemcpy(d_coordy, h_coordy, NBcoord, cudaMemcpyHostToDevice));
    checkCudaErrors(cudaGetLastError());

    uint32_t WARP = 32;
    uint32_t MAXTHREADS = properties.maxThreadsPerBlock; // per dimension of block
    uint32_t MAXBLOCKS = properties.maxGridSize[0]; // per dimension of grid

    uint32_t bdimx=0, bdimy=0;
    uint32_t gdimx=0, gdimy=0;

    if (Ncoef >= MAXTHREADS){
        bdimx = MAXTHREADS;
        bdimy = 1;
    } else {
        bdimx = WARP * ceil((float)Ncoef / WARP);
        bdimy = 1;
    }

    uint32_t nblocks = ceil((float)npix / (bdimx * bdimy));

    if (nblocks > MAXBLOCKS){
        gdimx = MAXBLOCKS;
        gdimy = ceil(nblocks / MAXBLOCKS);
    } else {
        gdimx = nblocks;
        gdimy = 1;
    }

	//printf("%i %i %i %i\n", bdimx, bdimy, gdimx, gdimy);

    // actually do things
    dim3 bdim(bdimx, bdimy, 1);
    dim3 gdim(gdimx, gdimy, 1);
    checkCudaErrors(cudaGetLastError());

    //printf("%i %i | %i %i\n", bdimx, bdimy, gdimx, gdimy);
    cu_chebpoly<<<gdim, bdim>>>(d_coef, d_coordx, d_coordy, d_terms, L, CI, CJ, NX, NY);
    checkCudaErrors(cudaGetLastError());

    // copy data back and let it go
	float *h_terms;
	h_terms = (float*)malloc(NBterms);
    checkCudaErrors(cudaDeviceSynchronize());
    checkCudaErrors(cudaMemcpy(h_terms, d_terms, NBterms, cudaMemcpyDeviceToHost));

    npy_intp dims[5] = {CJ, CI, L, NY, NX};
    PyObject *out = PyArray_SimpleNewFromData(5, dims, NPY_FLOAT32, h_terms);

    cudaFree(d_coef);
    cudaFree(d_coordx);
    cudaFree(d_coordy);
    cudaFree(d_terms);
    checkCudaErrors(cudaDeviceSynchronize());
    checkCudaErrors(cudaGetLastError());

    return out;
}








