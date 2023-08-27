Conway's game of life implemented with a wxWidgets frame
This code is dependent on wxWidgets cross-platform GUI library

This project is a simulation of Conway's Game of Life, a classic cellular automaton devised by mathematician John Conway. 
The Game of Life is a zero-player game, meaning that its evolution is determined by its initial state, 
requiring no further input.

Project Features:
- This simulator allows users to customize various aspects of the game, including the grid size, cell colors,
  and HUD overlays.
- Users can save and reload game states, allowing for continued exploration and experimentation.
- The simulator offers options for starting with user-generated seed configurations or generating random boards
  based on global time.
- It includes essential game functions such as play, pause, play one generation, and clear the board.
- The program responds to mouse events, enabling users to interact with the grid by toggling cell states.
- Users can choose whether the edges of the grid are treated as static or toroidal, adding versatility to the simulation.

Rules of the Game:
- The game is played on a grid of cells, each of which can be in one of two states: alive or dead.
- The game evolves in discrete steps, called generations. In each generation, the following rules are applied to each cell:
  1. Underpopulation: If a live cell has fewer than two live neighbors, it dies due to loneliness.
  2. Survival: If a live cell has two or three live neighbors, it survives to the next generation.
  3. Overpopulation: If a live cell has more than three live neighbors, it dies due to overcrowding.
  4. Reproduction: If a dead cell has exactly three live neighbors, it becomes a live cell in the next generation.
- These rules are applied simultaneously to all cells in the grid. As a result, complex patterns can emerge from
  simple initial configurations.

To run Conway's Game of Life simulator, you need to download and compile wxWidgets. 

Contributions to this project are welcome. If you have ideas for improvements or bug fixes, please
Create a pull request.
