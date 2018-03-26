#include <list>
#include <vector>

class pairs
{
    public:
        pairs(int);
        ~pairs() { delete [] pairlist; }
        void set_pair(int, int, int, int);
        bool get_pair(int, int, int, int);
    private:
        bool *pairlist;
        int Ncols;
        int * operator[](int i) const { return &pairlist[(i*(i+1))/2]; }
};

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

class set_t
{
    public:
        void create_set(pair<int, int>);
        void add_to_set(pair<int, int>);
        pair<int, int> * get_rep(pair<int, int>);
    private:
        list< pair<int, int> > data;
        pair<int, int> *rep;
};

class disjoint_set
{
    public:
        void create_set(int, int);
        pair<int, int> * find_set(int, int);
        void merge_set(int, int, int, int);
    private:
        vector<set_t> dis;
};
