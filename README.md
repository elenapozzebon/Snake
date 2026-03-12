# Snake Game

A custom C-based adaptation of the classic Snake game. Instead of an open field, the game takes place inside a maze where the main character (represented by the letter `o`) must navigate through obstacles to reach the final destination (represented by `_`).

## How to Run the Code

To play the game, you need to compile the source code using a C compiler (like GCC).

### Prerequisites
- A C compiler (e.g., `gcc` on Linux/Mac, or MinGW on Windows).
- (Optional but recommended) A modern terminal to properly display ANSI colors.

### Compilation and Execution
1. Open your terminal and navigate to the folder containing the game files (`Snake.c`, `dvector.c`, `dvector.h`).
2. Since the main file directly includes the dynamic vector source (`#include "dvector.c"`), you only need to compile the main file. Run this command:
   ```bash
   gcc Snake.c -o snake
   ```
3. Launch the generated executable:
  On Windows: ```snake.exe```
  On Linux/Mac: ```./snake```

## Game Modes
The game features two distinct modes of execution:

* **Standard Mode**: The user manually inputs the movements (North, South, East, West) via the keyboard to navigate the maze, collect items, and avoid crashing into walls (#) or the snake's own body.
* **AI Mode** (Artificial Intelligence): The game autonomously calculates the path to the finish line. It evaluates valid moves, avoids obstacles, tracks the sequence of steps using a linked list, and automatically drives the snake to the target.

## How It Is Implemented
The game architecture is divided into several core components: dynamic memory management, Artificial Intelligence logic, and maze rendering.

### 1. Libraries Used
The project relies on standard C libraries and OS-specific headers for console management:
  * `<stdlib.h>` & `<stdio.h>`: For dynamic memory allocation and standard input/output operations.
  * `<stdbool.h>`: To handle boolean data types.
  * `<windows.h>` & `<conio.h>`: For low-level I/O and Windows API functionalities, which are essential for coloring the console text and clearing the screen to create a smooth "frame animation" effect.

### 2. Dynamic Vector (dvector) for the Snake's Body
The snake's body is managed using a custom dynamic vector library, separated into the `dvector.h` and `dvector.c` files.

  * **Structure**: The vector keeps track of its current size and the total capacity allocated in memory, along with a pointer to the data vdata_t *data.
  * **Automatic Memory Management**: The implementation includes internal functions like `_v_check_extend` and `_v_check_shrink` that dynamically double (or halve) the allocated space using realloc when the snake's size changes.
  * **Game Dynamics**: This vector allows the snake to "grow" (when collecting the $ bonus) or "shrink" (when hitting the ! penalty) by inserting or removing coordinates from the vector using functions like `v_push_front` and `v_pop_back`.

### 3. AI Logic and Linked Lists
To manage the AI's pathfinding, a custom linked list data structure was implemented:

  * **Data Structures**:
    * `struct list`: Contains a char to save the move (n, s, e, o) and a pointer to the next node.
    * `struct allmoves`: Maintains head and tail pointers. Having a tail pointer makes inserting new moves highly efficient (O(1) complexity).
  * **AI Algorithm**:
    * `AI_modality()` handles the main AI loop, evaluating movements in all directions until the target is reached.
    * `AI_moves()` verifies the validity of a move by calculating future coordinates and checking for collisions with walls (#) or map boundaries. If the move is valid, it traces the path by placing a . and stores the direction in the linked list by calling `push_back_list()`.

### 4. Map Management
The game map is implemented as a single, dynamically allocated one-dimensional array (board) that simulates a 2D matrix by calculating the index with the formula `row * columns + column`.
* The `create_static_lab` function generates the default level, populating the array with walls (#), coins/bonuses ($, !, T), the start (o), and the finish line (_).
* The `create_dynamic_lab function`, on the other hand, allows the user to create a custom map by drawing it in real-time.
