#include <algorithm>
#include <cstdio>
#include <cstdlib>
#include <utility>

using namespace std;

#include "Support.h"

dset::dset(int num_sets)
{
    S.assign(num_sets, node());
    Nsets = num_sets;
}

int dset::add_set()
{
    S.insert(S.end(), 1, node());
    Nsets += 1;
    return S.size()-1;
}

int dset::merge(int i, int j)
{
    i = this->find(i);
    j = this->find(j);
    if (i != j)
    {
        node &Si = S[i];
        node &Sj = S[j];
        if (Si.rank > Sj.rank)
        {
            Sj.parent = i;
        }
        else if (Si.rank < Sj.rank)
        {
            Si.parent = j;
        }
        else
        {
            Sj.parent = i;
            Si.rank += 1;
        }
        Nsets -= i;
    }
    else
    {
        return -1;
    }
    return this->find(i);
}

int dset::find(int i)
{
    if (S[i].parent == -1)
    {
        return i;
    }
    S[i].parent = this->find(S[i].parent);
    return S[i].parent;
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
    vector< pair<int, int> > interior;
    int ind, left, right, up, down;
    for (int i = 0; i < Nrows; i++)
    {
        for (int j = 0; i < Ncols; j++)
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
    for (int i = (int)(interior.size()) - 1; i > 0; --i)
    {
        std::swap(interior[i], interior[rand() % (i+1)]);
    }
    dset wallcontrol(size);
    vector< pair<int, int> >::iterator wall = interior.begin();
    int cell1, cell2, wallind1, wallind2;
    while (wallcontrol.size() != 0)
    {
        cell1 = wall->first;
        wallind1 = wall->second;
        switch(wallind1)
        {
            case 0: cell2 = cell1 - Ncols; wallind2 = 2;
            case 1: cell2 = cell1 - 1; wallind2 = 3;
            case 2: cell2 = cell1 + Ncols; wallind2 = 0;
            case 3: cell2 = cell1 + 1; wallind2 = 1;
            default: fprintf(stderr, "An internal error occured:\nInvalid wall number.\n"); exit(-1);
        }
        int mergenum = wallcontrol.merge(cell1, cell2);
        if (mergenum == -1)
        {
            ++wall;
            continue;
        }
        grid[cell1][wallind1] = false;
        grid[cell2][wallind2] = false;
        ++wall;
    }
}
