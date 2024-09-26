# **cub3D** - A 2D Raycasting Game Project

## Overview

**cub3D** is a project from the 42 Network curriculum that involves creating a 2D raycasting engine to render a simple 3D perspective of a map. The objective is to implement the basics of a game engine, including rendering, player movement, and map management.

## Table of Contents

- [Overview](#overview)
- [Function Prototypes](#function-prototypes)
- [Project Structure](#project-structure)
- [How It Works](#how-it-works)
- [Compilation and Usage](#compilation-and-usage)

## Function Prototypes

```c
void initialize_game(void);
void render_frame(void);
int handle_input(int key);
```

- **Parameters**:
  - `initialize_game`: Initializes the game window and the player state.
  - `render_frame`: Renders the current frame of the game.
  - `handle_input`: Processes player input for movement and actions.

- **Return**:
  - Each function does not return a value; they manage the game state and rendering.

## Project Structure

```
cub3D/
│
├── Makefile
├── cub3d.h
├── src/
│   ├── main.c
│   ├── renderer.c
│   ├── input.c
│   ├── map.c
│   └── ...
└── README.md
```

- **main.c**: The entry point of the game program.
- **renderer.c**: Contains the rendering logic for drawing the 3D perspective.
- **input.c**: Functions for handling player input and movement.
- **map.c**: Functions for loading and managing the game map.
- **cub3d.h**: The header file containing function prototypes and necessary definitions.
- **Makefile**: Used for compiling the project.

## How It Works

The **cub3D** program operates as follows:
- It initializes the game window and sets up the player's starting position.
- The game processes user input for player movement and actions.
- Each frame, the program calculates the raycasting logic to render the map in a 3D perspective.
- The game loop continues until the player exits.

## Compilation and Usage

You can compile the project using the provided **Makefile**. To generate the object files and the final program, run the following commands:

```bash
# Compile the program
make

# Clean up object files
make clean

# Clean everything (object files and binary)
make fclean

# Recompile everything
make re
```

### Usage in Projects

To use **cub3D**, compile it and run the executable:

```bash
./cub3D map.cub
```

Replace `map.cub` with the path to your game map file.
