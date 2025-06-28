#include "Maze.h"
#include <iostream>
#include <thread>
#include <queue>
#include <unordered_map>
#include <algorithm>
#include <cstring> // For std::memset

using namespace std::chrono_literals;

Maze::Maze(mcpp::Coordinate basePoint, unsigned int xlen, unsigned int zlen)
    : basePoint(basePoint), xlen(xlen), zlen(zlen) {
    rng.seed(static_cast<unsigned int>(std::time(nullptr)));

    // Allocate memory for the 2D array
    mmaze = new char*[zlen];
    for (unsigned int i = 0; i < zlen; ++i) {
        mmaze[i] = new char[xlen];
        std::memset(mmaze[i], ' ', xlen * sizeof(char)); // Initialize with spaces
    }
}

Maze::~Maze() {
    // Free the allocated memory
    for (unsigned int i = 0; i < zlen; ++i) {
        delete[] mmaze[i];
    }
    delete[] mmaze;
}

Maze::Point::Point(int xx, int yy) : x(xx), y(yy) {}
int Maze::Point::getX() const { return x; }
int Maze::Point::getY() const { return y; }

void Maze::generateMaze(bool testMode) {
    unsigned int exitY = 0;
    unsigned int exitX = 0;
    if(!testMode){
        // Randomly choose which side to place the exit on
        unsigned int side = rand() % 4; // 0: top, 1: bottom, 2: left, 3: right
        if(xlen == 3 && zlen == 3){
            if(side == 0){
                exitX = 1;
                exitY = 0;
            }
            else if(side == 1){
                exitX = 1;
                exitY = zlen-1;
            }
            else if(side == 2){
                exitX = 0;
                exitY = 1;
            }
            else if(side == 3){
                exitX = xlen-1;
                exitY = 1;
            }
        }
        else if(zlen == 3){
            if (side == 0) {  // Top edge
                exitY = 0;
                exitX = 1 + 2 * (rand() % ((xlen - 2) / 2));  // Odd x
            }
            else if (side == 1) {  // Bottom edge
                exitY = zlen - 1;
                exitX = 1 + 2 * (rand() % ((xlen - 2) / 2));  // Odd x
            } 
            else if(side == 2){
                exitX = 0;
                exitY = 1;
            }
            else if(side == 3){
                exitX = xlen-1;
                exitY = 1;
            }
        }
        else {
            if (side == 0) {  // Top edge
                exitY = 0;
                exitX = 1 + 2 * (rand() % ((xlen - 2) / 2));  // Odd x
            } else if (side == 1) {  // Bottom edge
                exitY = zlen - 1;
                exitX = 1 + 2 * (rand() % ((xlen - 2) / 2));  // Odd x
            } else if (side == 2) {  // Left edge
                exitX = 0;
                exitY = 1 + 2 * (rand() % ((zlen - 2) / 2));  // Odd y
            } else {  // Right edge
                exitX = xlen - 1;
                exitY = 1 + 2 * (rand() % ((zlen - 2) / 2));  // Odd y
            }
        }

        // Set up the walls
        for (unsigned int i = 0; i < xlen; ++i) {
            mmaze[0][i] = 'x';
            mmaze[zlen - 1][i] = 'x';
        }
        for (unsigned int i = 1; i < zlen - 1; ++i) {
            mmaze[i][0] = 'x';
            mmaze[i][xlen - 1] = 'x';
        }

        // Initialize the interior with walls
        for (unsigned int i = 1; i < zlen - 1; ++i) {
            for (unsigned int j = 1; j < xlen - 1; ++j) {
                mmaze[i][j] = 'x'; // All walls initially
            }
            std::cout << std::endl;
        }
        // Mark the exit as '.' permanently
        mmaze[exitY][exitX] = '.';

        // Get start point inside the maze, ensuring it's adjacent to the exit
        int startY, startX;
        if (exitY == 0) {
            startY = 1; // Inside the maze, below the top wall
            startX = exitX; // Directly below the exit
        } else if (exitY == zlen - 1) {
            startY = zlen - 2; // Inside the maze, above the bottom wall
            startX = exitX; // Directly above the exit
        } else if (exitX == 0) {
            startX = 1; // Inside the maze, to the right of the left wall
            startY = exitY; // Directly right of the exit
        } else {
            startX = xlen - 2; // Inside the maze, to the left of the right wall
            startY = exitY; // Directly left of the exit
        }

        // Ensure the start point is marked as visited
        mmaze[startY][startX] = '.'; // Mark start point
        std::vector<Point> path;
        path.push_back(Point(startX, startY));

        std::vector<std::vector<bool>> visited(zlen, std::vector<bool>(xlen, false));
        visited[startY][startX] = true;

        while (!path.empty()) {
            Point current = path.back();
            unsigned int cx = current.getX();
            unsigned int cy = current.getY();

            std::vector<int> availableDirections;
            for (int i = 0; i < 4; ++i) {
                unsigned int nx = cx + 2 * DX[i];
                unsigned int ny = cy + 2 * DY[i];
                if (nx > 0 && nx < xlen - 1 && ny > 0 && ny < zlen - 1 && mmaze[ny][nx] == 'x' && !visited[ny][nx]) {
                    availableDirections.push_back(i);
                }
            }

            if (!availableDirections.empty()) {
                int randomDirIndex = rand() % availableDirections.size();
                int direction = availableDirections[randomDirIndex];

                int nx = cx + 2 * DX[direction];
                int ny = cy + 2 * DY[direction];

                mmaze[cy + DY[direction]][cx + DX[direction]] = '.';
                mmaze[ny][nx] = '.';
                path.push_back(Point(nx, ny));
                visited[ny][nx] = true;
            } else {
                path.pop_back();
            }
        }
    }
    else{ // TESTING MODE CODE
        exitY = 1;
        exitX = 1;

        // Set up the walls
        for (unsigned int i = 0; i < xlen; ++i) {
            mmaze[0][i] = 'x';
            mmaze[zlen - 1][i] = 'x';
        }
        for (unsigned int i = 1; i < zlen - 1; ++i) {
            mmaze[i][0] = 'x';
            mmaze[i][xlen - 1] = 'x';
        }

        // Initialize the interior with walls
        for (unsigned int i = 1; i < zlen - 1; ++i) {
            for (unsigned int j = 1; j < xlen - 1; ++j) {
                mmaze[i][j] = 'x'; // All walls initially
            }
        }

        mmaze[1][0] = '.';
        mmaze[1][1] = '.';

        std::vector<Point> path;
        path.push_back(Point(exitX, exitY));


        std::vector<std::vector<bool>> visited(zlen, std::vector<bool>(xlen, false));
        visited[1][1] = true;

        while (!path.empty()) {
        Point current = path.back();
        unsigned int cx = current.getX();
        unsigned int cy = current.getY();

        bool moved = false;

        // Try each direction in UP → RIGHT → DOWN → LEFT order without randomness
        for (int i = 0; i < 4; ++i) {
            unsigned int nx = cx + 2 * DX[i];
            unsigned int ny = cy + 2 * DY[i];

            // Check if the new position is within bounds and unvisited
            if (nx > 0 && nx < xlen - 1 && ny > 0 && ny < zlen - 1 && mmaze[ny][nx] == 'x' && !visited[ny][nx]) {
                // Carve the path
                mmaze[cy + DY[i]][cx + DX[i]] = '.';  // Mark the wall between current and next cell
                mmaze[ny][nx] = '.';                   // Mark the next cell as part of the path
                path.push_back(Point(nx, ny));         // Add the new position to the path
                visited[ny][nx] = true;                // Mark the new cell as visited
                moved = true;                          // Mark that we moved in a valid direction
                // No break here, continue checking other directions
            }
        }

        // If no valid move was found, backtrack
        if (!moved) {
            path.pop_back();
        }
    }

    }
}

