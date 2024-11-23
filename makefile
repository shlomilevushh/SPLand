
all: clean makedir compile run

makedir:
	mkdir bin

compile:
	g++ -g -Wall -Weffc++ -std=c++11 -Iinclude src/Auxiliary.cpp src/main.cpp -o bin/simulation

run:
	./bin/simulation

clean:
	rm -f -r bin
