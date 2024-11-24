
all: clean makedir compile run

makedir:
	mkdir bin

compile:
	g++ -g -Wall -Weffc++ -std=c++11 -Iinclude src/Auxiliary.cpp src/main.cpp src/Action.cpp src/Facility.cpp src/Plan.cpp src/SelectionPolicy.cpp src/Settlement.cpp src/Simulation.cpp -o bin/simulation

run:
	./bin/simulation

clean:
	rm -f -r bin