void Maze::createFromInput(mcpp::MinecraftConnection& mc) {
    std::cout << "Enter the maze structure:\n";

    // Free existing memory if the function is called more than once
    if (mmaze != nullptr) {
        for (unsigned int i = 0; i < zlen; ++i) {
            delete[] mmaze[i];
        }
        delete[] mmaze;
    }

    // Allocate memory for mmaze with zlen rows and xlen columns
    mmaze = new char*[zlen];  // Allocate rows
    for (unsigned int i = 0; i < zlen; ++i) {
        mmaze[i] = new char[xlen];  // Allocate columns for each row
    }

    // Read the input and populate mmaze
    for (unsigned int i = 0; i < zlen; ++i) {
        std::string row;
        std::cin >> row;
        checkEOF();
        // Check if the row length matches xlen
        if (row.length() != xlen) {
            std::cout << "Error: Row length must be " << xlen << " characters. Please enter again:\n";
            --i; // Repeat this row input            
        } else {
            // Validate and populate the row
            bool valid = true;
            for (char ch : row) {
                if (ch != 'x' && ch != '.') {
                    valid = false;                
                }
            }

            if (!valid) {
                std::cout << "Error: Row can only contain 'x' for walls and '.' for paths. Please enter again:\n";
                --i; // Repeat this row input
            } else {
                for (unsigned int j = 0; j < xlen; ++j) {
                    mmaze[i][j] = row[j]; // Correctly write to mmaze[i][j]
                }
            }
        }
    }

    std::cout << "Maze read successfully\n";

    // Lav add you part here dont change anything else
    printMaze(mc);
}



void Maze::floodFill(char**& env, int& height, int& width, Coord& curr){
    char startChar = env[curr.r][curr.c];
    char fillChar = 'x';
    bool exit = false;

    if (curr.r < 0 || curr.r >= height || curr.c < 0 || curr.c >= width) {
        exit = true;
    }

    if (!exit && (env[curr.r][curr.c] != startChar || env[curr.r][curr.c] == fillChar)) {
        exit = true;
    }

    if(exit){
        return;
    }
    env[curr.r][curr.c] = fillChar;

    Coord directions[] = {Coord(-1, 0), Coord(1, 0), Coord(0, -1), Coord(0, 1)};

    for (const Coord& dir : directions) {
        Coord next = curr + dir;
        floodFill(env, height, width, next);
    }
}

