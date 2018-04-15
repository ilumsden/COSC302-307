#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <stack>
#include <string>
#include <utility>
#include <vector>

using namespace std;

enum edit_t
{
    Delete,
    Insert,
    Match,
    Sub
};

enum
{
    DEF = 0,
    INS = 1,
    DEL = 2,
    MATCH = 3
};

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

template <typename T>
matrix<T>::matrix(int nr, int nc)
{
    Nrows = nr;
    Ncols = nc;
    data = new T[Nrows*Ncols];
}

template <typename T>
void matrix<T>::assign(int nr, int nc)
{
    Nrows = nr;
    Ncols = nc;
    delete [] data;
    data = new T[Nrows*Ncols];
}

class LCS 
{
    public:
        //LCS();
        //~LCS();
        void text1_push_back(string fname);
        void text2_push_back(string fname);
        void compute_alignment();
	void report_difference();
    private:
	// support functions
        void report_difference(stack<edit_t>&, int i, int j);
        void print_difference(stack<edit_t>&);
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
        //const int DEF, INS, DEL, MATCH;
};

/*LCS::LCS()
    : DEF(0)
    , INS(1)
    , DEL(2)
    , MATCH(4)
{}*/

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
        exit(-2);
    }
    string line;
    while (getline(fin, line))
    {
        text2.push_back(line);
    }
}

int LCS::op_cost(int ind1, int ind2)
{
    return text1[ind1-1] == text2[ind2-1] ? 0 : 1;
}

void LCS::compute_alignment()
{
    costs.assign(text1.size() + 1, text2.size() + 1);
    link.assign(text1.size() + 1, text2.size() + 1);
    costs[0][0] = 0;
    for (int i = 0; i < costs.get_Nrows(); i++)
    {
        for (int j = 0; j < costs.get_Ncols(); j++)
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
                    costs[i][j] = costs[i-1][j-1];
                    link[i][j] = MATCH;
                    continue;
                }
                int delcost = costs[i-1][j] + add_cost;
                int inscost = costs[i][j-1] + add_cost;
                if (delcost <= inscost)
                {
                    costs[i][j] = delcost;
                    link[i][j] = DEL;
                }
                else
                {
                    costs[i][j] = inscost;
                    link[i][j] = INS; 
                }
            }
        }
    }
}

void LCS::report_difference()
{
    stack<edit_t> moves;
    int start1 = text1.size();
    int start2 = text2.size();
    report_difference(moves, start1, start2);
    print_difference(moves);
}

void LCS::report_difference(stack<edit_t>& moves, int i, int j)
{
    switch (link[i][j])
    {
        case DEL: moves.push(Delete); report_difference(moves, i-1, j); break;
        case INS: moves.push(Insert); report_difference(moves, i, j-1); break;
        case MATCH: moves.push(Match); report_difference(moves, i-1, j-1); break;
        case DEF: return;
        default: fprintf(stderr, "Internal Error: the link matrix contains an unknown value.\n"); exit(-3); 
    }
    return;
}

void LCS::print_difference(stack<edit_t>& moves)
{
    vector< pair<edit_t, int> > groups;
    edit_t sing_move;
    int num_ins = 0;
    int num_del = 0;
    int num_match = 0;
    int num_sub;
    sing_move = moves.top();
    moves.pop();
    while (!moves.empty())
    {
        while (sing_move != Match)
        {
            if (sing_move == Insert)
            {
                num_ins++;
            }
            else
            {
                num_del++;
            }
            if (moves.empty())
            {
                break;
            }
            sing_move = moves.top();
            moves.pop();
        }
        while (sing_move == Match)
        {
            num_match++;
            if (moves.empty())
            {
                break;
            }
            sing_move = moves.top();
            moves.pop();
        }
        num_sub = num_ins - num_del;
        if (num_sub < 0)
        {
            num_del = abs(num_sub);
            num_sub = num_ins;
            if (num_sub != 0)
            {
                groups.push_back(make_pair(Sub, num_sub));
            }
            groups.push_back(make_pair(Delete, num_del));
            groups.push_back(make_pair(Match, num_match));
        }
        else if (num_sub > 0)
        {
            num_ins = abs(num_sub);
            num_sub = num_del;
            if (num_sub != 0)
            {
                groups.push_back(make_pair(Sub, num_sub));
            }
            groups.push_back(make_pair(Insert, num_del));
            groups.push_back(make_pair(Match, num_match));
        }
        else
        {
            num_sub = num_ins;
            if (num_sub != 0)
            {
                groups.push_back(make_pair(Sub, num_sub));
            }
            groups.push_back(make_pair(Match, num_match));
        }
    }
    for (int i = 0; i < (int)(groups.size()); i++)
    {
        for (int j = 0; j < groups[i].second; j++)
        {
            switch (groups[i].first)
            {
                case Insert: printf("Insert\n"); break;
                case Delete: printf("Delete\n"); break;
                case Sub: printf("Sub\n"); break;
                case Match: printf("Match\n"); break;
                default: printf("ERROR\n");
            }
        }
    }
}

int main(int argc, char **argv)
{
    // check two input files are specified on command line

    LCS lcs;  // instantiate your "lcs based diff" object

    lcs.text1_push_back(argv[1]);
    lcs.text2_push_back(argv[2]);

    lcs.compute_alignment();
    lcs.report_difference();
}
