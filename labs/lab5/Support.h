#include <vector>

class maze
{
    public:
        maze() { ; }
        ~maze();
        void create_maze(int, int);
        void write_maze();
    private:
        bool **grid;
        int Nrows, Ncols;
        int size;
};

class dset
{
    struct node
    {
        node() { rank = 0; parent = -1; }
        int rank;
        int parent;
    };
    public:
        dset(int num_sets);
        int size() const { return Nsets; }
        int add_set();
        int merge(int, int);
        int find(int);
    private:
        int Nsets;
        std::vector<node> S;
};
