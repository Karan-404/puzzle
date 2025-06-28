#include <iostream>
#include <mcpp/mcpp.h>
#include "menuUtils.h"
#include "Maze.h"
#include "terrain.h"
#include "maze_functions.h"

int main(int argc, char* argv[]) {
    bool testMode = false;
     if (argc > 1 && std::string(argv[1]) == "-testmode") {
        testMode = true;
        std::cout << "Test mode activated." << std::endl;
    }
    mcpp::MinecraftConnection mc;
    mc.doCommand("time set day");

    bool maze_generated = false;
    bool maze_built = false;
    bool withoutFlattening = false;
    States curState = ST_Main;
    Maze* maze = nullptr; 
    Maze* previousMaze = nullptr;
    mcpp::Coordinate playerPos;
    int xlen, zlen;
    printStartText();
    // std::cout << "Welcome to Minecraft MazeRunner!" << std::endl;
    // std::cout << "--------------------------------" << std::endl;

    while (true) {
        if (curState == ST_Main) {
            handleMainMenu(curState);
        } else if (curState == ST_GetMaze) {
            handleGetMaze(mc, maze, playerPos, xlen, zlen, maze_generated, withoutFlattening, curState, testMode);
        } else if (curState == ST_BuildMaze) {
            handleBuildMaze(mc, maze, previousMaze, maze_built, xlen, zlen, withoutFlattening, curState);
        } else if (curState == ST_SolveMaze) {
            handleSolveMaze(mc, maze, xlen, zlen, maze_built, withoutFlattening, curState, testMode);
        } else if (curState == ST_Creators) {
            printTeamInfo();
            curState = ST_Main;
        } else if (curState == ST_Exit) {
            cleanupAndExit(maze, mc);
        } else {
            std::cout << "Input Error: Enter a number between 1 and 5 ..." << std::endl;
            curState = ST_Main;
        }
    }

    return EXIT_SUCCESS;
}
