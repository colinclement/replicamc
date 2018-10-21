
# These are locations that will be unique to your computer
#PYTHON_DIR = $(HOME)/anaconda
CUDA_INC = -I$(CUDA_HOME)/inc
CUDA_LIB = -L$(CUDA_HOME)/lib64 -lcudart -lcublas

#Numpy headers
INC = -I$(PYTHON_DIR)/include/python3.6m/
INC += -I$(PYTHON_DIR)/lib/python3.6/site-packages/numpy/core/include/numpy/

#CUDA helper function for debugging
INC += -I$(CUDA_HOME)/samples/common/inc

CFLAGS = -O3 -D_FORCE_INLINES #-g 
CUDA_FLAGS = #--use_fast_math

all: _superreg.so
	
_superreg.so: superreg.o
	g++ $(CFLAGS) -fPIC -shared -o _superreg.so superreg.o $(CUDA_LIB)

superreg.o: superreg.cu
	nvcc $(CFLAGS) $(CUDA_FLAGS) -c superreg.cu -Xcompiler -fPIC $(INC) $(CUDA_INC)

clean:
	rm -f *.o *.so
