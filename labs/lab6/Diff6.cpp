#include <algorithm>
#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <string>
#include <utility>

using namespace std;

template <typename T>
class matrix 
{
    public:
        matrix(int nr=0, int nc=0);
        ~matrix() { delete [] data; }
        int get_Nrows() const { return Nrows; }
        int get_Ncols() const { return Ncols; }
        T* operator[](int i) { return &data[i*Ncols]; }
    private:
        int Nrows, Ncols;
        T* data;
};

matrix::matrix(int nr, int nc)
{
    Nrows = nr;
    Ncols = nc;
    data = new T[Nrows*Ncols];
}

class LCS 
{
    public:
        //~LCS();
        void text1_push_back(string fname);
        void text2_push_back(string fname);
        void compute_alignment();
	//void report_difference();
    private:
	// support functions
        int op_cost(int, int);
        vector<string> text1;
        vector<string> text2;
        matrix<int> costs;
        /* link[i][j][0] -> Diagonal Link (Eq)
         * link[i][j][1] -> Horizontal Link (Insert)
         * link[i][j][2] -> Vertical Link (Deletion)
         */
        matrix<bool[3]> link;
};

/*LCS::~LCS()
{
    delete costs;
    delete link;
}*/

void LCS::text1_push_back(string fname)
{
    fstream fin;
    fin.open(fname.c_str(), ios::in);
    if (!fin.is_open())
    {
        fprintf(stdout, "%s could not be opened.\n", fname.c_str());
        exit(-1);
    }
    string line;
    while (getline(fin, line))
    {
        text1.push_back(line);
    }
}

void LCS::text2_push_back(string fname)
{
    fstream fin;
    fin.open(fname.c_str(), ios::in);
    if (!fin.is_open())
    {
        fprintf(stdout, "%s could not be opened.\n", fname.c_str());
        exit(-1);
    }
    string line;
    while (getline(fin, line))
    {
        text2.push_back(line);
    }
}

int LCS::op_cost(int ind1, int ind2)
{
    return text1[ind1] == text2[ind2] ? 0 : 1;
}

void LCS::compute_alignment()
{
    costs = matrix(text1.size() + 1, text2.size() + 1);
    link = matrix(text1.size() + 1, text2.size() + 1);
    costs[0][0] = 0;
    int addcost;
    for (int i = 0; i < (int)(text1.size()) + 1; i++)
    {
        for (int j = 0; j < (int)(text2.size()) + 1; j++)
        {
            if (i == 0 && j == 0)
            {
                link[0][0][0] = false;
                link[0][0][1] = false;
                link[0][0][2] = false;
            }
            else if (i != 0 && j == 0)
            {
                costs[i][0] = costs[i-1][0] + 1;
                link[i][0][0] = false;
                link[i][0][1] = false;
                link[i][0][2] = true;
            }
            else if (i == 0 && j != 0)
            {
                costs[0][j] = costs[0][j-1] + 1;
                link[0][j][0] = false;
                link[0][j][1] = true;
                link[0][j][2] = false;
            }
            else
            {
                int to_sort[3];
                int pre_sort[3];
                pre_sort[0] = INT_MAX;
                int init_cost = op_cost(i, j);
                if (init_cost == 0)
                {
                    pre_sort[0] = costs[i-1][j-1];
                }
                pre_sort[1] = costs[i][j-1] + init_cost;
                pre_sort[2] = costs[i-1][j] + init_cost;
                for (int i = 0; i < 3; i++)
                {
                    to_sort[i] = pre_sort[i];
                }
                if (to_sort[1] < to_sort[0]) { swap(to_sort[0], to_sort[1]); }
                if (to_sort[2] < to_sort[0]) { swap(to_sort[0], to_sort[2]); }
                if (to_sort[2] < to_sort[1]) { swap(to_sort[1], to_sort[2]); }
                costs[i][j] = to_sort[0];
                
            }
        }
    }
}

int main(int argc, char **argv)
{
    // check two input files are specified on command line

    LCS lcs;  // instantiate your "lcs based diff" object

    // read the text from file1 into the lcs.text1 buffer
    // read the text from file2 into the lcs.text2 buffer

    lcs.compute_alignment();
    lcs.report_difference();
}
