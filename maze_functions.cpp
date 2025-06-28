#include <iostream>
#include <limits>
#include <string>
#include <sstream>
#include <mcpp/mcpp.h>
#include "maze_functions.h"
#include "menuUtils.h"
#include "Maze.h"
#include "terrain.h"

bool isInteger(const std::string& s) {
    bool result = true; // Assume the string is an integer by default

    if (s.empty()) {
        result = false;
    } else {
        size_t start = 0;
        if (s[0] == '-') start = 1;

        for (size_t i = start; i < s.size() && result; ++i) {
            if (!std::isdigit(s[i])) {
                result = false;
            }
        }
    }

    return result;
}



void checkEOF() {
    if (std::cin.eof()) {
        std::cout << "\nEOF received. Program will now safely exit." << std::endl;
        std::exit(EXIT_SUCCESS);
    }
}


void handleMainMenu(States& curState) {
    printMainMenu();
    int choice;
    std::cin >> choice;    
    checkEOF();
    // if (std::cin.eof()) {
    //     std::cout << "\nEOF received. Program will now safely exit." << std::endl;
    //     std::exit(EXIT_SUCCESS);
    // }

    if (std::cin.fail()) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "Input Error: Enter a number between 1 and 5 ...." << std::endl;
    } else if (choice == 1) {
        curState = ST_GetMaze;
    } else if (choice == 2) {
        curState = ST_BuildMaze;
    } else if (choice == 3) {
        curState = ST_SolveMaze;
    } else if (choice == 4) {
        curState = ST_Creators;
    } else if (choice == 5) {
        curState = ST_Exit;
    } else {
        std::cout << "Input Error: Enter a number between 1 and 5 ...." << std::endl;
    }
}

