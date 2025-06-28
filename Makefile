.default: all

all: mazeRunner

clean:
	rm -f mazeRunner *.o 

mazeRunner: mazeRunner.o Maze.o terrain.o maze_functions.o menuUtils.o
	g++ -Wall -Werror -std=c++17 -g -O -o $@ $^ -lmcpp

%.o: %.cpp
	g++ -Wall -Werror -std=c++17 -g -O -c $<