bool Maze::checkIso(){
    char** temp;
    temp = new char*[zlen]; // copy the real maze into the a temp arr for checking
    for (unsigned int i = 0; i < zlen; i++) {
        temp[i] = new char[xlen];
        for (unsigned int j = 0; j < xlen; j++) {
            temp[i][j] = mmaze[i][j];
        }
    }

    int xexit = -1;
    int zexit = -1;
    int exit_found = false;
    //check top wall for exit
    for(unsigned int i = 0; i < xlen; ++i){
        if(temp[0][i] == '.'){
            exit_found = true;
            xexit = i;
            zexit = 0;
        }
    }
    //check bottom wall for exit
    if(!exit_found){
        for(unsigned int i = 0; i < xlen; ++i){
            if(temp[zlen-1][i] == '.'){
                exit_found = true;
                xexit = i;
                zexit = zlen-1;
            }
        }
    }
    //check left wall for exit
    if(!exit_found){
        for(unsigned int i = 0; i < zlen; ++i){
            if(temp[i][0] == '.'){
                exit_found = true;
                xexit = 0;
                zexit = i;
            }
        }
    }
    //check rigth wall for exit
    if(!exit_found){
        for(unsigned int i = 0; i < zlen; ++i){
            if(temp[i][xlen-1] == '.'){
                exit_found = true;
                xexit = xlen-1;
                zexit = i;
            }
        }
    }
    
    Coord start(zexit, xexit);
    int xxlen = static_cast<int>(xlen);
    int zzlen = static_cast<int>(zlen);
    floodFill(temp, zzlen, xxlen, start);;
    bool loop = false;
    for (unsigned int i = 0; i < zlen; i++) {
        for (unsigned int j = 0; j < xlen; j++) {
            if(temp[i][j] != 'x'){
                loop = true;
            }
        }
    }
    return loop;
}

void Maze::checkMaze(){
    char** temp;
    temp = new char*[zlen]; // copy the real maze into the a temp arr for checking
    for (unsigned int i = 0; i < zlen; i++) {
        temp[i] = new char[xlen];
        for (unsigned int j = 0; j < xlen; j++) {
            temp[i][j] = mmaze[i][j];
        }
    }

    

    int xexit = -1;
    int zexit = -1;
    int exit_found = false;
    //check top wall for exit
    for(unsigned int i = 0; i < xlen; ++i){
        if(temp[0][i] == '.'){
            exit_found = true;
            xexit = i;
            zexit = 0;
        }
    }
    //check bottom wall for exit
    if(!exit_found){
        for(unsigned int i = 0; i < xlen; ++i){
            if(temp[zlen-1][i] == '.'){
                exit_found = true;
                xexit = i;
                zexit = zlen-1;
            }
        }
    }
    //check left wall for exit
    if(!exit_found){
        for(unsigned int i = 0; i < zlen; ++i){
            if(temp[i][0] == '.'){
                exit_found = true;
                xexit = 0;
                zexit = i;
            }
        }
    }
    //check rigth wall for exit
    if(!exit_found){
        for(unsigned int i = 0; i < zlen; ++i){
            if(temp[i][xlen-1] == '.'){
                exit_found = true;
                xexit = xlen-1;
                zexit = i;
            }
        }
    }
    
    Coord start(zexit, xexit);
    int xxlen = static_cast<int>(xlen);
    int zzlen = static_cast<int>(zlen);
    floodFill(temp, zzlen, xxlen, start);;
    bool loop = false;
    int lpx = 0;
    int lpz = 0;
    std::vector<Point> spots;
    for (unsigned int i = 0; i < zlen; i++) {
        for (unsigned int j = 0; j < xlen; j++) {
            if(temp[i][j] != 'x'){
                loop = true;
                lpx = j;
                lpz = i;
                spots.push_back(Point(lpz, lpx));
            }
        }
    }
    if(!loop){
        std::cout << "There is no isolated areas in this maze\n";
    }
    else{
        // std::cout << "Maze structure is getting fixed...\n";
        unsigned int nx, ny;
        int direction = rand() % 4;
        int point = rand() % spots.size();
        Point current = spots.at(point);
        nx = current.getX() + DX[direction];
        ny = current.getY() + DY[direction];
        bool valid = false;
        while(!valid){  //pick a direction in which a block will be removed to end a loop
            if(nx < xlen - 1 && ny < zlen - 1 && nx > 0 && ny > 0){
                valid = true;
            }
            else{
                int direction = rand() % 4;
                nx = lpx + DX[direction];
                ny = lpz + DY[direction];
            }
        }
        mmaze[ny][nx] = '.';
        bool loop = checkLoop();
        if(loop){
            mmaze[ny][nx] = 'x';
        }
        checkMaze(); //make a recursive function until there is no loop;
    }

    // Clean up memory
    for (unsigned int i = 0; i < zlen; i++) {
        delete[] temp[i];
    }
    delete[] temp;

}

