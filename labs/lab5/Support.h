#include <vector>

class maze
{
    struct pathnode
    {
        pathnode() { mazeind = 0; parent = 0; }
        pathnode(int i, int j) { mazeind = i; parent = j; }
        int mazeind, parent;
    };
    public:
        maze() { ; }
        ~maze();
        void create_maze(int, int);
        void write_maze();
        void read_maze();
        bool solve_maze();
        bool solve_maze(int, int);
        void write_path();
    private:
        bool **grid;
        std::vector<pathnode> path;
        int Nrows, Ncols;
        int size;
};
