# Game of Life Simulation

This is a simple implementation of Conway's Game of Life simulation using SDL2 for visualization. The Game of Life is a cellular automaton devised by the British mathematician John Horton Conway in 1970. It's a zero-player game, meaning that its evolution is determined by its initial state, requiring no further input. 

## How to Use

### Prerequisites
- Either SDL2 library or SDL2_ttf library

### Building and Running
1. Clone this repository.
2. Install the required library.
3. Compile the code using a C compiler. For example, if you have SDL2 installed:

    ```bash
    gcc -o game_of_life game_of_life.c -lSDL2 -lSDL2_ttf
    ```

    If you have SDL2_ttf installed:

    ```bash
    gcc -o game_of_life game_of_life.c -lSDL2_ttf
    ```

4. Run the executable:

    ```bash
    ./game_of_life
    ```

5. To specify the initial configuration of the simulation, you need to use either `-randomGrid <nb>` or `-pentomino <nb>`. Here's how to use them:

    - `-randomGrid <nb>`: Populate the grid with a random initial configuration of `<nb>` cells.
    - `-pentomino <nb>`: Populate the grid with `<nb>` R-pentomino shapes (a specific initial pattern).

    For example:
    ```bash
    ./game_of_life -randomGrid 1500
    ```
    This will populate the grid with 1500 randomly placed cells.
    ![vid-2024-04-15_01 52 43-ezgif com-video-to-gif-converter](https://github.com/Andrebtk/GameOfLife/assets/53980377/f127bdd2-5911-4073-85c9-b860f60c276f)

     <br /><br />

    ```bash
    ./game_of_life -speed normal -pentomino 25
    ```
    This will populate the grid with 25 R-pentomino shapes.

   ![vid-2024-04-15_01 42 30-ezgif com-video-to-gif-converter](https://github.com/Andrebtk/GameOfLife/assets/53980377/9f15157f-12b2-4383-accc-48d10df5e061)


    Optionally, you can also specify the simulation speed using `-speed <slow|normal|fast>` to set the speed of the simulation.


## Controls
- **Quit**: Close the window to exit the simulation.

## Additional Information
- This code uses a fixed grid size of 1230x780 pixels, where each cell is 10x10 pixels.
- You can modify the code to change the initial configuration or add new patterns.