void Maze::floodLoopFill(char**& env, int& height, int& width, Coord& curr){
    char startChar = env[curr.r][curr.c];
    char fillChar = '.';
    bool exit = false;

    if (curr.r < 0 || curr.r >= height || curr.c < 0 || curr.c >= width) {
        exit = true;
    }

    if (!exit && (env[curr.r][curr.c] != startChar || env[curr.r][curr.c] == fillChar)) {
        exit = true;
    }

    if(exit){
        return;
    }
    env[curr.r][curr.c] = fillChar;

    Coord directions[] = {Coord(-1, 0), Coord(1, 0), Coord(0, -1), Coord(0, 1)};

    for (const Coord& dir : directions) {
        Coord next = curr + dir;
        floodLoopFill(env, height, width, next);
    }
}

bool Maze::checkLoop(){
    char** tempp;
    tempp = new char*[zlen]; // copy the real maze into the a temp arr for checking
    for (unsigned int i = 0; i < zlen; i++) {
        tempp[i] = new char[xlen];
        for (unsigned int j = 0; j < xlen; j++) {
            tempp[i][j] = mmaze[i][j];
        }
    }

    int xxlen = static_cast<int>(xlen);
    int zzlen = static_cast<int>(zlen);
    bool loop = false;	
    for(int i = 0; i < xxlen; ++i){
        Coord start(0, i);
        floodLoopFill(tempp, zzlen, xxlen, start);
    }
    for (unsigned int i = 0; i < zlen; i++) {
        for (unsigned int j = 0; j < xlen; j++) {
            if(tempp[i][j] != '.'){
                loop = true;
            }
        }
    }
    return loop;
}
	
void Maze::checkLoopMaze(){
    char** tempp;
    tempp = new char*[zlen]; // copy the real maze into the a temp arr for checking
    for (unsigned int i = 0; i < zlen; i++) {
        tempp[i] = new char[xlen];
        for (unsigned int j = 0; j < xlen; j++) {
            tempp[i][j] = mmaze[i][j];
        }
    }

    int xxlen = static_cast<int>(xlen);
    int zzlen = static_cast<int>(zlen);
    bool loop = false;
    int lpx = 0;
    int lpz = 0;	
    for(int i = 0; i < xxlen; ++i){
        Coord start(0, i);
        floodLoopFill(tempp, zzlen, xxlen, start);
    }
    for (unsigned int i = 0; i < zlen; i++) {
        for (unsigned int j = 0; j < xlen; j++) {
            if(tempp[i][j] != '.'){
                loop = true;
                lpx = j;
                lpz = i;
            }
        }
    }
    if(!loop){
        std::cout << "There is no loops in this maze\n";
    }
    else{
        // std::cout << "Maze structure is getting fixed...\n";
        unsigned int nx, ny;
        int direction = rand() % 4;
        nx = lpx + DX[direction];
        ny = lpz + DY[direction];
        bool valid = false;
        while(!valid){  //pick a direction in which a block will be removed to end a loop
            if(nx < xlen - 1 && ny < zlen - 1 && nx > 0 && ny > 0){
                valid = true;
            }
            else{
                int direction = rand() % 4;
                nx = lpx + DX[direction];
                ny = lpz + DY[direction];
            }
        }
        mmaze[ny][nx] = 'x';
        checkLoopMaze(); //make a recursive function until there is no loop;
    }

    // Clean up memory
    for (unsigned int i = 0; i < zlen; i++) {
        delete[] tempp[i];
    }
    delete[] tempp;

}

void Maze::flattenTerrainInMinecraft(mcpp::MinecraftConnection& mc, int startX, int startY, int startZ, int width, int length, int terrainHeight) {
    mcpp::BlockType emptyBlock = mcpp::Blocks::AIR;

    unsigned int extendedWidth = width + 2;   
    unsigned int extendedLength = length + 2;

    for (unsigned int x = startX; x < startX + extendedWidth; ++x) {
        for (unsigned int z = startZ; z < startZ + extendedLength; ++z) {
            int columnHeight = mc.getHeight(x, z);
            mcpp::BlockType highestBlockType = mc.getBlock(mcpp::Coordinate(x, columnHeight, z));

            for (int y = columnHeight; y > terrainHeight; --y) {
                mcpp::Coordinate blockLoc(x, y, z);
                mcpp::BlockType originalBlock = mc.getBlock(blockLoc);
                modifiedTerrainBlocks.addChange(blockLoc, originalBlock);
                mc.setBlock(blockLoc, emptyBlock);
            }

            for (int y = terrainHeight; y > columnHeight; --y) {
                if (y >= 0) {
                    mcpp::Coordinate blockLoc(x, y, z);
                    mcpp::BlockType originalBlock = mc.getBlock(blockLoc);
                    modifiedTerrainBlocks.addChange(blockLoc, originalBlock);
                    mc.setBlock(blockLoc, highestBlockType);
                }
            }
        }
    }
}


