#compiler for C++ : g++
CXX = g++

#version
CXXFLAGS = -std=c++11

# main code file
SRC = iscas.cpp

#executable file
EXECUTABLE = iscas

all: $(EXECUTABLE)

$(EXECUTABLE): $(SRC)
	$(CXX) $(CXXFLAGS) -o $(EXECUTABLE) $(SRC)

clean:
	rm -f $(EXECUTABLE)