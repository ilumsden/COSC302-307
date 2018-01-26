#include <iostream>
#include <sstream>
#include <string>
#include <iomanip>
#include <vector>
#include <cstdlib>
#include <algorithm>

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

// Quickselect is for Part B
// template <typename Tdata>
// void quickselect(...) { write this }

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
    int i = lowerbound - 1;
    int j = upperbound;
    while (1) {
        while (data[++i] < pivot) { }
	while (pivot < data[--j]) { }
	if (i>=j) {
            break;
	}
	swap(data[i], data[j]);
    }
    pindex = i;
    swap(data[pindex], data[upperbound]);
    quicksort(data, lowerbound, pindex-1);
    quicksort(data, pindex+1, upperbound);
    return;
}

class person_t {
    public:
        person_t() { }
        bool operator<(const person_t &) const;
        friend istream & operator>>(istream &, person_t &);
        friend ostream & operator<<(ostream &, const person_t &);
	int strwidth;
    private:
        void set_strwidth();
        string firstname;
        string lastname;
        string phonenum;
}; 

bool person_t::operator<(const person_t &rhs) const {
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

void person_t::set_strwidth() {
    strwidth = lastname.size() + firstname.size() + 1;
    return;
}

istream & operator>>(istream &in, person_t &r) { 
    string line;
    string word;
    sstream s;
    int i;
    getline(in, line);
    s.str(line);
    i = 0;
    while (s >> word) {
        if (i == 0) {
            r.firstname = word;
	}
	else if (i == 1) {
            r.lastname = word;
	}
	else {
            r.phonenum = word;
	}
	i++;
    }
    s.str();
    r.set_strwidth();
    return in;
}

ostream & operator<<(ostream &out, const person_t &r) {
    out << r.lastname << " " << r.firstname << "       ";
    out << setw(width - r.strwidth) << right << r.phonenum;
    return out;
}

typedef vector<person_t>::iterator ptiter;
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
    // Part B, Vers. 1: perform command-line check 

    vector<person_t> A;

    person_t din;
    while (cin >> din)
        A.push_back(din);
  
    int N = (int)A.size();

    int k0 = 0;
    int k1 = N-1;

    // Part B: if given as command-line arguments,
    // update k0, k1 and apply quickselect
    // to partition A accordingly
	quicksort(A, k0, k1);

    printlist(A.begin(), A.end());
}