void Maze::printMaze2(mcpp::MinecraftConnection& mc) {
    try {
        
       
        
        for (unsigned int i = 0; i < zlen; ++i) {
            for (unsigned int j = 0; j < xlen; ++j) {
                std::cout << mmaze[i][j] << ' ';
            }
            std::cout << std::endl;  // Newline after each row for better readability
        }
    } catch (const std::exception& e) {
        std::cerr << "Exception occurred while printing the maze: " << e.what() << std::endl;
    } catch (...) {
        std::cerr << "Unknown error occurred while printing the maze." << std::endl;
    }
}




void Maze::printMaze(mcpp::MinecraftConnection& mc) {
        std::cout << "**Printing Maze**\n";
        mcpp::Coordinate playerLoc = mc.getPlayerPosition();
        int heightB = mc.getHeight(playerLoc.x, playerLoc.z);
        std::cout << "BasePoint: (" << playerLoc.x << ", " << heightB+1 << ", " << playerLoc.z << ")\n";
        std::cout << "Structure:" << std::endl;
        for(unsigned int i = 0; i < zlen; ++i){
            for(unsigned int j = 0; j < xlen ; ++j){
                std::cout << mmaze[i][j];
            }
            std::cout << std::endl;
        }
        std::cout << "**End Printing Maze**\n";
}



void Maze::buildMazeInMinecraft(mcpp::MinecraftConnection& mc, unsigned int xlen, unsigned int zlen) {
    std::cout << "Building the maze in Minecraft...\n" << std::flush;
    int terrainHeight = mc.getHeight(basePoint.x, basePoint.z);

    // Flatten terrain starting at the base point
    flattenTerrainInMinecraft(mc, basePoint.x, basePoint.y, basePoint.z, xlen, zlen, terrainHeight);

    mcpp::BlockType wallBlock = mcpp::Blocks::ACACIA_WOOD_PLANK;
    mcpp::BlockType exitBlock = mcpp::Blocks::BLUE_CARPET;

    // Adjust maze start location to (basePoint.x + 1, basePoint.y, basePoint.z + 1)
    for (unsigned int z = 0; z < zlen; ++z) {
        for (unsigned int x = 0; x < xlen; ++x) {
            if (mmaze[z][x] == 'x') {
                for (int y = 0; y < 3; ++y) {
                    mcpp::Coordinate loc(basePoint.x + x + 1, terrainHeight + y + 1, basePoint.z + z + 1);
                    modifiedMazeBlocks.push_back({loc, mc.getBlock(loc)});
                    mc.setBlock(loc, wallBlock);
                    std::this_thread::sleep_for(50ms);
                }
            } else if (mmaze[z][x] == '.') {
                if (z == zlen - 1 || z == 0 || x == xlen - 1 || x == 0) {
                    mcpp::Coordinate loc(basePoint.x + x + 1, terrainHeight + 1, basePoint.z + z + 1);
                    modifiedMazeBlocks.push_back({loc, mc.getBlock(loc)});
                    mc.setBlock(loc, exitBlock);
                }
            }
        }
    }
    std::cout << "The maze successfully built in Minecraft!\n";
}


void Maze::revertMazeChanges(mcpp::MinecraftConnection& mc) {
    for (const auto& entry : modifiedMazeBlocks) {
        mc.setBlock(entry.first, entry.second);
    }
    modifiedTerrainBlocks.revertChanges(mc);
}


void Maze::checkEOF() {
    if (std::cin.eof()) {
        std::cout << "\nEOF received. Program will now safely exit." << std::endl;
        std::exit(EXIT_SUCCESS);
    }
}


void Maze::solveManually(mcpp::MinecraftConnection& mc, unsigned int xlen, unsigned int zlen, bool testMode) {
    mcpp::Coordinate playerPosition = basePoint;
    int height = mc.getHeight(playerPosition.x + 1, playerPosition.z + 1);  // Adjusted for new maze start position
    playerPosition.y = height + 1;

    unsigned int withoffset = xlen;
    unsigned int heightoffset = zlen;

    if (testMode) {
        // Place the player in the empty cell farthest from the base point (bottom-right edge).
        for (unsigned int y = 0; y < withoffset; ++y) {
            for (unsigned int x = 0; x < heightoffset; ++x) {
                if (mmaze[y][x] == '.') {  // If cell is empty
                 //   playerPosition = mcpp::Coordinate(basePoint.x + x + 1, height + 1, basePoint.z + y + 1);  // Adjusted coordinates
                    playerPosition = mcpp::Coordinate(basePoint.x + x + 1, basePoint.y, basePoint.z + y + 1);  // Adjusted coordinates
                }
            }
        }
    } else {
        // Place the player in a random empty cell in non-testing mode
        std::vector<mcpp::Coordinate> emptyCells;
        for (unsigned int y = 0; y < withoffset; ++y) {
            for (unsigned int x = 0; x < heightoffset; ++x) {
                if (mmaze[y][x] == '.') {  // Collect all empty cells
                    int cellHeight = mc.getHeight(basePoint.x + x + 1, basePoint.z + y + 1);
                    emptyCells.push_back(mcpp::Coordinate(basePoint.x + x + 1, cellHeight + 1, basePoint.z + y + 1));  // Adjusted coordinates
                }
            }
        }

        if (!emptyCells.empty()) {
            playerPosition = emptyCells[rand() % emptyCells.size()];  // Pick a random empty cell
        } else {
            std::cout << "Could not find space in the maze" << std::endl;
        }
    }

    mc.setPlayerPosition(playerPosition);
    std::cout << "Player moved inside the Maze" << std::endl;
// std::cout << "Player Position: (" << playerPosition.x << ", " << playerPosition.y << ", " << playerPosition.z << ")" << std::endl;
}


