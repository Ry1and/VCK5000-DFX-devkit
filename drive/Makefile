all: $(SUBDIRS) $(C_OBJS) $(CPP_OBJS) dma_test hash_test

CXX := g++ 
CC := gcc

CXXFLAGS : -Ishells -Ikernels -O2 -Wall -pthread

C_SRC = $(wildcard *.c)

CPP_SRC = $(wildcard *.cpp) 

C_OBJS = $(C_SRC:.c=.o) 

CPP_OBJS = $(CPP_SRC:.cpp=.o)


SUBDIRS := shells kernels

$(SUBDIRS):
	$(MAKE) -C $@

dma_test: dma_test.o dma_rw.o
	g++ $^ -o $@

hash_test: hash_test.o dma_rw.o utils.o shells/vck5000.o kernels/hash_kernel.o kernels/app_kernel.o
	g++ $^ -o $@



$(C_OBJS): %.o : %.c
	$(CXX) -c $< -o $@
$(CPP_OBJS): %.o : %.cpp
	$(CXX) -c $< -o $@

clean:
	rm -f *.o $(all)


	
.PHONY: $(SUBDIRS)