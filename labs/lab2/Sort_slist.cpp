#include <iostream>
#include <iomanip>
#include <sstream>
#include <vector>
#include <algorithm>

using namespace std;

/* Sort_slist.cpp
 * Author: Ian Lumsden
 * Date: February 5, 2018
 *
 * The code in this file reads text from the console (the text
 * is actually stored in files, but it is read through the console).
 * It then stores the personal data from the console into a
 * single-linked list of person_t objects. Then, using a smart
 * pointer subclass, the linked list is sorted by making a vector of
 * smart pointers that point to list elements. This vector is then
 * sorted and used to sort the list of data. Finally, this code prints
 * the sorted contents of the list with specific foratting.
 */

// This global variable is used to set formatting.
int width = 0;

typedef stringstream sstream;

/* This class defines the single-linked
 * list object used to store the data.
 */
template <class T>
class slist {
    private:
        /* This struct defines the nodes that
         * make up the linked list.
         */
        struct node {
            /* The default constructor for the node object.
             * It sets members data and next to their default
             * values.
             */
            node() { data = T(); next = NULL; }
            /* The copy constructor for the node object.
             * It sets the data member with the parameter passed.
             * It also sets next to its default value.
             */
            node(const T &key) { data = key; next = NULL; }
            /* This less-than operator for node objects is used
             * by the smart pointer object to allow for comparison
             * and sorting.
             */
            bool operator<(const node &rhs) const { return (data < rhs.data); }

            T data;
            node *next;
        };

        /* This class defines the smart pointers that will be used
         * to sort the contents of the list.
         */
        class sptr {
            public:
                /* The smart pointer constructor.
                 * If a parameter is passed, the member
                 * pointer (ptr) is set using that parameter.
                 * Otherwise, ptr is set to NULL.
                 */
                sptr(node *_ptr=NULL) { ptr = _ptr; }
                /* This less-than operator is directly used to compare
                 * smart pointers and sort the linked list. When called,
                 * it will use pointer dereferencing to call the node 
                 * comparison operator.
                 */
                bool operator<(const sptr &rhs) const { return (*ptr < *rhs.ptr); }
                /* This overloaded dereference operator converts the
                 * smart pointer object into a T* pointer.
                 */
                operator node * () const { return ptr; }
            private:
                node *ptr;
        };

    public:
        class iterator {
            public:
                iterator() { p = NULL; }
                T & operator*() { return p->data; }
                iterator & operator++() { p = p->next; return *this; }
                bool operator!=(const iterator & rhs) const { return p != rhs.p; }

                friend class slist<T>;
            private:
                iterator(node *n_p) { p = n_p; }
                node *p;
        };
    public:
        slist();
        ~slist();

        void push_back(const T &);
        void sort();

        iterator begin() { return iterator(head->next); }
        iterator end() { return iterator(NULL); }

    private:
        node *head;
        node *tail;
};

template <typename T>
slist<T>::slist() {
    head = new node();
    tail = head;
}

template <typename T>
slist<T>::~slist() {
    while (head->next != NULL) {
        node *p = head->next;
        head->next = p->next;
        delete p;
    }
    delete head;

    head = NULL;
    tail = NULL;
}

template <typename T>
void slist<T>::push_back(const T &din) {
    tail->next = new node(din);
    tail = tail->next;
}

template <typename T>
void slist<T>::sort() {
    iterator i = begin();
    iterator j = end();
    int size = 0;
    while (i != j) {
        size++;
        ++i;
    }
    vector<sptr> Ap;
    Ap.resize(size);
    node *p = head;
    for (int k = 0; k < size; k++) {
        Ap[k] = p->next;
        p = p->next;
    }
    std::sort(Ap.begin(), Ap.end());
    p = head;
    for (int k = 0; k < size; k++) {
        p->next = Ap[k];
        p = p->next;
    }
    tail = p;
    tail->next = NULL;
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

typedef slist<person_t>::iterator ptiter;

/* This function sets the global `width` variable by
 * determining the largest `strwidth` member of the
 * elements in the vector and adding 20 to the largest.
 */
void setwidth(ptiter p1, ptiter p2) { 
    while (p1 != p2) {
        if ((*p1).strwidth > width) {
            width = (*p1).strwidth;
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
    if (argc > 1) {
        cerr << "Usage: ./Sort_sptr (No arguments)\n";
        return -1;
    }
    slist<person_t> A;

    person_t din;
    while (cin >> din)
        A.push_back(din);

    A.sort();

    printlist(A.begin(), A.end());
}