void Maze::solveManuallyforE1(mcpp::MinecraftConnection& mc, unsigned int xlen, unsigned int zlen, bool testMode) {
    mcpp::Coordinate playerPosition = basePoint;
    int height = mc.getHeight(playerPosition.x + 1, playerPosition.z + 1);  // Adjusted for new maze start position
    playerPosition.y = height + 1;

    unsigned int withoffset = xlen;
    unsigned int heightoffset = zlen;

    if (testMode) {
        // Place the player in the empty cell farthest from the base point (bottom-right edge).
        for (unsigned int y = 0; y < withoffset; ++y) {
            for (unsigned int x = 0; x < heightoffset; ++x) {
                if (mmaze[y][x] == '.') {  // If cell is empty
                    int targetHeight = mc.getHeight(basePoint.x + x , basePoint.z + y);
                    // Check if height difference is within one block from the starting height
                    if (abs(targetHeight - height) <= 1) {
                        playerPosition = mcpp::Coordinate(basePoint.x + x , targetHeight + 1, basePoint.z + y );
                    }
                }
            }
        }
        mc.setPlayerPosition(playerPosition);
        std::cout << "Player moved inside the Maze" << std::endl;
    } else {
        // Place the player in a random empty cell in non-testing mode
        std::vector<mcpp::Coordinate> emptyCells;
        for (unsigned int y = 0; y < withoffset; ++y) {
            for (unsigned int x = 0; x < heightoffset; ++x) {
                if (mmaze[y][x] == '.') {  // Collect all empty cells
                    int cellHeight = mc.getHeight(basePoint.x + x , basePoint.z + y);
                    // Collect cells without height restriction in non-testing mode
                    emptyCells.push_back(mcpp::Coordinate(basePoint.x + x , cellHeight + 1, basePoint.z + y ));
                }
            }
        }

        if (!emptyCells.empty()) {
            playerPosition = emptyCells[rand() % emptyCells.size()];  // Pick a random empty cell
            mc.setPlayerPosition(playerPosition);
            std::cout << "Player moved inside the Maze" << std::endl;
        } else {
            std::cout << "Could not find space in the maze" << std::endl;
        }
    }
    //std::cout << "Player Position: (" << playerPosition.x << ", " << playerPosition.y << ", " << playerPosition.z << ")" << std::endl;
}


    


// Shortest path - E2

void Maze::solveWithBFS(mcpp::MinecraftConnection& mc) {
    std::cout << "Looking for the shortest path..." << std::endl;
    mcpp::Coordinate start = mc.getPlayerTilePosition();
    start.y += 1; // Start just above the tile the player is on

    std::queue<mcpp::Coordinate> queue;
    queue.push(start);

    std::list<mcpp::Coordinate> visited = {start};
    std::list<std::pair<mcpp::Coordinate, mcpp::Coordinate>> previous;
    bool foundExit = false;
    mcpp::Coordinate exitPosition;

    std::vector<mcpp::Coordinate> directions = {MOVE_XMINUS, MOVE_XPLUS, MOVE_ZMINUS, MOVE_ZPLUS};

    // BFS loop
    while (!queue.empty() && !foundExit) {
        mcpp::Coordinate current = queue.front();
        queue.pop();

        for (const auto& direction : directions) {
            if (!foundExit) {
                mcpp::Coordinate neighbor = current + direction;

                bool isVisited = std::find(visited.begin(), visited.end(), neighbor) != visited.end();
                if (!isVisited && isValidMove(mc, neighbor.x, neighbor.y, neighbor.z)) {
                    visited.push_back(neighbor);
                    previous.push_back({neighbor, current});
                    queue.push(neighbor);

                    if (mc.getBlock(neighbor) == mcpp::Blocks::BLUE_CARPET) {
                        foundExit = true;
                        exitPosition = neighbor;
                    }
                }
            }
        }
    }

    if (foundExit) {
        std::vector<mcpp::Coordinate> path;
        mcpp::Coordinate pathStep = exitPosition;

        while (pathStep != start) {
            // Check if the pathStep already exists in the path to avoid duplicates
            if (std::find(path.begin(), path.end(), pathStep) == path.end()) {
                path.push_back(pathStep);
            }
            bool stepFound = false;

            for (const auto& entry : previous) {
                if (!stepFound && entry.first == pathStep) {
                    pathStep = entry.second;
                    stepFound = true;
                }
            }
            
            if (!stepFound) {
                pathStep = start; // Force loop exit condition if step not found
            }
        }

        if (std::find(path.begin(), path.end(), start) == path.end()) {
            path.push_back(start);
        }

        std::reverse(path.begin(), path.end());

        int stepCount = 1;
        std::cout << "Follow the lime carpet..." << std::endl;
        // Display the path in Minecraft and print the coordinates
        for (const auto& step : path) {
            mc.setBlock(step, mcpp::Blocks::LIME_CARPET);
            std::this_thread::sleep_for(std::chrono::milliseconds(500)); 

            std::cout << "Step[" << stepCount << "]: (" << step.x << ", " << step.y << ", " << step.z << ")" << std::endl;
            stepCount++;

            // Clear the block after display to maintain the animation effect
            mc.setBlock(step, mcpp::Blocks::AIR);

            if (step == exitPosition) {
                mc.setBlock(exitPosition, mcpp::Blocks::BLUE_CARPET);
            }
        }

        std::cout << "You have reached the exit!" << std::endl;
    } else {
        std::cout << "No path found to the exit!" << std::endl;
    }
}



