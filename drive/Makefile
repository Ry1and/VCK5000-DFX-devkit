all: $(SUBDIRS) $(C_OBJS) $(CPP_OBJS) hash_test

CXX := g++ 
CC := gcc

CXXFLAGS : -Ishells -Ikernels -Itests -O2 -Wall -pthread

C_SRC = $(wildcard *.c)

CPP_SRC = $(wildcard *.cpp) 

C_OBJS = $(C_SRC:.c=.o) 

CPP_OBJS = $(CPP_SRC:.cpp=.o)


SUBDIRS := shells kernels tests

$(SUBDIRS):
	$(MAKE) -C $@



hash_test: test_main.o tests/reconf_test.o tests/dma_test.o dma_rw.o utils.o shells/vck5000.o kernels/hash_kernel.o kernels/app_kernel.o
	g++ $^ -o $@



$(C_OBJS): %.o : %.c
	$(CXX) -c $< -o $@
$(CPP_OBJS): %.o : %.cpp
	$(CXX) -c $< -o $@

clean:
	rm -f *.o $(all)
	rm -f kernels/*.o
	rm -f shells/*.o
	rm -f tests/*.o


	
.PHONY: $(SUBDIRS)