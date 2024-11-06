# Chutes And Ladders Game 🪜🪂
This repository contains a C implementation of a Chutes & Ladders game. The game is designed as a command-line application where two players roll a die and move across a board with chutes (sliding players down) and ladders (lifting players up). Special board features, such as "Havens" and specific move-back and move-forward rules, enhance the gameplay.

**Game Rules**
1. Players: This is a two-player game.
2. Objective: The first player to reach or exceed the end of the board wins.
3. Movement: Each player rolls a die (1-6), by entering 'r', to move forward on the board.
4. Chutes & Ladders:
   * Landing on a chute ('a' to 'm') moves the player back.
   * Landing on a ladder ('o' to 'z') moves the player forward.
   * Chutes and ladders are removed after they are used.
5. Special Squares:
   * B: Moves the player back to the previous Haven (H) or to the start if none exists.
   * F: Moves the player forward to the next Haven (H) or stays in place if none exists.
6. Havens (H): Special locations that act as checkpoints on the board.
**Files**
* main.c: Contains the main game logic, including player turns and game flow.
* README.md: Documentation for the repository.

**Getting Started**
**Prerequisites**
* C Compiler: Ensure you have a C compiler (like gcc) installed.
* Makefile (optional): If provided, you can compile the code using make.
  
**Compilation**

**bash**: _gcc -o chutes_ladders main.c_

**Running the Game**

**bash:** _./chutes_ladders_

The game will then run in the terminal, displaying each player’s moves and current board status after every turn.
