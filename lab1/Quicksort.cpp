
// include header files needed
using namespace std;

// template <typename Tdata>
// void quickselect(...) { write this }

template <typename Tdata>
void quicksort(...) { 
  // write this 
}

class person_t {
  public:
    person_t() { }

	// add operator< using lastname, firstname, phone number

    friend istream & operator>>(istream &, person_t &);
    friend ostream & operator<<(ostream &, const person_t &);

  private:
    string firstname;
    string lastname;
    string phonenum;
};

istream & operator>>(istream &in, person_t &r) { 
  // write this to read person_t object data
}

ostream & operator<<(ostream &out, const person_t &r) {
  // write this to write person_t object data
}

void printlist(...) { similar to arraylist_handout }

int main(int argc, char *argv[]) {
  // perform command-line check 

  vector<person_t> A;

  person_t din;
  while (cin >> din)
    A.push_back(din);
  
  int N = (int)A.size();

  int k0 = 0;
  int k1 = N-1;

  // if given as command-line arguments,
  // update k0, k1 and apply quickselect
  // to partition A accordingly

  quicksort(A, k0, k1);

  printlist(A.begin(), A.end());
}