void handleGetMaze(mcpp::MinecraftConnection& mc, Maze*& maze, mcpp::Coordinate& playerPos, int& xlen, int& zlen, bool& maze_generated, bool& withoutFlattening, States& curState, bool testMode) {
    printGenerateMazeMenu();
    std::string choice;
    std::cin >> choice;
    checkEOF();
    // if (std::cin.eof()) {
    //     std::cout << "\nEOF received. Program will now safely exit." << std::endl;
    //     cleanupAndExit(maze, mc);
    // }

    if (choice == "1") {
        if(testMode) {
            mcpp::Coordinate testM(4848, 71, 4369);
            std::cout << "Test mode: Player teleported to (" << testM.x << ", " << testM.y << ", " << testM.z << ")" << std::endl;
            mc.setPlayerPosition(testM);
            playerPos = testM;
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        } else {
            std::string input;
            std::cout << "In Minecraft, navigate to where you need the maze to be built in Minecraft and type - done:\n";
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::getline(std::cin, input);
            while (input != "done" && input != "DONE" && input != "Done") {
                std::cout << "Please type 'done' when ready:\n";
                std::getline(std::cin, input);
                checkEOF();
            }
        }
        playerPos = mc.getPlayerPosition();
        std::cout << "Enter the length and width of the maze:\n";
        bool validInputs = false;
        while (!validInputs) {
            std::string inputLine;
            std::getline(std::cin, inputLine);
            checkEOF();
            std::istringstream inputStream(inputLine);
            std::string xlenStr, zlenStr, extra;
            inputStream >> xlenStr >> zlenStr;

            if (isInteger(xlenStr) && isInteger(zlenStr)) {
            xlen = std::stoi(xlenStr);
            zlen = std::stoi(zlenStr);
            if (xlen >= 3 && zlen >= 3) {  // Ensure minimum dimensions
                if (xlen % 2 != 0 && zlen % 2 != 0) {  // Check for odd dimensions
                    validInputs = true;
                } else {
                    std::cout << "Both length and width need to be odd numbers! Please enter again:\n";
                }
            } else {
                std::cout << "Input Error: Dimensions must be at least 3. Please enter again:\n";
            }
        } else {
            std::cout << "Input Error: Only positive integer values are allowed for dimensions. Please enter again:\n";
        }
    }
        
        maze = new Maze(playerPos, xlen, zlen);
        if (maze) {
            maze->createFromInput(mc);
            maze_generated = true;
            curState = ST_Main;
            withoutFlattening = false;
        } else {
            std::cout << "Failed to create maze. Try again." << std::endl;
        }

    } else if (choice == "2") {
        std::string input;
        std::cout << "In Minecraft, navigate to where you need the maze to be built in Minecraft and type - done:\n";
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::getline(std::cin, input);
        checkEOF();
        while (input != "done" && input != "DONE" && input != "Done") {
            std::cout << "Please type 'done' when ready:\n";
            std::getline(std::cin, input);
            checkEOF();
        }
        playerPos = mc.getPlayerPosition();
        std::cout << "Enter the length and width of the maze:\n";
        std::cin >> xlen >> zlen;
        checkEOF();

        while (xlen <= 0 || zlen <= 0) {
            if (std::cin.fail()) {
                std::cout << "Invalid input, please enter numeric values.\n";
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            } else if (xlen <= 0 || zlen <= 0) {
                std::cout << "Dimensions must be greater than 0. Try again.\n";
            }
            std::cout << "Enter the length and width of the maze:\n";
            std::cin >> xlen >> zlen;
            checkEOF();
        }

        // Ensure both dimensions are odd
        while (xlen % 2 == 0 || zlen % 2 == 0 || xlen == 1 || zlen == 1) {
            std::cout << "Both length and width need to be odd numbers (numbers can't be 1)!!!" << std::endl;
            std::cout << "Enter the length and width of the maze:\n";
            std::cin >> xlen >> zlen;
            checkEOF();
        }

        maze = new Maze(playerPos, xlen, zlen);
        if (maze) {
            maze->generateMaze(testMode);
            maze->printMaze(mc);
            maze_generated = true;
            curState = ST_Main;
            withoutFlattening = false;
        } else {
            std::cout << "Failed to create maze. Try again." << std::endl;
        }
        } else if(choice =="3") {
        std::string input;
bool ready = false;

std::cout << "Suggestion: Create a maze in a new place other than the existing maze position to get a Perfect Maze.\n"
             " OR Exit and Restart if you want to build the maze in an existing maze position (Check README.md for details).\n";
             
std::cout << std::endl;

std::cout << "In Minecraft, navigate to where you want the maze to be built and type 'done':\n";
std::cout << "Type 'back' to return to the main menu.\n";

// Wait for 'done' input to proceed
std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
std::getline(std::cin, input);

while (!ready) {
    if (input == "done" || input == "DONE" || input == "Done") {
        ready = true;
    } else if (input == "back" || input == "BACK" || input == "Back") {
        curState = ST_Main;
        ready = true; 
    } else {
        std::cout << "Please type 'done' when ready, or 'back' to return to the main menu:\n";
        std::getline(std::cin, input);
        checkEOF();
    }
}

if (input == "done" || input == "DONE" || input == "Done") {
    playerPos = mc.getPlayerPosition();
    std::cout << "Enter the length and width of the maze (must be odd numbers):\n";

    bool validInputs = false;
    while (!validInputs) {
        std::string inputLine;
        std::getline(std::cin, inputLine);
        checkEOF();

        std::istringstream inputStream(inputLine);
        std::string xlenStr, zlenStr;
        inputStream >> xlenStr >> zlenStr;

        if (isInteger(xlenStr) && isInteger(zlenStr)) {
            xlen = std::stoi(xlenStr);
            zlen = std::stoi(zlenStr);

            // Check if dimensions are odd numbers and greater than 2
            if (xlen > 2 && zlen > 2 && xlen % 2 == 1 && zlen % 2 == 1) {
                validInputs = true;
            } else if (xlen <= 2 || zlen <= 2) {
                std::cout << "Input Error: Dimensions must be greater than 2. Please enter again:\n";
            } else {
                std::cout << "Input Error: Both dimensions must be odd numbers. Please enter again:\n";
            }
        } else {
            std::cout << "Input Error: Only positive integer values are allowed for dimensions. Please enter again:\n";
        }
    }

    // Create and generate the maze
    maze = new Maze(playerPos, xlen, zlen);
    if (maze) {
        maze->generateMaze(testMode);
        //std::cout << xlen << " <> " << zlen << std::endl;
        maze->withoutFlattening(mc , xlen, zlen, maze);
        
        maze->printMaze2(mc);
        maze_generated = true;
        curState = ST_Main;
        withoutFlattening = true;
    } else {
        std::cout << "Failed to create maze. Try again." << std::endl;
    }
}

    
    } else if(choice == "4"){
        bool loop = maze->checkLoop();
        bool iso = maze->checkIso();
        if(loop || iso){
            std::cout << "The entered maze has some issues, would you like to:\n";
            std::cout << "1. Enter a new maze with same dimensions\n";
            std::cout << "2. Fix it automatically by the program\n";

            int choice;
            std::cin >> choice;

            if (std::cin.fail()) {
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                std::cout << "Input Error: Enter a number between 1 and 5 ...." << std::endl;
            } else if (choice == 1) {
                maze->createFromInput(mc);
            } else if (choice == 2) {
                maze->checkLoopMaze();
                maze->checkMaze();
                maze->printMaze(mc); 
            } else {
                std::cout << "Input Error: Enter either 1 or 2 ...." << std::endl;
            }
        }
        else{
            std::cout << "\nThis maze is perfect, no change needed\n";
        }
        curState = ST_Main;


    }else if (choice == "5") {
        
        curState = ST_Main;
    } else {
        std::cout << "Input Error: Enter a number between 1 and 5 ...." << std::endl;
    }
}
void handleBuildMaze(mcpp::MinecraftConnection& mc, Maze*& maze, Maze*& previousMaze, bool& maze_built, int xlen, int zlen, bool& withoutFlattening, States& curState) {
    if (!maze) {
        std::cout << "Please create a maze first." << std::endl;
    } else if (maze && maze != previousMaze) {
        if (maze_built) {
            previousMaze->revertMazeChanges(mc);
            delete previousMaze;
        }
        
        // Display maze build options
        if (withoutFlattening) {
           // std::cout << "without flattening " << xlen << " <> " << zlen << std::endl;
            maze->buildMaze2ForE1(mc, xlen, zlen);
        } else {
          
            maze->buildMazeInMinecraft(mc, xlen, zlen);
        }

        previousMaze = maze;
        maze_built = true;
    } 
    else if (maze == previousMaze) {
        std::cout << "The maze is already built." << std::endl;
    }
    
    curState = ST_Main;
}

