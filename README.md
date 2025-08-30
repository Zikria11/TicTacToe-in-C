Tic-Tac-Toe Game
================

Overview
--------

This is a visually enhanced Tic-Tac-Toe game built using the **Raylib** library in C++. The game features a 3x3 grid where two players take turns placing 'X' or 'O' symbols to achieve three in a row, column, or diagonal. It includes smooth animations, particle effects, and a dynamic user interface to elevate the classic game experience.

Features
--------

*   **Interactive Gameplay**: Players alternate placing 'X' or 'O' by clicking on empty cells.
    
*   **Animations**: Fade-in, scale, rotation, and bounce effects for each move.
    
*   **Particle Effects**: Sparks and particles appear when a move is made or a player wins.
    
*   **Winning Line Animation**: A glowing line highlights the winning row, column, or diagonal.
    
*   **Hover Preview**: Shows a faint preview of the current player's symbol when hovering over an empty cell.
    
*   **Dynamic UI**: Displays the current player, game result, and restart instructions with pulsing effects.
    
*   **Responsive Design**: Centered 480x480 grid within an 800x800 window, with a gradient background and subtle glow effects.
    

Prerequisites
-------------

*   **Raylib**: Ensure the Raylib library is installed and linked correctly.
    
*   **C++ Compiler**: A compiler supporting C++11 or later (e.g., g++).
    
*   **Development Environment**: Set up to include Raylib headers and link against the Raylib library.
    

Installation
------------

1.  **Install Raylib**:
    
    *   Follow the official [Raylib installation guide](https://www.raylib.com/) for your platform.
      ``` bash  
       sudo apt-get install libraylib-dev
      ```  
2.  **Compile the Game**:
    
    *   Save the provided C++ code in a file, e.g., tictactoe.cpp.
      ``` bash  
       g++ tictactoe.cpp -o tictactoe -lraylib -lGL -lm -lpthread -ldl -lrt -lX11
      ```  
3.  **Run the Game**:
    ``` bash
     ./tictactoe
    ```    

How to Play
-----------

*   **Objective**: Be the first to align three of your symbols ('X' or 'O') horizontally, vertically, or diagonally.
    
*   **Controls**:
    
    *   **Mouse Click**: Click on an empty cell to place your symbol.
        
    *   **Spacebar or Click**: When the game ends (win or draw), press the spacebar or click to restart.
        
*   **Game Flow**:
    
    *   Player 'X' starts, followed by Player 'O'.
        
    *   The game ends when a player wins or the board is full (draw).
        
    *   A win displays a glowing line and the result text (e.g., "X Wins!").
        
    *   A draw displays "It's a Draw!".
        

Code Structure
--------------

*   **Main Components**:
    
    *   CellAnimation: Manages fade-in, scale, rotation, and bounce animations for each cell's symbol.
        
    *   ParticleEffect: Handles particle effects for moves and wins.
        
    *   WinLineAnimation: Animates the winning line with a glowing effect.
        
*   **Game Logic**:
    
    *   The game state is stored in a 3x3 board vector.
        
    *   Win conditions are checked after each move (rows, columns, diagonals).
        
    *   A draw is detected when the board is full with no winner.
        
*   **Rendering**:
    
    *   Uses Raylib for drawing the grid, symbols, particles, and text.
        
    *   Includes a gradient background, glowing grid lines, and animated UI elements.
        

Customization
-------------

*   **Colors**: Modify the predefined colors (primaryX, secondaryX, primaryO, secondaryO, etc.) in the code to change the visual theme.
    
*   **Grid Size**: Adjust gridSize and cellSize for a different board size (ensure it fits within screenWidth and screenHeight).
    
*   **Animation Speed**: Modify animation parameters (e.g., fadeIn, scale, rotation) in the CellAnimation updates for different effects.
    
*   **Particle Effects**: Change the number, speed, or size of particles in the ParticleEffect creation.
    

Known Limitations
-----------------

*   No AI opponent; designed for two human players.
    
*   Limited to a 3x3 grid.
    
*   No menu or settings screen; the game starts immediately.
    

Future Enhancements
-------------------

*   Add an AI opponent for single-player mode.
    
*   Include a main menu for game settings (e.g., board size, difficulty).
    
*   Support sound effects for moves and wins.
    
*   Add a score tracker for multiple rounds.
    
## Screenshots
<img width="792" height="828" alt="image" src="https://github.com/user-attachments/assets/b3847733-1ef1-42fe-847f-c345475c5a4d" />


<img width="797" height="833" alt="image" src="https://github.com/user-attachments/assets/78647f2d-f89b-4438-a1e1-bef54bed3c03" />



Dependencies
------------

*   **Raylib**: For graphics, input handling, and window management.
    
*   **C++ Standard Library**: Uses vector, string, algorithm, and cmath for game logic and animations.
    

License
-------

This project is unlicensed and provided as-is for educational purposes. Feel free to modify and distribute.

Credits
-------

Developed with love using Raylib. Inspired by the classic Tic-Tac-Toe game with a modern, animated twist.
