#include <cstdio>
#include <vector>
#include <utility>

using namespace std;

#include "Support.h"

pairs::pairs(int maze_size, int nc)
{
    size = maze_size;
    Ncols = nc;
    pairlist = new int[maze_size];
    for (int i = 0; i < size; i++)
    {   
        pairlist[i] = false;
    }
}
void pairs::set_pair(int i1, int j1, int i2, int j2)
{
    int ind1 = i1*Ncols + j1;
    int ind2 = i2*Ncols + j2;
    (*this)[ind1][ind2] = true;
}
bool pairs::get_pair(int i1, int j1, int i2, int j2)
{
    int ind1 = i1*Ncols + j1;
    int ind2 = i2*Ncols + j2;
    return (*this)[ind1][ind2];
}
             
maze::~maze()
{
    for (int i = 0; i < size; i++)
    {
        delete [] grid[i];
    }
    delete [] grid;
}

void maze::create_maze(int nr, int nc)
{
    Nrows = nr;
    Ncols = nc;
    size = nr * nc;
    grid = new bool[size][4];
    for (int i = 0; i < size; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            grid[i][j] = true;
        }
    }
    for (int i = 1; i < Nrows - 1; i++)
    {
        for (int j = 1; i < Ncols - 1; j++)
        {
        }
    }
}
