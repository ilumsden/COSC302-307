#include <vector>

/* Dset.h
 * Author: Ian Lumsden
 * Date: April 2, 2018
 *
 * This file contains the declaration of the dset class
 * used in Support.cpp to control the random removal of
 * walls.
 */

// The dset class to montinor the linking of maze cells
class dset
{
    // `node` is the element of `dset`. It allows elements to be linked through `parent`.
    struct node
    {
        node() { rank = 0; parent = -1; }
        int rank, parent;
    };
    public:
        dset(int num_sets);
        // An accessor function that returns the number of sets in the `dset` object.
        int size() const { return Nsets; }
        int merge(int, int);
        int find(int);
    private:
        // `Nsets` stores the number of sets in the object.
        int Nsets;
        // A vector of nodes that stores the data of the object.
        std::vector<node> S;
};
