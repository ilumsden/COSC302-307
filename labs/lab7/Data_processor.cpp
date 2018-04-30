#include <fstream>
#include <sstream>

using namespace std;

#include "Person.h"
#include "Sptrsort.h"

int main(int argc, char *argv[]) {
  if (argc != 2 || string(argv[1]).substr(string(argv[1]).size()-4, 4) != ".xml") {
      fprintf(stderr, "Usage: ./Data_processor data.xml\n");
      return -3;
  }

  person *n_person;
  vector<person *> person_list;

  person_enum person_type = UNKNOWN;

  int line = 0;
  int width = -1;
  size_t iL, iR;
  string input;
  string name;
  string category;
  double new_gp;
  vector<string> course;
  vector<double> gp;

  fstream fin;
  fin.open(argv[1], ios::in);
  if (!fin.is_open())
  {
      fprintf(stderr, "Could not open %s\n", argv[1]);
      return -4;
  }

  while (getline(fin, input)) {
	line++;

	if ((iL = input.find('<')) == string::npos) {
	  continue;

    } else if (input.compare(iL,9,"<faculty>") == 0) {
	  person_type = FACULTY;
	  course.clear();
	  gp.clear();

	} else if (input.compare(iL,10,"</faculty>") == 0) {
	  faculty_enum faculty_type;

	  if (category.compare("Assistant Professor") == 0)
	    faculty_type = ASST_PROF;
	  else if (category.compare("Associate Professor") == 0)
	    faculty_type = ASSOC_PROF;
	  else if (category.compare("Full Professor") == 0)
	    faculty_type = FULL_PROF;

	  //CODE FOR ADDING FACULTY PERSON TO DATABASE
	  n_person = new faculty(name, faculty_type, course); 

	  person_list.push_back(n_person);

	  person_type = UNKNOWN;
	  continue;

	} else if (input.compare(iL,9,"<student>") == 0) {
	  person_type = STUDENT;
	  course.clear();
	  gp.clear();

	} else if (input.compare(iL,10,"</student>") == 0) {
	  student_enum student_type;

	  if (category.compare("Freshman") == 0)
	    student_type = FRESHMAN;
	  else if (category.compare("Sophomore") == 0)
	    student_type = SOPHOMORE;
	  else if (category.compare("Junior") == 0)
	    student_type = JUNIOR;
	  else if (category.compare("Senior") == 0)
	    student_type = SENIOR;

	  //CODE FOR ADDING STUDENT PERSON TO DATABASE
	  n_person = new student(name, student_type, course, gp);

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
          if ((int)(input.substr(iL+2,iR-(iL+2)).size()) > width)
          {
              width = (int)(input.substr(iL+2,iR-(iL+2)).size());
          }

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
  fin.close();

  for (person* p : person_list)
  {
      p->set_course_width(width);
  }

  sptrsort<person>(person_list.begin(), person_list.end());
  printf("command: person\ncommand: faculty\ncommand: student\n\n");
  printf("command> ");
  string type;
  while (cin >> type)
  {
      if (type == "person")
      {
          for (int i=0; i<(int)person_list.size(); i++)
            cout << *person_list[i] << "\n";
      }
      else if (type == "faculty")
      {
          for (int i=0; i<(int)person_list.size(); i++)
          {
              faculty *fac = dynamic_cast<faculty*>(person_list[i]);
              if (fac)
              {
                  cout << *fac << "\n";
              }
          }
      }
      else if (type == "student")
      {
          for (int i=0; i<(int)person_list.size(); i++)
          {
              student *stu = dynamic_cast<student*>(person_list[i]);
              if (stu)
              {
                  cout << *stu << "\n";
              }
          }
      }
      printf("command> ");
  }
  printf("\n");

  for (int i = 0; i < (int)(person_list.size()); i++)
  {
      n_person = person_list.back();
      person_list.pop_back();
      delete n_person;
  }
}
