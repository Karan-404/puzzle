# Group Project Information

This project was developed as a group assignment for COSC2804 Programming Studio at RMIT University by Group 116:
- Karanpreet Singh 
- Gayath Wethmin Kaluwahewa 
- Lav Bujak 


# Getting Started: Setting up mcpp and Minecraft

To run this project and communicate with Minecraft using C++, you need to set up a local Minecraft Spigot server and the mcpp (Minecraft++) C++ library. Follow these steps:

## 1. Install WSL 2 and Ubuntu 22.04 (Windows Only)
- Open PowerShell as Administrator and run:
  ```
  wsl --install -d Ubuntu-22.04
  ```
- After installation, launch Ubuntu from the Start menu.

## 2. Install Java (JRE 17+)
In your WSL/Ubuntu terminal:
```bash
sudo apt update
sudo apt upgrade
sudo apt install openjdk-17-jdk
```
Verify with:
```bash
java --version
```

## 3. Download Minecraft Tools and Spigot Server
```bash
cd ~
mkdir PS2
cd PS2
git clone https://github.com/rozukke/minecraft_tools.git
```

## 4. Start the Minecraft Server
```bash
cd ~/PS2/minecraft_tools
chmod a+x start_server.sh
./start_server.sh
```
Keep this terminal open while working.

## 5. Install Minecraft Java Edition (1.19.4)
- In the Minecraft Launcher, create a new installation for version 1.19.4.
- Launch Minecraft, go to Multiplayer, and add a server with address `::1`.

## 6. Install C++ Tools and mcpp
```bash
sudo apt-get install make g++
sudo apt install cmake
cd ~/PS2
git clone https://github.com/rozukke/mcpp.git
cd mcpp
cmake -B build
cd build
sudo mkdir -p /usr/local/lib
sudo make install
```
If you see a library error later, run:
```bash
sudo ldconfig
```

## 7. Test the Setup
Create a file `hello_minecraft.cpp`:
```cpp
#include <mcpp/mcpp.h>
int main() {
    mcpp::MinecraftConnection mc;
    mc.postToChat("Hello Minecraft!");
}
```
Compile and run:
```bash
g++ -std=c++17 -Wall -o hello_minecraft hello_minecraft.cpp -lmcpp
./hello_minecraft
```
You should see “Hello Minecraft!” in the Minecraft chat.

# How to Compile and Run with the Makefile

To build and run the project using the provided Makefile, use the following commands in your project directory (inside WSL/Ubuntu):

```bash
make        # Compiles the project and produces the mazeRunner executable
./mazeRunner    # Runs the program in normal mode
./mazeRunner -testmode   # Runs the program in test mode
```

If you want to clean up compiled files, use:
```bash
make clean
```
## Karanpreet Singh 

### Solve Maze

- **Solve Manually (normal):**
  - Empty space coordinates are pushed into a vector and chosen randomly to move to an empty place in the maze.
  - A vector is used to store empty space coordinates as the number of empty spaces is unknown and constant O(1) time complexity is required.
  - To use this feature, the maze must be built and generated first.

- **Solve Manually (testmode):**
  - The farthest cell is chosen from the basepoint using nested loops to find the farthest cell (using the last value of player position as the player position).

- **Solve using Right Hand Rule Escape Route (normal):**
  - The right hand rule is used to find the escape route. The structure of the maze is not needed to find the escape route.
  - Always try to move to the right; if blocked, then forward; if blocked, then left.
  - Assumes the maze is perfect. If there is a loop, the player will be stuck in the loop.
  - Can be used to solve any perfect maze (built by hand or generated) if the wall structure is made of `ACACIA_WOOD_PLANK` and the exit is marked with `BLUE_CARPET`.
  - You can change `Maze::isValidMove` in `maze.cpp` to adjust the wall structure if a different material is used for walls.

- **Solve using Right Hand Rule Escape Route (testmode):**
  - The same right hand rule is used. Assume the player is initially facing the positive x-axis and adjust orientation until the right hand faces a wall.

