#include <vector>

/* Support.h
 * Author: Ian Lumsden
 * Date: April 2, 2018
 *
 * This file contains the declaration of the maze
 * class used in Support.cpp, Mazemake.cpp, and
 * Mazesolve.cpp.
 */

// The maze class to store the data for the maze and path.
class maze
{
    public:
        maze() { ; }
        ~maze();
        void create_maze(int, int);
        void write_maze();
        void read_maze();
        bool solve_maze();
        bool solve_maze(int, int);
        void write_path();
    private:
        // `grid` is a bool array that is used to store the maze data.
        bool **grid;
        /* `vlink` is a vector of ints that stores the shortest path as calculated
         * by the two solve_maze() functions.
         */
        std::vector<int> vlink;
        // `Nrows` and `Ncols` are the number of rows and columns respectively in the maze.
        int Nrows, Ncols;
        // `size` the number of rows in `grid`. In other words, the total number of maze cells.
        int size;
        // `source` and `sink` are the starting and ending points respectively of the path.
        int source, sink;
};
