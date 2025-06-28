#ifndef ASSIGN_MAZE_H
#define ASSIGN_MAZE_H

#include <mcpp/mcpp.h>
#include <string>
#include "terrain.h"
#include <random>
#include "Agent.h"
#include <list>
#include <utility>
#include <vector>

const int DX[4] = {1, 0, -1, 0};  // UP, RIGTH, DOWN, LEFT
const int DY[4] = {0, 1, 0, -1};


class Coord {


public:
    int r;
    int c;
    Coord(int r, int c) : r(r), c(c) {}
    friend Coord operator+(const Coord& lhs, const Coord& rhs) {
        return Coord(lhs.r + rhs.r, lhs.c + rhs.c);
    }
};

class Maze {
    class Point {
        private:
            int x;
            int y;

        public:
            Point(int xx, int yy);
            int getX() const;
            int getY() const;
    };

public:
    Maze(mcpp::Coordinate basePoint, unsigned int xlen, unsigned int zlen);
    ~Maze();

    void createFromInput(mcpp::MinecraftConnection& mc);
    void buildMazeInMinecraft(mcpp::MinecraftConnection& mc, unsigned int xlen, unsigned int zlen);
    void revertMazeChanges(mcpp::MinecraftConnection& mc);
    void flattenTerrainInMinecraft(mcpp::MinecraftConnection& mc, int startX, int startY, int startZ, int width, int length, int baseHeight);
    void generateMaze(bool testMode);
    void printMaze(mcpp::MinecraftConnection& mc);
    void solveManually(mcpp::MinecraftConnection& mc, unsigned int xlen, unsigned int zlen , bool testMode );
    void escapeRoute(mcpp::MinecraftConnection& mc, bool testMode);
    bool isValidMove(mcpp::MinecraftConnection& mc, int x, int y, int z);
   // bool isValidMoveRHR(mcpp::MinecraftConnection& mc, int x, int y, int z);
    void solveManuallyforE1(mcpp::MinecraftConnection& mc, unsigned int xlen, unsigned int zlen, bool testMode) ;
    void printMaze2(mcpp::MinecraftConnection& mc);
    void solveWithBFS(mcpp::MinecraftConnection& mc);
    bool isWall(mcpp::MinecraftConnection& mc, const mcpp::Coordinate& coord);
    void checkLoopMaze();
    void cleanupAndExit(Maze* maze, mcpp::MinecraftConnection& mc);
    void checkMaze();    
    void floodFill(char**& env, int& height, int& width, Coord& curr);
    void floodLoopFill(char**& env, int& height, int& width, Coord& curr);
    void withoutFlattening( mcpp::MinecraftConnection& mc, unsigned int  x,  unsigned int z, Maze* maze);
    void buildMaze2ForE1(mcpp::MinecraftConnection& mc, unsigned int xlen, unsigned int zlen) ;
    void setMazeStruct(char** newStruct);
    char** getMazeStruct() const;
    void checkEOF();
    bool checkIso();
    bool checkLoop();
    
private:
   
    mcpp::Coordinate basePoint;
    unsigned int xlen, zlen;
    // bool withFlattening = true; 

    //   unsigned int length;
    // unsigned int width;
    char** mmaze; // Dynamic 2D array for the maze
    std::vector<std::pair<mcpp::Coordinate, mcpp::BlockType>> modifiedMazeBlocks;
    bool heightCheck(mcpp::MinecraftConnection& mc, int x, int y, int z) ;
    std::default_random_engine rng;
    ChangeList modifiedTerrainBlocks;
};

#endif
