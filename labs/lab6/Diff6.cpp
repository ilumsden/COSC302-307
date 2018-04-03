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
        /* 0 -> Initial value (0, 0)
         * 1 -> Deletion
         * 2 -> Insertion
         * 4 -> No op
         */
        matrix<int> link;
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
    link[0][0] = 0;
    int addcost;
    for (int i = 0; i < (int)(text1.size()) + 1; i++)
    {
        for (int j = 0; j < (int)(text2.size()) + 1; j++)
        {
            if (i == 0 && j == 0)
            {
                continue;
            }
            else if (i != 0 && j == 0)
            {
                costs[i][0] = costs[i-1][0] + 1;
                link[i][0] = 1;
            }
            else if (i == 0 && j != 0)
            {
                costs[0][j] = costs[0][j-1] + 1;
                link[0][j] = 2;
            }
            else
            {
                
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