---

# Enhancement E1: Create a Maze Without Flattening the Terrain

- **Access via:** `/Generate Maze/ Generate Maze Without Flattening the Terrain`
- Uses `mcpp::getHeights` to get the height of the terrain at each point (with starting and ending points of the maze).
- The maze is created without removing or flattening any blocks.
- If the height of a block is greater than 2, it is treated as a wall; if less than 2, it is treated as a floor (since the player can't jump more than 1 block high).
- Walls are built from ground level, leaving no space underneath and disconnecting the player from the outside world.
- The right hand rule can be used to find the escape route, but the height of the terrain at each point must be checked.
- `solveManualforE1`: Due to different building techniques (+2 in length and width of basepoint in building and flattening the world maze without enhancement), the basepoint must be adjusted, so a new solve manual was needed with minor adjustments. Moreover, `SolveManual` didn't need to check height at every point. The same reasons apply for `BuildMazeforE1`.
- **Important:** For unflattening the world, you need to be at a new location. If you build at the old point, the world will be generated based on the current world (including the maze), which will be removed, leaving empty space and a bad maze.

**Workarounds:**
- Type `back` and press Enter to return to the main menu and restart the program, then build the maze again.
- Go to a new location and press `done`, or simply press `done` (this might lead to a bad maze).
- Other options like auto-deleting the maze before generating the structure were not chosen as they limit user freedom and user experience.



### Gayath Wethmin Kaluwahewa 

- Reading maze from terminal:
  I'm using a 2D dynamically allocated array to store the maze structure
- Building maze in minecraft world and flattening the terrain
  Link list is used to store the changes to the world so upon exit or building a new maze, it can be reversed.
- E2
  For this, Im uing BFS(breadth first search) as recommended. I did try some other alogorothms such as, A Star Search(Manhattan Distance) and Depth First Search but, this was the most efficient one, so i stuck with it.    

Note: Minecraft has GRASS(No ID in mcpp) and GRASS BLOCKs(ID 2). Upon clearing the terrain on Grass Blocks (ID 2), if there is GRASS on top of the GRASS BLOCK it will not be removed. So I tried implementing a methode to remove the remaining GRASS(NOT GRASS BLOCKS) on the GRASS BLOCKs. But it turns out that there is no ID for GRASS in mcpp.(There are ids for LONG GRASS and GRASS BLOCKS but not for GRASS). So I let it be because replacing with air did not seem like a logical way of going about it since if you are on a different terrain, you would be replacing air blocks for no reason and its not practical for larger mazes.


### Lav Bujak 

- Random maze generation:
I have done this by using the recursive backtracking method as suggested in the specification file

- E3:
I have added an extra option inside the Generate Maze menu options. I have used the floodfill recursive function to assist me to find both the loops and isolated areas inside the maze from terminal input, since the backtracking algorithm is almost "perfect" therefore doesn't create those issues.
 

---

## Error Handling (Team)

All team members have implemented error handling in their respective code. The error handling is designed to:
- Handle incorrect input
- Inform the user that an input error has been made
- Indicate what the expected inputs are
- Safely move to an appropriate program state without exiting or crashing

---

## Test Cases

### Solve Menu

- `Tests/noGenSolMan.expout` & `Tests/noGenSolMan.input`  
  Check if you can solve manually without generating the maze first or building maze with enhancement E1

- `Tests/solveManualwithoutbuilt.expout` & `Tests/solveManualwithoutbuilt.input`  
  Solving maze without building and just generating

- `Tests/solveManualwithbuilt.expout` & `Tests/solveManualwithbuilt.input`  
  Solving maze with built

- `Tests/solveMazeInput.input` & `Tests/solveMaze.expout`  
  How solve menu responds to user input and error, including ctrl+D

- `Tests/E1error.input` & `Tests/E1error.expout`  
  Check if user can handle error when building maze with enhancement E1



