#include WHAT'S NEEDED
using namespace std;

#include "Person.h"
#include "Sptrsort.h"

int main(int argc, char *argv[]) {
  if (argc/argv problem) {
    output error message
	return 0;
  }

  person *n_person;
  vector<person *> person_list;

  person_enum person_type = UNKNOWN;

  int line = 0;
  size_t iL, iR;
  string input;
  string name;
  string category;
  vector<string> course;
  vector<float> gp;

  while (getline(cin, input)) {
	line++;

	if ((iL = input.find('<')) == string::npos) {
	  continue;

    } else if (input.compare(iL,9,"<faculty>") == 0) {
	  person_type = FACULTY;
	  course.clear();
	  gp.clear();

	} else if (input.compare(iL,10,"</faculty>") == 0) {
	  faculty_type_e faculty_type;

	  if (category.compare("Assistant Professor") == 0)
	    faculty_type = ASST;
	  else if (category.compare("Associate Professor") == 0)
	    faculty_type = ASSOC;
	  else if (category.compare("Full Professor") == 0)
	    faculty_type = FULL;

	  CODE FOR ADDING FACULTY PERSON TO DATABASE
	  n_person = ?

	  person_list.push_back(n_person);

	  person_type = UNKNOWN;
	  continue;

	} else if (input.compare(iL,9,"<student>") == 0) {
	  person_type = STUDENT;
	  course.clear();
	  gp.clear();

	} else if (input.compare(iL,10,"</student>") == 0) {
	  student_type_e student_type;

	  if (category.compare("Freshman") == 0)
	    student_type = FRESHMAN;
	  else if (category.compare("Sophomore") == 0)
	    student_type = SOPHOMORE;
	  else if (category.compare("Junior") == 0)
	    student_type = JUNIOR;
	  else if (category.compare("Senior") == 0)
	    student_type = SENIOR;

	  CODE FOR ADDING STUDENT PERSON TO DATABASE
	  n_person = ?

	  person_list.push_back(n_person);

	  person_type = UNKNOWN;
	  continue;

	} else if (input.compare(iL,5,"<name") == 0) {
	  iL = input.find("=\"", iL);
	  iR = input.find("\"/>", iL+2);
	  name = input.substr(iL+2,iR-(iL+2));

	} else if (input.compare(iL,10,"<category=") == 0) {
	  iL = input.find("=\"", iL);
	  iR = input.find("\"/>", iL+2);
	  category = input.substr(iL+2,iR-(iL+2));

	} else if (input.compare(iL,7,"<course") == 0) {
	  iL = input.find("=\"", iL);
	  iR = input.find("\"", iL+2);
	  course.push_back(input.substr(iL+2,iR-(iL+2)));

	  iL = iR;
	  if (person_type == FACULTY) {
	    iR = input.find("/>", iL+1);
	  } else if (person_type == STUDENT) {
	    iL = input.find("gp=\"", iL);
	    iR = input.find("\"/>", iL+4);

	    stringstream ss;
	    ss << input.substr(iL+4,iR-(iL+3)); 
		ss >> new_gp;
	    gp.push_back(new_gp);
	  }
	}
  }

  // MODIFY TO INFINITE LOOP ASKING FOR PERSON,
  // FACULTY OR STUDENT MODE FOR SORTING

  sptrsort(person_list.begin(), person_list.end());

  for (int i=0; i<(int)person_list.size(); i++)
    cout << *person_list[i] << "\n";

  // RELEASE DYNAMICALLY ALLOACTED MEMORY
}
