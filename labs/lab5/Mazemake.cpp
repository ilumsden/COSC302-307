#include <cstdlib>
#include <ctime>
#include <iostream>

using namespace std;

#include "Support.h"

/* Mazemake.cpp
 *
 * This file is the driver for making a random maze given the
 * number of rows and columns. It was provided, and no meaningful
 * changes were made to it.
 */

int main(int argc, char *argv[]) 
{
    srand(time(NULL));

    if (argc != 3) {
        cerr << "usage: " << argv[0]
             << " nrows ncols\n";
	    return 0;
    }

    maze M;

    int Nrows = atoi(argv[1]);
    int Ncols = atoi(argv[2]);

    M.create_maze(Nrows, Ncols);
    M.write_maze();
}
