#include <algorithm>
#include <climits>
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <stack>
#include <utility>

using namespace std;

#include "Support.h"
#include "Dset.h"

/* Support.cpp
 * Author: Ian Lumsden
 * Date: April 2, 2018
 *
 * This file contains the implementation for the maze member
 * functions as listed in Support.h. This file is also where Dset is used.
 */

// A deconstructor for maze to prevent data corruption and seg faults.
maze::~maze()
{
    for (int i = 0; i < size; i++)
    {
        delete [] grid[i];
    }
    delete [] grid;
}

// This function initializes and creates the maze.
void maze::create_maze(int nr, int nc)
{
    // Sets the non-path members
    Nrows = nr;
    Ncols = nc;
    size = nr * nc;
    // Initializes the `grid` member variable and builds all possible walls in the maze
    grid = new bool*[size];
    for (int i = 0; i < size; i++)
    {
        grid[i] = new bool[4];
        for (int j = 0; j < 4; j++)
        {
            grid[i][j] = true;
        }
    }
    // Creates a vector of all the interior cell/wall pairs.
    vector< pair<int, int> > interior;
    int ind;
    for (int i = 0; i < Nrows; i++)
    {
        for (int j = 0; j < Ncols; j++)
        {
            ind = i*Ncols + j;
            if (i != 0)
            {
                interior.push_back(make_pair(ind, 0));
            }
            if (j != 0)
            {
                interior.push_back(make_pair(ind, 1));
            }
            if (i != Nrows - 1)
            {
                interior.push_back(make_pair(ind, 2));
            }
            if (j != Ncols - 1)
            {
                interior.push_back(make_pair(ind, 3));
            }
        }
    }
    // Randomly perterbates the list of interior walls to allow for random wall deletion.
    for (int i = (int)(interior.size()) - 1; i > 0; --i)
    {
        std::swap(interior[i], interior[rand() % (i+1)]);
    }
    // `wallcontrol` is a disjoint set that controls how many walls are removed
    dset wallcontrol(size);
    // Declares several variables to control wall deletion.
    vector< pair<int, int> >::iterator wall = interior.begin();
    int cell1, cell2, wallind1, wallind2;
    // The while loop ends when all cells belong to the same set in `wallcontrol`
    while (wallcontrol.size() != 1)
    {
        // Grabs the starting cell and the wall to be deleted from `interior`.
        cell1 = wall->first;
        wallind1 = wall->second;
        // Uses `walllind1` to determine the neighboring cell and its wall number.
        switch(wallind1)
        {
            case 0: cell2 = cell1 - Ncols; wallind2 = 2; break;
            case 1: cell2 = cell1 - 1; wallind2 = 3; break;
            case 2: cell2 = cell1 + Ncols; wallind2 = 0; break;
            case 3: cell2 = cell1 + 1; wallind2 = 1; break;
            default: fprintf(stderr, "An internal error occured:\nInvalid wall number.\n"); exit(-1);
        }
        // Tries to merge the sets of the two neighboring cells in the disjoint set
        int mergenum = wallcontrol.merge(cell1, cell2);
        // If the cells are already in the same set, the rest of this loop iteration is skipped.
        if (mergenum == -1)
        {
            ++wall;
            continue;
        }
        // The wall separating `cell1` and `cell2` is deleted.
        grid[cell1][wallind1] = false;
        grid[cell2][wallind2] = false;
        // Moves to the next wall as stated by `interior`
        ++wall;
    }
}

// This function writes each cell and the number representing its standing walls to stdout.
void maze::write_maze()
{
    fprintf(stdout, "MAZE %i %i\n", Nrows, Ncols);
    for (int i = 0; i < size; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            if (grid[i][j] == true)
            {
                fprintf(stdout, "%i %i\n", i, j);
            }
        }
    }
}

// This function reads a maze TXT file and creates a graph from it.
void maze::read_maze()
{
    // Reads in the header data.
    string label;
    int nr, nc;
    cin >> label >> nr >> nc;
    // Sets `Nrows`, `Ncols`, and `size` using the header data.
    Nrows = nr;
    Ncols = nc;
    size = nr * nc;
    // Initializes `grid` so that it has the correct size and no walls.
    grid = new bool*[size];
    for (int i = 0; i < size; i++)
    {
        grid[i] = new bool[4];
        for (int j = 0; j < 4; j++)
        {
            grid[i][j] = false;
        }
    }
    // Reads cell/wall pairs from the TXT file, and uses them to build the walls of the maze.
    int cell, wall;
    while (cin >> cell >> wall)
    {
        grid[cell][wall] = true;
    }
}

// A basic overloaded function to make it easy to solve the maze with no arguments.
bool maze::solve_maze()
{
    return solve_maze(0, size-1);
}

// A cell color variable to help with the Depth-First Search algorithm in `solve_maze`
typedef enum { WHITE, BLACK } vcolor_t;

// This function generates the shortest path from source/start to sink/stop.
bool maze::solve_maze(int start, int stop)
{
    // Sets the `source` and `sink` members.
    source = start;
    sink = stop;
    // Generates a vector of `vcolor_t` data to represent cell color.
    vector<vcolor_t> vcolor;
    vcolor.assign(size, WHITE);
    // Generates a vector of ints representing the distance from source to any cell.
    vector<int> vdist;
    vdist.assign(size, INT_MAX);
    vdist[source] = 0;
    // Assigns default values to the member variable, `vlink`
    vlink.assign(size, -1);
    vlink[source] = source;
    // Creates a stack to control the Depth-First Search and adds source to it
    stack<int> S;
    S.push(source);
    // `next` stores the index of the candidate for the next cell in the path.
    int next;
    while (!S.empty())
    {
        int i = S.top();
        S.pop();
        if (i == sink)
        {
            break;
        }
        if (vcolor[i] == BLACK)
        {
            continue;
        }
        vcolor[i] = BLACK;
        for (int j = 0; j < 4; j++)
        {
            switch(j)
            {
                case 0: next = i - Ncols; break;
                case 1: next = i - 1; break;
                case 2: next = i + Ncols; break;
                case 3: next = i + 1; break;
                default: fprintf(stderr, "An internal error occured:\nInvalid wall number.\n"); exit(-2);
            }
            if (next < 0 || next >= size)
            {
                continue;
            }
            if (grid[i][j] == false && vdist[next] == INT_MAX)
            {
                vdist[next] = vdist[i] + 1;
                vlink[next] = i;
                S.push(next);
            }
        } 
    }
    while (!S.empty())
    {
        S.pop();
    }
    if (vdist[sink] == INT_MAX)
    {
        fprintf(stderr, "There is no path between source (%i) and sink (%i).\n", source, sink);
        return false;
    }
    return true;
}

void maze::write_path()
{
    fprintf(stdout, "PATH %i %i\n", Nrows, Ncols);
    vector<int> reverse;
    for (int i = sink; i != source; i = vlink[i])
    {
        reverse.push_back(i);
    }
    reverse.push_back(source);
    for (int j = (int)(reverse.size())-1; j >= 0; --j)
    {
        fprintf(stdout, "%i\n", reverse[j]);
    }
}
