#include <vector>

class maze
{
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
        std::vector<int> vlink;
        int Nrows, Ncols;
        int size;
        int source, sink;
};
