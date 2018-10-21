import time
import numpy as np
from numpy.polynomial.chebyshev import chebval
from _superreg import chebpoly

def npchebpoly(coef, coord):
    CJ, CI = coef.shape
    _, L, Ny, Nx = coord.shape
    eye = np.identity(CI)
    terms = np.zeros((CJ, CI, L, Ny, Nx))
    for im in range(L):
        for j in range(CJ):
            for i in range(CI):
                terms[j,i,im] = coef[j,i] * (
                                 chebval(coord[0, im], eye[j]) * 
                                 chebval(coord[1, im], eye[i])
                )
    return terms

rng = np.random.RandomState(92089)
np.random.seed(0)

L = 2
N = 128
C = 128

coef = np.ones((C, C))
coef = np.random.randn(C, C)

shifts = np.linspace(0, 0.01, L)
coordy, coordx = np.mgrid[-1:1:1j*N, -1:1:1j*N]
coordx = np.array([coordx + s for s in shifts])
coordy = np.array([coordy + s for s in shifts])
coord = np.array([coordy, coordx])

ts = time.time()
result = chebpoly(
    coef.astype('float32'),
    coordx.astype('float32'),
    coordy.astype('float32')
)
te = time.time()
print(te - ts)
#print(result.shape, result.size)
#print(result[0,1,0,:,:])
