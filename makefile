all: create

create: client.cpp data.o
	c++ -std=c++17 -o FundsCalculator client.cpp data.o

data.o: data.cpp
	c++ -std=c++17 -c data.cpp

clean:
	rm -rf *.o
