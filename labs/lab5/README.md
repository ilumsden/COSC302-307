# Lab 5

## Introduction

See Wikipedia for a general description of how to generate and solve mazes: <http://en.wikipedia.org/wiki/Maze_generation_algorithm"> We will proceed with a simple version that generates mazes which have lots of the usual deadends. Perhaps somewhat unusual, they allow any cell to be reached from any other cell. This would easy to fix but besides the point of the exercise.

Your job is to write the support code needed to create your own Mazemake and Mazesolve executables which must behave as described next. Mazeshow is a standalone program that you write entirely on your own.

## Mazemake

`Mazemake` generates a random maze. The program takes two commandline arguments, namely, `Nrows` and `Ncols`. Your first job is to write the missing code for this program. Define a maze class and place it in a file called `Support.h`. Place member function implementations in a file called `Support.cpp`. See the provided `Mazemake.cpp` file for public members needed. Your code must be compatible with this main program which you must not change.

Conceptually, imagine an `Nrows x Ncols` grid where the grid lines have been colored in. Now randomly erase grid lines between neighboring cells until paths exist that will allow you to walk from any cell to any other cell. That's the maze you will make. You will treat this maze grid as a graph. How you create the graph is up to you, but keep the following observations in mind. Cell `(i,j)` has four neithbors, namely, `(i-1,j)`, `(i,j-1)`, `(i+1,j)`, and `(i,j+1)`. The connectivity is bidirectional. That is, if `(i-1,j; i,j)` represents a wall, then `(i,j; i-1;j)` represents the same wall viewed in the opposite direction. Same goes when no wall is present. Cells on the periphery do not have neighbors outside the grid. The easiest way to handle them is to never remove the outer walls thereby making it impossible to step off the grid.

Technically speaking, a graph consists of vertices and edges. However, you don't need to store the maze graph vertices since they are merely `(i,j)` index pairs which you can traverse using nested loops. You also don't need to store the edges since they are given by the grid. What you do need to store are Boolean variables that indicate whether a neighbor is walled off or not. To keep us all on the same page, let neighbor index `0` refer to cell `(i-1,j)`, while neighbor indices `1`, `2`, and `3` refer to cells `(i,j-1)`, `(i+1,j)`, and `(i,j+1)`, respectively. Let each of the corresponding four Booleans variables be `true` if there is a wall between `(i,j)` and the cell in question, and `false` if there isn't a wall. This means you can represent the graph as a 2D array which has `Nrows x Ncols` rows and four columns. You look up cell `(i,j)` using index `i*Ncols+j`. This is no different from the matrix class where we stored the data in a 1D buffer and allowed 2D lookup.

Initially, set all cells to be completely walled in (all four Boolean variables are `true`). In order to efficiently select which walls to remove (which Booleans variables to set `false`), generate
a list of all _**interior** neighbor cell pairs,_ which you then subject to random perturbation. Having done that, you merely have to iterate through this list while using a disjoint-set to see if a wall should be removed. How so? First, place each cell in its own set. When given a neighbor cell pair, remove the wall between them if the two cells belong to different sets and merge the cells into the same set. When all cells end up belonging to the same set, stop the process since all cells are now connected to each other. 

The last step is to output the maze to `stdout` for redirection into a file. The output format should be as indicated below. Basically, you state where to put a wall by providing integers that refer to a cell and a wall. Once again, the cell at grid position `(i,j)` is represented by the integer `i*Ncols+j` and the four neighbors `(i-1,j)`, `(i,j-1)`, `(i+1,j)`, and `(i,j+1)` are indicated by 0, 1, 2, and 3.

**Hint**: Keep in mind that if you break down the wall between cell `i` and cell `j` and indicate this in cell `i`'s wall list, you must also update cell `j`'s wall list to indicate the lack of a wall between it and cell `i`. If you forget to this, then you end up with one-way walls which make no sense.

## Mazesolve

Mazesolve finds a path through the maze. The program reads a maze file from `stdin` and outputs the
path to `stdout`. Your job is to add functionality to the maze class. Update the maze class in `Support.h`. Add new member function implementations in `Support.cpp`. See the provided `Mazesolve.cpp` file for public members needed. Your code must be compatible with this main program
which you must not change.

Read the maze file and re-create the graph that represents it by doing the opposite of what you did in the `mazemake` program.
**Hint**: `Mazemake` initialized the maze to have walls between every cell and then proceeded to knock them down. `Mazesolve` should initialize the maze to not have no walls and use the maze file to add them.

Once the graph is in place, apply depth-first search to find a path thru the maze (stack based backtracking). Assume the source is index 0 and the sink is index `Nrows x Ncols - 1`. Keep in mind that you don't want to enter into cycles. That is, you don't want to revisit cells already visited. Use vertex coloring for this purpose. Also, you can't walk through walls. Finally, you need to leave a "breadcrumb trail" as you go along, so you can find your way back. That is, make a note of who the predecessor is when you move from one cell to the next.

The last step is to output the path to `stdout` for redirection into a file. The output format should be as indicated below. This time, you output the sequence of cells on the path.

## Mazeshow (Required for COSC307, optional extra credit opportunity for COSC302)

Mazeshow creates either a ppm image of the maze or a ppm image of the maze with the solution overlaid in yellow (think "Wizard of Oz"). The program takes commandline arguments arguments accordingly:
* a maze file for the former version and
* a maze file and a path for the latter version.
Make this a standalone program that doesn't depend on the `Support.h` and `Support.cpp` files. See inspiration from code handouts and previous labs with ppm file manipulation.

There is not much to say about this program. Create an `(Nrows x width + 1) x (Ncols x width + 1)` sized ppm image where `width` is the number of pixels assigned to a cell. Make `width` a variable, but set it to 11 for the code you submit. Walls should be drawn in the top row or left column of a cell. Since wall index 0 for `(i,j)` is wall index 2 for `(i-1,j)` you really only need to color the former, but there is nothing wrong in coloring both walls, and doing that makes for less checking. Likewise, since wall index 1 for `(i,j)` is wall index 3 for `(i,j-1)`, you really only need to color the former. There is nothing wrong in coloring both walls and doing that makes for less checking. Same problem and solution, same wording.

This probably sounds confusing, but it's actually pretty straightforward. At first, make the ppm image white `(255,255,255)`. Then add the walls which you color black `(0,0,0)`. The only tricky bit is stepping through the `width x width` blocks for each maze grid cell. The bottom and right hand side walls are the single pixel add-ons indicated by the image size `(* + 1) x (* + 1)` above. When a path file is also given, first color the maze grid cells yellow `(240,240,100)`. Then add the black maze grid. The reason for doing the path first is that it's easier to color the entire cell than to work out which pixels are walls.

When done, write the resulting ppm to `stdout` and redirect it into a file. Then take a look! _Word of warning_: A 100 x 100 maze contains 10,000 cells. Using 11 x 11 pixels to represent each cell, the
size of the ppm image is `3 x 10,000 x 121 = 3.5 Mbytes`. Consider piping the output from mazeshow to 
`convert - filename.png` to get a smaller file which is faster to download via a wireless network. See the `game.sh` shell script.

## Provided Materials:

The following files were provided before starting the lab:
* Solution Executables: `mazemake`, `mazesolve`, and `mazeshow`
* Driver code files `Mazemake.cpp` and `Mazesolve.cpp`
* A makefile
* A shell script that automates the maze creation and solving process for the solution executables: `game.sh`
