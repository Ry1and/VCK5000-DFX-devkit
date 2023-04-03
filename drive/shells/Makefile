all: $(CPP_OBJS)

CXX := g++ 
CXXFLAGS := -I../

CPP_SRC = $(wildcard *.cpp)

CPP_OBJS = $(CPP_SRC:.cpp=.o)

$(CPP_OBJS): %.o : %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f *.o $(all)