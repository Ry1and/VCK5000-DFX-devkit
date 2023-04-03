all: $(CPP_OBJS)



CXX := g++ 
CXXFLAGS := -I../shells -I../ -pthread

CPP_SRC: $(wildcard *.cpp)

CPP_OBJS = $(CPP_SRC:.cpp=.o)

$(CPP_OBJS): %.o : %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