void handleSolveMaze(mcpp::MinecraftConnection& mc, Maze* maze, int xlen, int zlen, bool& maze_built, bool&  withoutFlattening, States& curState, bool testMode){
    bool isHandled = false;
     if(!isHandled) {
        printSolveMazeMenu();
        std::string solveMenuStr;
        std::cin >> solveMenuStr;
        checkEOF();
        if (isInteger(solveMenuStr)) {
            int solveMenu = std::stoi(solveMenuStr);
              
       
        if (solveMenu == 1 ) {
             if (!maze) {
        std::cout << "Please create a maze first." << std::endl;
        isHandled = true;
        } 
             else if (!maze_built) {
        std::cout << "Please generate the maze first." << std::endl;
        isHandled = true;
         } else  {
             if (withoutFlattening){
          maze->solveManuallyforE1(mc, xlen, zlen , testMode);
             }
           else {
            maze->solveManually(mc, xlen, zlen, testMode);
         }
         }
        }
         else if (solveMenu == 2) {
              std::cout << "The player must be inside a maze for this function to work.\n";
                std::cout << "1) Continue\n";
                std::cout << "2) Back\n";
                
                bool corIn = false;
                while (!corIn) {
                    std::string con;
                    std::getline(std::cin, con);
                    checkEOF();

                    if (con == "1") {
            maze->escapeRoute(mc, testMode);
                        corIn = true;
                    } else if (con == "2") {
                        isHandled = true;
                        corIn = true;
                    } else {
                        std::cout << "Please enter 1 or 2: ";
                    }
                }          
             } 
             
         else if (solveMenu == 3) {
            std::cout << "The player must be inside a maze for this function to work.\n";
                std::cout << "1) Continue\n";
                std::cout << "2) Back\n";
                
                bool corIn = false;
                while (!corIn) {
                    std::string con;
                    std::getline(std::cin, con);
                    checkEOF();

                    if (con == "1") {
            maze->solveWithBFS(mc);
                        corIn = true;
                    } else if (con == "2") {
                        isHandled = true;
                        corIn = true;
                    } else {
                        std::cout << "Please enter 1 or 2: ";
                    }
                }
             } 
        else if (solveMenu == 4) {
            curState = ST_Main;
             } 
        else {
          std::cout << "Input Error: Enter a number between 1 and 4 ...." << std::endl;
        }
        } else {std::cout << "Input Error: Enter a number between 1 and 4 ...." << std::endl;
    }
} 
 
}


void cleanupAndExit(Maze* maze, mcpp::MinecraftConnection& mc) {
    if (maze) {
        maze->revertMazeChanges(mc);
        delete maze;
    }
    printExitMassage();
    std::exit(EXIT_SUCCESS);
}

void safeInput(std::istream& in, std::string& input) {
    std::getline(in, input);
    if (in.eof()) {
        std::cout << "\nEOF received. Program will now safely exit." << std::endl;
        std::exit(EXIT_SUCCESS);
    }
}
