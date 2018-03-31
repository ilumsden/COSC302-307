#include <vector>

class dset
{
    struct node
    {
        node() { rank = 0; parent = -1; }
        int rank, parent;
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
