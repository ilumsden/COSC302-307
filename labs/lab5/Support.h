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
