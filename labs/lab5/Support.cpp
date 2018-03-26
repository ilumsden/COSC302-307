#include <algorithm>
#include <cstdio>
#include <utility>

using namespace std;

#include "Support.h"

dset::dset(int num_sets)
{
    S.assign(N, node());
    Nsets = N;
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
    return this->find(i);
}

int dset::find(int i)
{
    if (S[i].parent == -1)
    {
        return i;
    }
    S[i].parent = find(S[i].parent);
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
}
