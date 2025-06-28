#ifndef MAZE_FUNCTIONS_H
#define MAZE_FUNCTIONS_H

#include <mcpp/mcpp.h>
#include "Maze.h"
#include "terrain.h"

enum States {
    ST_Main,
    ST_GetMaze,
    ST_BuildMaze,
    ST_SolveMaze,
    ST_Creators,
    ST_Exit
};

void checkEOF();
void handleMainMenu(States& curState);
void handleGetMaze(mcpp::MinecraftConnection& mc, Maze*& maze, mcpp::Coordinate& playerPos, int& xlen, int& zlen, bool& maze_generated,bool& withoutFlattening, States& curState, bool testMode);
void handleBuildMaze(mcpp::MinecraftConnection& mc, Maze*& maze, Maze*& previousMaze, bool& maze_built, int xlen, int zlen, bool&  withoutFlattening, States& curState);

void handleSolveMaze(mcpp::MinecraftConnection& mc, Maze* maze, int xlen, int zlen, bool& maze_built, bool&  withoutFlattening, States& curState, bool testMode);
void cleanupAndExit(Maze* maze, mcpp::MinecraftConnection& mc);
void safeInput(std::istream& in, std::string& input);

#endif