bool Maze::isWall(mcpp::MinecraftConnection& mc, const mcpp::Coordinate& coord) {
    mcpp::BlockType block = mc.getBlock(coord);
    return block == mcpp::Blocks::ACACIA_WOOD_PLANK;
}

bool Maze::isValidMove(mcpp::MinecraftConnection& mc, int x, int y, int z) {
    mcpp::BlockType block = mc.getBlock(mcpp::Coordinate(x, y, z));
    return (block != mcpp::Blocks::ACACIA_WOOD_PLANK);
}
// bool Maze::isValidMoveRHR(mcpp::MinecraftConnection& mc, int x, int y, int z) {
//     mcpp::BlockType block = mc.getBlock(mcpp::Coordinate(x, y, z));
//     return block == mcpp::Blocks::AIR || block == mcpp::Blocks::BLUE_CARPET;
// }

 bool Maze::heightCheck(mcpp::MinecraftConnection& mc, int x, int y, int z) {
    // Get the height of the current position and the target position
    int targetHeight = mc.getHeight(x, z);      // Target block height at the given x, z coordinates
    // Check if the height difference between the current block and the target block is <= 1
    bool canMove = true;

    if (abs(targetHeight - y) > 1) {
        canMove = false;
    }
    return canMove;
}
 

void Maze::escapeRoute(mcpp::MinecraftConnection& mc, bool testMode) {
    bool blueCarpetAtPosition = false;
    std::cout << "Follow the lime carpet..." << std::endl;

    mcpp::Coordinate playerPos = mc.getPlayerPosition();
    int coordinateX = playerPos.x;
    int coordinateY = playerPos.y;  
    int coordinateZ = playerPos.z;
    std::cout << "Player Position Starting at (" << coordinateX << ", " << coordinateY << ", " << coordinateZ << ")\n";

    int sideX[] = {0, 1, 0, -1};
    int sideZ[] = {-1, 0, 1, 0};
    int side = 0; 

    if (testMode) {
        side = 1; // Start facing positive x-axis by default
        bool wallDetected = false;
        for (int i = 0; i < 4; ++i) {
            int testX = coordinateX + sideX[(side + 1) % 4];
            int testZ = coordinateZ + sideZ[(side + 1) % 4];
            wallDetected = wallDetected || (!isValidMove(mc, testX, coordinateY, testZ) && heightCheck(mc, testX, coordinateY, testZ));
            side = wallDetected ? side : (side + 1) % 4;
        }
    }

    int numStep = 1;
    if (mc.getBlock(mcpp::Coordinate(coordinateX, coordinateY, coordinateZ)) == mcpp::Blocks::BLUE_CARPET) {
        blueCarpetAtPosition = true;
    }

    while (!blueCarpetAtPosition) {
        coordinateY = mc.getHeight(coordinateX, coordinateZ);
        mc.setBlock(mcpp::Coordinate(coordinateX, coordinateY + 1, coordinateZ), mcpp::Blocks::LIME_CARPET);

        mcpp::Coordinate prevCarpetPosition = mcpp::Coordinate(coordinateX, coordinateY + 1, coordinateZ);

        int directionR = (side + 1) % 4;
        int directionX = coordinateX + sideX[directionR];
        int directionZ = coordinateZ + sideZ[directionR];
        int directionHeight = mc.getHeight(directionX, directionZ);

        if (isValidMove(mc, directionX, directionHeight, directionZ) && heightCheck(mc, directionX, coordinateY, directionZ)) {
            side = directionR;
            coordinateX = directionX;
            coordinateZ = directionZ;
            coordinateY = directionHeight;  // Update Y to the new height
        } else {
            int forwardX = coordinateX + sideX[side];
            int forwardZ = coordinateZ + sideZ[side];
            int forwardHeight = mc.getHeight(forwardX, forwardZ);

            if (isValidMove(mc, forwardX, forwardHeight, forwardZ) && heightCheck(mc, forwardX, coordinateY, forwardZ)) {
                coordinateX = forwardX;
                coordinateZ = forwardZ;
                coordinateY = forwardHeight;  // Update Y to the new height
            } else {
                side = (side + 3) % 4;
            }
        }

        std::cout << "Step[" << numStep << "]: (" << coordinateX << ", " << coordinateY << ", " << coordinateZ << ")" << std::endl;
        numStep++;

        if (mc.getBlock(mcpp::Coordinate(coordinateX, coordinateY + 1, coordinateZ)) == mcpp::Blocks::BLUE_CARPET) {
            blueCarpetAtPosition = true;
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(500));
        mc.setBlock(prevCarpetPosition, mcpp::Blocks::AIR);
    }

    std::cout << "You have reached the exit!" << std::endl;
}



