
#include <cstdlib>
#include <iostream>
using namespace std;

#include "Support.h"

/* Mazesolve.cpp
 *
 * This file is the driver for solving a maze provided through
 * stdin. It was provided, and no meaningful changes were made to it.
 */

int main(int argc, char *argv[]) {
  if (argc != 1 && argc != 3) {
    cerr << "usage: " << argv[0]
		 << " [source sink]\n";
	return 0;
  }

  maze M;

  M.read_maze();

  if (argc == 1) {
    if (M.solve_maze())
      M.write_path();
  } else
  if (argc == 3) {
    if (M.solve_maze(atoi(argv[1]), atoi(argv[2])))
      M.write_path();
  }
}
