#include <vector>

class maze
{
    public:
        maze() { ; }
        ~maze();
        void create_maze(int, int);
        void write_maze();
    private:
        std::vector< std::vector<bool> > grid;
        int Nrows, Ncols;
};
