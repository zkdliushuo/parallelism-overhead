CXX=mpicxx
CXXFLAGS=-Iinclude/ -O3 -g -mavx -mfma -fopenmp

SRCS=main.cc

main: $(SRCS)
	$(CXX) $(CXXFLAGS) -o main -no-pie $(SRCS) $(LDFLAGS)

all: main

clean:
	rm -f $(OBJECTS)
