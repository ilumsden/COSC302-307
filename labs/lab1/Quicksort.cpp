#include <iostream>
#include <sstream>
#include <string>
#include <iomanip>
#include <vector>
#include <cstdlib>
#include <algorithm>
#include <ctime>

using namespace std;

/* Quicksort.cpp
 * Author: Ian Lumsden
 * Date: January 25, 2018
 *
 * The code in this file reads text from the console (the text is
 * actually stored in files, but it is read in through the console).
 * It then stores the personal data from the console in a vector of
 * `person_t` objects. This vector is then sorted using an overloaded
 * less-than operator for the person_t class and a custom quicksort algorithm
 * that randomly selects a pivot. Finally, this code prints the sorted
 * contents of the list with specific formatting.
 */

int width = 0;

typedef stringstream sstream;

/* This function applies the same general algorithm as quicksort
 * to ensure that the element at index "k" is at its sorted position.
 * This function will only consider the elements from indexes lowerbound
 * to upperbound, inclusive.
 */
template <typename Tdata>
void quickselect(vector<Tdata> &data, int lowerbound, int k, int upperbound) {
    /* This while loop ensures that the algorithm is repeated
     * until the element at index "k" is at its sorted position.
     * It is used in favor of recursion.
     */
    while (1) {        
        /* If the size of the data considered is less than or
         * equal to 0, the function will simply return. This
         * should (hopefully) never happen, but it is included
         * as a precaution.
         */
        if (upperbound <= lowerbound) {
            return;
        }
        /* This pivot is determined. Then, the value stored at
         * pindex is saved for later use, and the element at
         * pindex is swapped with the element at upperbound.
         */
        int size = (upperbound - lowerbound) + 1;
        int pindex = rand() % size + lowerbound;
        Tdata pivot = data[pindex];
        swap(data[pindex], data[upperbound]);
        /* `i` and `j` are used as the bounds for the
         * quicksort-style partitioning performed in this
         * interior while loop.
         */
        int i = lowerbound - 1;
        int j = upperbound;
        /* This loop carries out a quicksort-style partitioning
         * of the data between i and j. More specifically, it will
         * increase i until the element at i is greater than or
         * equal to the pivot. Similarly, it will decrease j until
         * it is less than or equal to the pivot. Then, if i >= j,
         * the loop will break. Otherwise, the element at i will be
         * swapped with the element at j.
         */
        while (1) {
            while (data[++i] < pivot) { }
            /* The `j-1 >= 0` statement is used to ensure the access
             * operation in the second half of the condition does not
             * cause an error for accessing data outside of data.
             */
            while (j-1 >= 0 && pivot < data[--j]) { }
	    if (i>=j) {
                break;
	    }
	    swap(data[i], data[j]);
        }
        /* Once partitioning is finished, the pivot is moved to the final
	     * index that i reached.
         */
        pindex = i;
        swap(data[pindex], data[upperbound]);
        /* If the pivot's index is k, the element is in its
         * sorted location, and the function is exited.
         * Otherwise, if pindex > k, this algorithm will be
         * repeated on the elements with indicies less than pindex.
         * If pindex < k, this algorithm will be repeated on the
         * elements with indices greater than pindex.
         */
        if (pindex == k) {
            return;
        }
        if (k < pindex) {
            upperbound = pindex - 1;
        }
        else {
            lowerbound = pindex + 1;
        }
    }
}

/* This function implements a quicksort algorithm with
 * random pivot selection to sort a vector of data.
 */
template <typename Tdata>
void quicksort(vector<Tdata> &data, int lowerbound, int upperbound) { 
    /* If the current partition contains one or fewer
     * elements, this branch of recursion is ended
     * without doing anything.
     */
    if (upperbound <= lowerbound) {
        return;
    }
    /* `size` is the size of the current partition
     * and is used to control the random selection
     * of the pivot. Once the pivot is selected,
     * its data is stored in a variable for future use,
     * and it is swapped with the element at `upperbound`.
     */
    int size = (upperbound - lowerbound) + 1;
    int pindex = rand() % size + lowerbound;
    Tdata pivot = data[pindex];
    swap(data[pindex], data[upperbound]);
    /* `i` and `j` are used to perform the next set of partitioning.
     * `i` is set to loweroubnd-1 to force the "right" progression to start
     * at the element at lowerbound. `j` is set to upperbound to force
     * the "left" progression to start at the element at upperbound-1.
     */
    int i = lowerbound - 1;
    int j = upperbound;
    /* This loop progresses `i` and `j` until the element at `i` is greater
     * than the pivot and the element at `j` is less than the pivot.
     * Then, if `i` is less than `j`, the elements at `i` and `j` are swapped.
     * Otherwise, the loop is exited.
     */
    while (1) {
        while (data[++i] < pivot) { }
	while (j-1 >= 0 && pivot < data[--j]) { }
	if (i>=j) {
            break;
	}
	swap(data[i], data[j]);
    }
    /* The pivot is moved into its sorted position. Then, this algorithm is
     * applied again to the left and right partitions.
     */
    pindex = i;
    swap(data[pindex], data[upperbound]);
    quicksort(data, lowerbound, pindex-1);
    quicksort(data, pindex+1, upperbound);
    return;
}

// The class to store the data read from the console.
class person_t {
    public:
        // This is a default constructor for the class.
        person_t() { }
        bool operator<(const person_t &) const;
        friend istream & operator>>(istream &, person_t &);
        friend ostream & operator<<(ostream &, const person_t &);
        // `strwidth` stores the size of the string "lastname firstname"
	int strwidth;
    private:
        void set_strwidth();
        string firstname;
        string lastname;
        string phonenum;
}; 

