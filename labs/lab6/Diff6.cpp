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

using namespace std;

/* Diff6.cpp
 * Author: Ian Lumsden
 * Date: April 17, 2018
 *
 * This program implements a basic C++ implementation of the UNIX diff command.
 *
 * In other words, this program reads the contents of two TXT files. It then uses
 * dynamic programming to determine a set of change (insert+delete),
 * insert, and delete operations that could be used to convert the first
 * file into the second. Finally, it prints the operations to stdout.
 */

// An enum type for simplifying the syntax of the path generation
enum edit_t
{
    Delete,
    Insert,
    Match
};

// A templated matrix class for storing data (costs and links)
template <typename T>
class matrix 
{
    public:
        matrix(int nr=0, int nc=0);
        // Custom destructor to free up internal memory of the matrix
        ~matrix() { delete [] data; }
        void assign(int nr, int nc);
        // Returns the number of rows in the matrix
        int get_Nrows() const { return Nrows; }
        // Returns the number of columns in the matrix.
        int get_Ncols() const { return Ncols; }
        // Square-bracket operator overload that allows 2D array lookup
        T* operator[](int i) { return &data[i*Ncols]; }
    private:
        int Nrows, Ncols;
        T* data;
};

// Constructor for making a matrix of the desired size
template <typename T>
matrix<T>::matrix(int nr, int nc)
{
    Nrows = nr;
    Ncols = nc;
    data = new T[Nrows*Ncols];
}

// Essentially a wrapper function for the constructor
template <typename T>
void matrix<T>::assign(int nr, int nc)
{
    Nrows = nr;
    Ncols = nc;
    delete [] data;
    data = new T[Nrows*Ncols];
}

// Class for storing the file data and computing the diff-style output
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
        /* text1 stores the lines of the original TXT file.
         * text2 stores the lines of the modified TXT file.
         */
        vector<string> text1;
        vector<string> text2;
        // A matrix for storing the costs of the dynamic programming progression
        matrix<int> costs;
        /* `link` is a matrix for storing the link values.
         * The valid link values are:
         * 0 -> Default (only applies to [0][0])
         * 1 -> Insertion
         * 2 -> Deletion
         * 3 -> Match
         */
        matrix<int> link;
        // These variables are made constexpr to allow them to be used in a switch statement.
        static constexpr int DEF = 0;
        static constexpr int INS = 1;
        static constexpr int DEL = 2;
        static constexpr int MATCH = 3;
};

// Stores the contents of the original TXT file in the `text1` member variable.
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

// Stores the contents of the modified TXT file in the `text2` member variable.
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

// A simple helper function that returns 0 if the lines are equal and 1 otherwise
int LCS::op_cost(int ind1, int ind2)
{
    return text1[ind1-1] == text2[ind2-1] ? 0 : 1;
}

