#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <stack>
#include <string>
#include <utility>
#include <vector>

using namespace std;

template <typename T>
class matrix 
{
    public:
        matrix(int nr=0, int nc=0);
        ~matrix() { delete [] data; }
        void assign(int nr, int nc);
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

void matrix::assign(int nr, int nc)
{
    matrix(nr, nc);
}

class LCS 
{
    public:
        LCS();
        //~LCS();
        void text1_push_back(string fname);
        void text2_push_back(string fname);
        void compute_alignment();
	void report_difference();
    private:
	// support functions
        void report_difference(int i, int j);
        int op_cost(int, int);
        vector<string> text1;
        vector<string> text2;
        matrix<int> costs;
        /* 0 -> Default (only applies to [0][0])
         * 1 -> Insertion
         * 2 -> Deletion
         * 4 -> Match
         */
        matrix<int> link;
        const int DEF, INS, DEL, MATCH;
};

LCS::LCS()
    : DEF(0)
    , INS(1)
    , DEL(2)
    , MATCH(4)
{}

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
    costs.(text1.size() + 1, text2.size() + 1);
    link.(text1.size() + 1, text2.size() + 1);
    costs[0][0] = 0;
    int addcost;
    for (int i = 0; i < (int)(text1.size()) + 1; i++)
    {
        for (int j = 0; j < (int)(text2.size()) + 1; j++)
        {
            if (i == 0 && j == 0)
            {
                link[0][0] = DEF;
            }
            else if (i != 0 && j == 0)
            {
                costs[i][0] = costs[i-1][0] + 1;
                link[i][0] = DEL;
            }
            else if (i == 0 && j != 0)
            {
                costs[0][j] = costs[0][j-1] + 1;
                link[0][j] = INS;
            }
            else
            {
                int add_cost = op_cost(i, j);
                if (add_cost == 0)
                {
                    cost[i][j] = cost[i-1][j-1];
                    link[i][j] = MATCH;
                    continue;
                }
                int delcost = cost[i-1][j] + add_cost;
                int inscost = cost[i][j-1] + add_cost;
                if (delcost <= inscost)
                {
                    cost[i][j] = delcost;
                    link[i][j] = DEL;
                }
                else
                {
                    cost[i][j] = inscost;
                    link[i][j] = INS; 
                }
            }
        }
    }
}

void LCS::report_difference()
{
    stack<string> allign1, allign2;
    int start1 = text1.size();
    int start2 = text2.size();
    report_difference(allign1, allign2, start1, start2);
}

void LCS::report_difference(stack<string>$ allign1, stack<string>& allign2, int i, int j)
{

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
