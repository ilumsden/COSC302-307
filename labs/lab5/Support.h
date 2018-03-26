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