// Fills the costs and link matrices with the correct values based on the contents of the text files.
void LCS::compute_alignment()
{
    // Resizes the costs and link matrices to the desired size. 
    costs.assign(text1.size() + 1, text2.size() + 1);
    link.assign(text1.size() + 1, text2.size() + 1);
    for (int i = 0; i < costs.get_Nrows(); i++)
    {
        for (int j = 0; j < costs.get_Ncols(); j++)
        {
            /* If the current matrix cell is at (0,0), set the cost and link to
             * default values.
             */
            if (i == 0 && j == 0)
            {
                costs[0][0] = 0;
                link[0][0] = DEF;
            }
            /* If the current matrix cell is in the first column, set the cost to
             * the cost of the element above it plus 1, and set the link to DEL.
             */
            else if (i != 0 && j == 0)
            {
                costs[i][0] = costs[i-1][0] + 1;
                link[i][0] = DEL;
            }
            /* If the current matrix cell is in the first row, set the cost to
             * the cost of the element to the left plus 1, and set the link to INS.
             */
            else if (i == 0 && j != 0)
            {
                costs[0][j] = costs[0][j-1] + 1;
                link[0][j] = INS;
            }
            /* For any other cell, the lowest cost operation is determined, and the
             * cost and link are set accordingly.
             */
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

// A public wrapper function for determining the edit sequence and printing it to stdout
void LCS::report_difference()
{
    stack<edit_t> moves;
    int start1 = text1.size();
    int start2 = text2.size();
    report_difference(moves, start1, start2);
    print_difference(moves);
}

// A recursive function that determines the edit sequence and stores it in `moves`
void LCS::report_difference(stack<edit_t>& moves, int i, int j)
{
    switch (link[i][j])
    {
        case DEL: moves.push(Delete); report_difference(moves, i-1, j); break;
        case INS: moves.push(Insert); report_difference(moves, i, j-1); break;
        case MATCH: moves.push(Match); report_difference(moves, i-1, j-1); break;
        case DEF: return;
        // The code should never reach here. Used for error checking purposes.
        default: fprintf(stderr, "Internal Error: the link matrix contains an unknown value.\n"); exit(-3); 
    }
    return;
}

/* A private helper function for determining which output edits should be made.
 * This function converts the raw data passed into it into the data that is displayed in the
 * edit header that is printed to the console.
 * This function was added only to simplify the code in `print_difference`.
 */
void LCS::handle_match(vector< tuple< char, pair<int, int>, pair<int, int> > >& headlist, const vector<char>& str1, int start1, int start2, int end1, int end2, int ind1, int ind2)
{
    // `type` stores the operation id character.
    char type;
    // If the previous operation was a match, do nothing.
    if (start1 == -1 && start2 == -1)
    {
        ;
    }
    /* If there is only a single-line insert or delete (i.e. the first operation in the sample
     * output on Canvas), the operation is determined using `str1`, and it is added to `headlist`.
     */
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
    /* If you have multiple insert or delete operations, the number of insertions and deletions are
     * calculated and used to add the corrresponding operations to `headlist`.
     */
    else
    {
        /* `diff1` is the number of deletions.
         * `diff2` is the number of insertions.
         */
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

/* This function converts the output of the private `report_difference` function
 * into output that is printed to stdout.
 */ 
void LCS::print_difference(stack<edit_t>& moves)
{
    /* The output of the private `report_difference` function is converted
     * into two allignment "strings".
     */
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
    /* `ind1` and `ind2` are the current indexes for `text1` and `text2` respectively.
     * `start1` and `start2` are the indexes for the start of the current non-match operation
     *     sequences. They correspond with `text1` and `text2` respectively.
     * `end1` and `end2` are the equivalents of `start1` and `start2` for the end of the current
     *     non-match operation sequences.
     */
    int ind1 = 0;
    int ind2 = 0;
    int start1 = -1;
    int start2 = -1;
    int end1 = -1;
    int end2 = -1;
    // `headlist` is a vector of tuples for storing the header data.
    vector< tuple<char, pair<int, int>, pair<int, int> > > headlist;
    for (int i = 0; i < (int)(str1.size()); i++)
    {
        /* If the operation is an insertion, ind2 is incremented, and either start2
         * or end2 is set.
         */
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
        /* If the operation is an deletion, ind1 is incremented, and either start1
         * or end1 is set.
         */
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
        /* If the operation is a match, the `handle_match` function is called, and
         * the sequence variables (start1, start2, end1, end2) are reset.
         * ind1 and ind2 are also both incremented.
         */
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
    /* The `handle_match` function is called one last time to add on any non-match
     * operations that occur between the last match and the end of the comparison.
     */
    handle_match(headlist, str1, start1, start2, end1, end2, ind1, ind2);
    // type stores the operation id obtained from headlist.
    char type;
    /* For each operation in headlist, the data for the header is obtained
     * and used to print the corresponding data to stdout.
     */
    for (int i = 0; i < (int)(headlist.size()); i++)
    {
        type = get<0>(headlist[i]);
        start1 = get<1>(headlist[i]).first;
        end1 = get<1>(headlist[i]).second;
        start2 = get<2>(headlist[i]).first;
        end2 = get<2>(headlist[i]).second;
        // Addition/Insert Operation
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
        // Deletion operation
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
        // Change operation
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
    /* If there are not enought command-line arguments provided,
     * print an error message and exit.
     */
    if (argc < 3)
    {
        fprintf(stderr, "Usage: ./Diff6 original.txt modified.txt\n");
        exit(-5);
    }
    /* If either of the specified files are not TXT files,
     * print an error and exit.
     */
    string f1 = argv[1];
    string f2 = argv[2];
    if (f1.substr(f1.size() - 4, 4) != ".txt" || f2.substr(f2.size() -4, 4) != ".txt")
    {
        fprintf(stderr, "Usage: ./Diff6 original.txt modified.txt\n");
        exit(-6);
    }
    // Creates an LCS object and adds the content of the files to it.
    LCS lcs;
    lcs.text1_push_back(argv[1]);
    lcs.text2_push_back(argv[2]);
    // Computes the allignment between the two files and reports the difference to stdout.
    lcs.compute_alignment();
    lcs.report_difference();
}
