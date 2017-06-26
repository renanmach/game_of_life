# MO644 Projeto Final 
# Renan Gomes Pereira 103927
# Conway's Game of Life
# Makefile
 
CC = gcc
CUDA = nvcc
CFLAGS = -Wall
MY_FLAGS = -DCOMPARE_SERIAL
OPENMP = -fopenmp
PTHREADS = -pthread -lpthread

all: conway_serial conway_omp conway_omp_tasks conway_pthreads conway_cuda conway_cuda_shared_memory
	
conway_serial: conway_functions.c conway_serial.c conway_main.c
	$(CC) $(CFLAGS) $(MY_FLAGS) conway_functions.c conway_serial.c conway_main.c -o serial

conway_omp: conway_functions.c conway_serial.c conway_main.c
	$(CC) $(CFLAGS) $(MY_FLAGS) $(OPENMP) conway_functions.c conway_omp.c conway_main.c -o omp

conway_omp_tasks: conway_functions.c conway_serial.c conway_main.c
	$(CC) $(CFLAGS) $(MY_FLAGS) $(OPENMP) conway_functions.c conway_omp_tasks.c conway_main.c -o omp_tasks

conway_pthreads: conway_functions.c conway_serial.c conway_main.c
	$(CC) $(CFLAGS) $(MY_FLAGS) $(PTHREADS) conway_functions.c conway_pthreads.c conway_main.c -o pthreads

conway_cuda: conway_functions.c conway_serial.c conway_main.c
	$(CUDA) $(MY_FLAGS) conway_functions.c conway_cuda.cu conway_main.c -o cuda

conway_cuda_shared_memory: conway_functions.c conway_serial.c conway_main.c
	$(CUDA) $(MY_FLAGS) conway_functions.c conway_cuda_shared_memory.cu conway_main.c -o cuda_shared_memory

clean:
	rm -f serial omp omp_tasks pthreads cuda cuda_shared_memory

run:
	./omp; ./omp_tasks; ./pthreads; ./cuda; ./cuda_shared_memory;
