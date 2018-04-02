using namespace std;

#include "Dset.h"

/* Dset.cpp
 * Author: Ian Lumsden
 * Date: April 2, 2018
 *
 * This file contains the implementation for the dset member
 * functions as listed in Dset.h.
 */

// The constructor of dset. Makes `S` be size `num_sets`, and uses `num_sets` to set Nsets
dset::dset(int num_sets)
{
    S.assign(num_sets, node());
    Nsets = num_sets;
}

// This function merges the sets specified by `i` and `j`.
int dset::merge(int i, int j)
{
    /* If `i` and/or `j` are part of a set with multiple elements, these calls
     * determine the index that represents the set to which they belong. If they
     * are the only elements in their respective sets, they are their own representatives.
     * So, they will not change.
     */
    i = this->find(i);
    j = this->find(j);
    /* If the parameters do not belong to the same set, the parent of either `i` or `j`
     * will be set to the other, linking them together. Rank comparison is used to determine
     * which will be the child and which will be the parent. If they are of equal rank,
     * `i` will always be the parent, and its rank will be incremented. No matter how the
     * linkage happens, the number of sets will always be reduced by 1.
     */
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
        Nsets -= 1;
    }
    // If the parameters belong to the same set, -1 is returned.
    else
    {
        return -1;
    }
    // The representative of the new set containing `i` and `j` is returned.
    return this->find(i);
}

// This function returns the representative of the set containing `i`.
int dset::find(int i)
{
    // If `i` does not have a parent, it is the representative and is returned.
    if (S[i].parent == -1)
    {
        return i;
    }
    /* If `i` is not the representative, the function is recursively called to set
     * the parent of `i` to the representative of its set. 
     */
    S[i].parent = this->find(S[i].parent);
    // The representative of the set containing `i` is returned.
    return S[i].parent;
}