void Maze::withoutFlattening(  mcpp::MinecraftConnection& mc, unsigned int  x,  unsigned int z, Maze* maze) { //E1
    mcpp::Coordinate playerPos = mc.getPlayerPosition();
    int height  = x;
    int breath = z;
     basePoint = playerPos;
     mcpp::Coordinate finalPos = mcpp::Coordinate(basePoint.x + xlen- 1, basePoint.y, basePoint.z + zlen - 1);
     mcpp::HeightMap Map = mc.getHeights(basePoint, finalPos);
    char** mazeTemplate = maze->getMazeStruct();
     
     for (int i = 0; i < height; i++) {
    for (int j = 0; j < breath; j++) {
        try {
            int heightVal = abs(Map.get(i, j));
            if (mazeTemplate[i][j] == 'x') {
                // Left
                if (j > 0) {
                    int heightL = abs(Map.get(i, j - 1));
                    if (abs(heightL - heightVal) >= 2) {
                        mazeTemplate[i][j - 1] = 'o';
                    }
                }
                // Up
                if (i > 0) {
                    int heightU = abs(Map.get(i - 1, j));
                    if (abs(heightU - heightVal) >= 2) {
                        mazeTemplate[i - 1][j] = 'o';
                    }
                }
                // Right
                if (j + 1 < breath) {
                    int heightR = abs(Map.get(i, j + 1));
                    if (abs(heightR - heightVal) >= 2) {
                        mazeTemplate[i][j + 1] = 'o';
                    }
                }
                // Down
                if (i + 1 < height) {
                    int heightD = abs(Map.get(i + 1, j));
                    if (abs(heightD - heightVal) >= 2) {
                        mazeTemplate[i + 1][j] = 'o';
                    }
                }
            }
        } catch (const std::out_of_range& e) {
            std::cerr << "Out of range error: " << e.what() << std::endl;
        }
    }
}

    maze->setMazeStruct(mazeTemplate);
   
 }


void Maze::setMazeStruct(char** newStruct) {
    for (unsigned int i = 0; i < zlen; ++i) {
        for (unsigned int j = 0; j < xlen; ++j) {
            mmaze[i][j] = newStruct[i][j];
            //   std::cout << mmaze[i][j];
        }
    }
}

char** Maze::getMazeStruct() const  {
    return mmaze;
}


void Maze::buildMaze2ForE1(mcpp::MinecraftConnection& mc, unsigned int xlen, unsigned int zlen)  { //E1
    std::cout << "Building the maze in Minecraft...\n" << std::flush;

    mcpp::BlockType wallBlock = mcpp::Blocks::ACACIA_WOOD_PLANK;
    mcpp::BlockType exitBlock = mcpp::Blocks::BLUE_CARPET;

    for (unsigned int z = 0; z < zlen; ++z) {
        for (unsigned int x = 0; x < xlen; ++x) {
            // Get terrain height at this (x, z) location
            int terrainHeight = mc.getHeight(basePoint.x + x, basePoint.z + z);

            if (mmaze[z][x] == 'x') {  // Place wall blocks
                for (int y = 0; y < 3; ++y) {  // Place 4 blocks high
                    mcpp::Coordinate loc(basePoint.x + x, terrainHeight + y+1, basePoint.z + z);
                    modifiedMazeBlocks.push_back({loc, mc.getBlock(loc)});
                    mc.setBlock(loc, wallBlock);
                    std::this_thread::sleep_for(50ms);
                }
            } 
            else if (mmaze[z][x] == '.' && (z == zlen - 1 || z == 0 || x == xlen - 1 || x == 0)) {  // Check for boundary
                int terrainHeight = mc.getHeight(basePoint.x + x, basePoint.z + z);
                mcpp::Coordinate loc(basePoint.x + x, terrainHeight + 1, basePoint.z + z);  // Place carpet one block above ground
                modifiedMazeBlocks.push_back({loc, mc.getBlock(loc)});
                mc.setBlock(loc, exitBlock);

            }
        }
    }

    std::cout << "The maze was successfully built in Minecraft!\n";
}
