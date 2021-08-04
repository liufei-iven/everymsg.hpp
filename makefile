CXX = g++
LIB = 
INC = 
CXXFLAGS = -std=c++11
 
TARGET=test_everymsg


CCSRC=$(wildcard ./*.c)
CPPSRC=$(wildcard ./*.cpp)

CCOBJ=$(patsubst %.c, %.o, $(CCSRC))
CPPOBJ=$(patsubst %.cpp, %.o, $(CPPSRC))


$(TARGET): $(CCOBJ) $(CPPOBJ)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LIB)
	@echo Building $@ completed ! 

$(CCOBJ):%.o: %.c
	$(CXX) $(CXXFLAGS) $(INC) -o $@ -c $<

$(CPPOBJ):%.o: %.cpp
	$(CXX) $(CXXFLAGS) $(INC) -o $@ -c $<
 
clean:
	rm -f *.o
	rm -f $(TARGET)
