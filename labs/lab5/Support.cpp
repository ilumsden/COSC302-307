#include <cstdio>
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

void set_t::create_set(pair<int, int> cell)
{
    if (!data.empty())
    {
        return;
    }
    data.push_front(cell);
}

void set_t::add_to_set(pair<int, int> cell)
{
    data.push_front(cell);
}

pair<int, int> * set_t::get_rep(pair<int, int> cell)
{
    list< pair<int, int> >::iterator i = data.begin();
    while (i != data.end())
    {
        if (*i == cell)
        {
            return rep;
        }
    }
    return NULL;
}

set_t * disjoint_set::find_set(int i, int j)
{
    pair<int, int> *rep;
    for (int i = 0; i < (int)(dis.size()); i++)
    {
        rep = dis[i].get_rep(make_pair(i, j));
        if (rep != NULL)
        {
            for (int j = 0; j < (int)(dis.size()); j++)
            {
                if (rep == &dis[j])
                {
                    return &dis[j];
                }
            }
        }
    }
    return NULL;
}

void disjoint_set::create_set(int i, int j)
{
    if (find_set(i, j) != NULL)
    {
        return;
    }
    set_t s;
    s.create_set(make_pair(i, j));
    dis.push_back(s);
}

void disjoint_set::merge_set(int i1, int j1, int i2, int j2)
{
    set_t *rep1 = find_set(i1, j1);
    set_t *rep2 = find_set(i2, j2);
    
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
    pairs pairlist(size, Ncols);
    for (int i = 1; i < Nrows - 1; i++)
    {
        for (int j = 1; i < Ncols - 1; j++)
        {
            pairlist.set_pair(i, j, i-1, j);
            pairlist.set_pair(i, j, i, j-1);
            pairlist.set_pair(i, j, i+1, j);
            pairlist.set_pair(i, j, i, j+1);
        }
    }
}
