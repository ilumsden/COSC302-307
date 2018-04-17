#include <climits>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <stack>
#include <string>
#include <tuple>
#include <utility>
#include <vector>

#include <iostream>
#include <iomanip>

using namespace std;

enum edit_t
{
    Delete,
    Insert,
    Match
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
        void text1_push_back(string fname);
        void text2_push_back(string fname);
        void compute_alignment();
	void report_difference();
    private:
	// support functions
        void report_difference(stack<edit_t>&, int i, int j);
        void handle_match(vector< tuple< char, pair<int, int>, pair<int, int> > >&, const vector<char>&, int, int, int, int, int, int);
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
};

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
                costs[i][j] = INT_MAX;
                int add_cost = op_cost(i, j);
                if (add_cost == 0)
                {
                    costs[i][j] = costs[i-1][j-1];
                    link[i][j] = MATCH;
                }
                int delcost = costs[i-1][j] + 1;
                int inscost = costs[i][j-1] + 1;
                if (inscost < costs[i][j])
                {
                    costs[i][j] = inscost;
                    link[i][j] = INS;
                }
                if (delcost < costs[i][j])
                {
                    costs[i][j] = delcost;
                    link[i][j] = DEL; 
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

void LCS::handle_match(vector< tuple< char, pair<int, int>, pair<int, int> > >& headlist, const vector<char>& str1, int start1, int start2, int end1, int end2, int ind1, int ind2)
{
    char type;
    if (start1 == -1 && start2 == -1)
    {
        ;
    }
    else if (end1 == -1 && end2 == -1)
    {
        switch (str1[ind1])
        {
            case '-': type = 'a'; break;
            case 'D': type = 'd'; break;
            case 'M': break;
            default: fprintf(stderr, "Internal Error: Invalid allignment type.\n"); exit(-4);
        }
        if (start1 == -1)
        {
            start1 = ind1;
        }
        if (start2 == -1)
        {
            start2 = ind2;
        }
        headlist.push_back(make_tuple(type, make_pair(start1, -1), make_pair(start2, -1)));
    }
    else
    {
        int diff1, diff2;
        if (start1 != -1 && start2 != -1) 
        {
            diff1 = abs(end1 - start1);
            diff2 = abs(end2 - start2);
        }
        else if (start1 == -1 && start2 != -1)
        {
            diff1 = 0;
            diff2 = abs(end2 - start2);
        }
        else
        {
            diff1 = abs(end1 - start1);
            diff2 = 0;
        }
        if (diff1 == diff2)
        {
            headlist.push_back(make_tuple('c', make_pair(start1, end1), make_pair(start2, end2)));
        }
        else if (diff1 < diff2)
        {
            if (diff1 != 0)
            {
                headlist.push_back(make_tuple('c', make_pair(start1, end1), make_pair(start2, start2+diff1)));
                headlist.push_back(make_tuple('a', make_pair(end1, -1), make_pair(start2+diff1+1, end2)));
            }
            else
            {
                headlist.push_back(make_tuple('a', make_pair(ind1, -1), make_pair(start2, end2))); 
            }
        }
        else
        {
            if (diff2 != 0)
            {
                headlist.push_back(make_tuple('c', make_pair(start1, start1+diff2), make_pair(start2, end2)));
                headlist.push_back(make_tuple('d', make_pair(start1+diff1+1, end1), make_pair(end2, -1)));
            }
            else
            {
                headlist.push_back(make_tuple('d', make_pair(start1, end1), make_pair(ind2, -1)));
            }
        }
    }
}

void LCS::print_difference(stack<edit_t>& moves)
{
    vector<char> str1, str2;
    edit_t op;
    while (!moves.empty())
    {
        op = moves.top();
        moves.pop();
        if (op == Insert)
        {
            str1.push_back('-');
            str2.push_back('I');
        }
        else if (op == Delete)
        {
            str1.push_back('D');
            str2.push_back('-');
        }
        else
        {
            str1.push_back('M');
            str2.push_back('M');
        }
    }
    int ind1 = 0;
    int ind2 = 0;
    int start1 = -1;
    int start2 = -1;
    int end1 = -1;
    int end2 = -1;
    vector< tuple<char, pair<int, int>, pair<int, int> > > headlist;
    for (int i = 0; i < (int)(str1.size()); i++)
    {
        if (str1[i] == '-' && str2[i] == 'I')
        {
            ++ind2;
            if (start2 == -1)
            {
                start2 = ind2;
            }
            else
            {
                end2 = ind2;
            }
        }
        else if (str1[i] == 'D' && str2[i] == '-')
        {
            ++ind1;
            if (start1 == -1)
            {
                start1 = ind1;
            }
            else
            {
                end1 = ind1;
            }
        }
        else
        {
            handle_match(headlist, str1, start1, start2, end1, end2, ind1, ind2);
            start1 = -1;
            end1 = -1;
            start2 = -1;
            end2 = -1;
            ++ind1;
            ++ind2;
        }
    }
    handle_match(headlist, str1, start1, start2, end1, end2, ind1, ind2);
    char type;
    for (int i = 0; i < (int)(headlist.size()); i++)
    {
        type = get<0>(headlist[i]);
        start1 = get<1>(headlist[i]).first;
        end1 = get<1>(headlist[i]).second;
        start2 = get<2>(headlist[i]).first;
        end2 = get<2>(headlist[i]).second;
        if (type == 'a')
        {
            if (end2 == -1)
            {
                printf("%ia%i\n", start1, start2);
                end2 = start2;
            }
            else
            {
                printf("%ia%i,%i\n", start1, start2, end2);
            }
            for (int i = start2 - 1; i < end2; i++)
            {
                printf("> %s\n", text2[i].c_str());
            }
        }
        else if (type == 'd')
        {
            if (end1 == -1)
            {
                printf("%id%i\n", start1, start2);
                end1 = start1;
            }
            else
            {
                printf("%i,%id%i\n", start1, end1, start2);
            }
            for (int i = start1 - 1; i < end1; i++)
            {
                printf("< %s\n", text1[i].c_str());
            }
        }
        else
        {
            printf("%i,%ic%i,%i\n", start1, end1, start2, end2); 
            for (int i = start1 - 1; i < end1; i++)
            {
                printf("< %s\n", text1[i].c_str());
            }
            printf("---\n");
            for (int j = start2 - 1; j < end2; j++)
            {
                printf("> %s\n", text2[j].c_str());
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
