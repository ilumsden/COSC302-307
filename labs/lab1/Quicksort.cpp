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

template <typename Tdata>
void quickselect(vector<Tdata> &data, int lowerbound, int k, int upperbound) {
    while (1) {        
        if (upperbound <= lowerbound) {
            return;
        }
        int size = (upperbound - lowerbound) + 1;
        int pindex = rand() % size + lowerbound;
        Tdata pivot = data[pindex];
        swap(data[pindex], data[upperbound]);
        int i = lowerbound - 1;
        int j = upperbound;
        while (1) {
            while (data[++i] < pivot) { }
	        while (j-1 >= 0 && pivot < data[--j]) { }
	        if (i>=j) {
                break;
	        }
	        swap(data[i], data[j]);
        }
        pindex = i;
        swap(data[pindex], data[upperbound]);
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
// that is used by the quicksort function to order the data.
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
void printlist(ptiter p1, ptiter p2, int lower, int upper) {
    ptiter tmp = p1;
    setwidth(tmp, p2);
	int c = -1;
    while (p1 != p2) {
        cout << *p1;
		c++;
		if (c >= lower && c <= upper) {
            cout << " **";
		}
		cout << "\n";
	++p1;
    }
    return;
}

int main(int argc, char *argv[]) {
	if (argc == 2 || argc > 3) {
        cerr << "Usage: ./Quicksort or ./Quicksort Lowerbound(Int) Upperbound(Int)\n";
		return -1;
	}

	int lower, upper;
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

    if (upper >= A.size()) {        
        cerr << "Usage: ./Quicksort or ./Quicksort Lowerbound(Int) Upperbound(Int)\n";
        cerr << "The provided Upperbound is not valid.\n";
        return -3;
	}
  
    /* The size of vector `A` is used to set the initial
     * bounds for the quicksort algorithm.
     */
    int N = (int)A.size();
	srand(time(NULL));

    /* The quicksort() function is called to sort the contents of
     * `A`. Then, the contents of `A` are printed to the console
     * using the printlist() function.
     */ 
    if (argc == 1) {
        quicksort(A, 0, N-1);
	}
	else {
        quickselect(A, 0, lower, N-1);
		quickselect(A, lower, upper, N-1);
		quicksort(A, lower, upper);
	}

    printlist(A.begin(), A.end(), lower, upper);
}
