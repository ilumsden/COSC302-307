using namespace std;

#include "Dset.h"

dset::dset(int num_sets)
{
    S.assign(num_sets, node());
    Nsets = num_sets;
}

int dset::add_set()
{
    S.insert(S.end(), 1, node());
    Nsets += 1;
    return S.size() - 1;
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
