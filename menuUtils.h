// #include <iostream>

// void printStartText(void){
//     std::cout << std::endl;
//     std::cout << "Welcome to MineCraft MazeRunner!" << std::endl;
//     std::cout << "--------------------------------" << std::endl;
// }

// void printMainMenu(void){
//     std::cout << std::endl;
//     std::cout << "------------- MAIN MENU -------------" << std::endl;
//     std::cout << "1) Generate Maze" << std::endl;
//     std::cout << "2) Build Maze in MineCraft" << std::endl;
//     std::cout << "3) Solve Maze" << std::endl;
//     std::cout << "4) Show Team Information" << std::endl;
//     std::cout << "5) Exit" << std::endl;
//     std::cout << std::endl;
//     std::cout << "Enter Menu item to continue: " << std::endl;
// }

// void printGenerateMazeMenu(void){
//     std::cout << std::endl;
//     std::cout << "------------- GENERATE MAZE -------------" << std::endl;
//     std::cout << "1) Read Maze from terminal" << std::endl;
//     std::cout << "2) Generate Random Maze" << std::endl;
//     std::cout << "3) Back" << std::endl;
//     std::cout << std::endl;
//     std::cout << "Enter Menu item to continue: " << std::endl;
// }

// void printSolveMazeMenu(void){
//     std::cout << std::endl;
//     std::cout << "------------- SOLVE MAZE -------------" << std::endl;
//     std::cout << "1) Solve Manually" << std::endl;
//     std::cout << "2) Show Escape Route" << std::endl;
//     std::cout << "3) Back" << std::endl;
//     std::cout << std::endl;
//     std::cout << "Enter Menu item to continue: " << std::endl;
// }


// void printTeamInfo(void){
//     std::cout << std::endl;
//     std::cout << "Team members:" << std::endl;

//     //TODO: change this part to reflect your team**********
//     std::cout << "\t [1] Karanpreet Singh (s4043579@student.rmit.edu.au)" << std::endl;
//     std::cout << "\t [2] Lav Bujak (s4094261@student.rmit.edu.au)" << std::endl;
//     std::cout << "\t [3] Gayath Wethmin Kaluwahewa (S4052285@student.rmit.edu.au)" << std::endl;
//     //*************

//     std::cout << std::endl;
// }

// void printExitMassage(void){
//     std::cout << std::endl;
//     std::cout << "The End!" << std::endl;
//     std::cout << std::endl;
// }








#ifndef MENU_UTILS_H
#define MENU_UTILS_H

void printStartText();
void printMainMenu();
void printTeamInfo();
void printSolveMazeMenu();
void printExitMassage();
void printGenerateMazeMenu();

#endif // MENU_UTILS_H