// This is an overloaded less-than operator for the person_t class
// that is used to order the data.
bool person_t::operator<(const person_t &rhs) const {
    /* This function will return true if the left hand side's (LHS's) lastname
     * is less than the right hand side's (RHS's). If they are equal, this
     * function will then return true if the LHS's firstname is less than
     * the RHS's. If both the firstnames and lastnames are equal, the function
     * will return true if the LHS's phonenum is less than the RHS's. If any
     * of these check result in the LHS's value being greater than the RHS's,
     * the function will return false. Alternatively, if the two person_t objects
     * are equal, the function will also return false.
     */
    if (lastname < rhs.lastname) {
        return true;
    }
    else if (lastname == rhs.lastname) {
        if (firstname < rhs.firstname) {
            return true;
	}
	else if (firstname == rhs.firstname) {
	    if (phonenum < rhs.phonenum) {
                return true;
	    }
        }
    }
    return false;
}

// This function sets the `strwidth` member of the person_t class.
void person_t::set_strwidth() {
    strwidth = lastname.size() + firstname.size() + 1;
    return;
}

// This function stores data given by an `istream` object into a person_t object.
istream & operator>>(istream &in, person_t &r) { 
    /* `line` is a string used to get the desired line of text
     * from the `istream` object. `s` is the stringsteam object
     * used to extract individual words from `line`. The `word` variables
     * are the strings used to store the words extracted by `s`.
     */ 
    string line;
    string word1, word2, word3;
    sstream s;
    /* This code extracts the desired line from the console. Then,
     * that line is fed into a stringstream, which is used to store
     * the three desired "words" in the correct variables.
     */
    getline(in, line);
    s.str(line);
    s >> word1 >> word2 >> word3;
    /* The string members of the person_t object are set using the 
     * "words" obtained from the stringstream.
     */
    r.firstname = word1;
    r.lastname = word2;
    r.phonenum = word3;
    s.str();
    // This call is used to set the last member (`strwidth`) of the perso_t object.
    r.set_strwidth();
    return in;
}

// This function prints the data in a person_t object with the desired formatting.
ostream & operator<<(ostream &out, const person_t &r) {
    out << r.lastname << " " << r.firstname << "       ";
    out << setw(width - r.strwidth) << right << r.phonenum;
    return out;
}

typedef vector<person_t>::iterator ptiter;

/* This function sets the global `width` variable by
 * determining the largest `strwidth` member of the
 * elements in the vector and adding 20 to the largest.
 */
void setwidth(ptiter p1, ptiter p2) { 
    while (p1 != p2) {
        if (p1->strwidth > width) {
            width = p1->strwidth;
	}
	++p1;
    }
    width += 20;
    return;
}

/* This function is a driver for printing the contents
 * of a vector<person_t> object.
 */
void printlist(ptiter p1, ptiter p2) {
    ptiter tmp = p1;
    setwidth(tmp, p2);
    while (p1 != p2) {
        cout << *p1 << endl;
	    ++p1;
    }
    return;
}

int main(int argc, char *argv[]) {
    /* If there are more than 2 or exactly 1 user-provided 
     * command-line arguments, an error message will be produced
     * showing the correct usage of the executable from the command line.
     * Then, the program will be ended.
     */
    if (argc == 2 || argc > 3) {
        cerr << "Usage: ./Quicksort or ./Quicksort Lowerbound(Int) Upperbound(Int)\n";
	return -1;
    }

    int lower, upper;
    /* If there are 2 user-provided command-line arguments,
     * this block of code will convert them to integers and save them
     * in the variables lower and upper. If lower < 0, then lower has an
     * invalid value (because it is an index). So, the corresponding error
     * message is printed to the console before ending the program.
     */
    if (argc == 3) {
        lower = atoi(argv[1]);
	if (lower < 0) {
            cerr << "Usage: ./Quicksort or ./Quicksort Lowerbound(Int) Upperbound(Int)\n";
	    cerr << "The provided Lowerbound is not valid.\n";
	    return -2;
	}
	upper = atoi(argv[2]);
    }

    /* This code stores the data passed from the
     * console in the vector<person_t> object `A`.
     */
    vector<person_t> A;

    person_t din;
    while (cin >> din)
        A.push_back(din);

    /* If upper >= the size of the vector of data, then upper has an
     * invalid value. So, the corresponding error message is printed
     * to the console before ending the program.
     */
    if (argc == 3 && upper >= A.size()) {        
        cerr << "Usage: ./Quicksort or ./Quicksort Lowerbound(Int) Upperbound(Int)\n";
        cerr << "The provided Upperbound is not valid.\n";
        return -3;
    }
  
    int N = (int)A.size();
    srand(time(NULL));

    /* If there were no user-provided command-line arguments,
     * the quicksort function is called on the entire vector of data
     * to sort it. Otherwise (i.e. if there were 2 user-provided
     * command-line arguments), the quickselect function is called
     * twice to put the elements at lower and upper in there sorted
     * positions (see above for more info). Then, the quicksort function
     * is called to sort the data between lower and upper.
     */
    if (argc == 1) {
        quicksort(A, 0, N-1);
    }
    else {
        quickselect(A, 0, lower, N-1);
	quickselect(A, lower, upper, N-1);
	quicksort(A, lower, upper);
    }

    // Finally, the data is printed to the console.
    printlist(A.begin(), A.end());
    return 0;
}
