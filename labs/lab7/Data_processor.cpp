#include <fstream>
#include <sstream>

using namespace std;

#include "Person.h"
#include "Sptrsort.h"

/*
 * Data_processor.cpp
 * Author: Ian Lumsden
 * Date: 4/30/18
 *
 * The driver code for this lab.
 * This file contains the main function for this lab. It manages the
 * command line arguments. Then, it parses the XML file specifed by
 * argv[1] into a vector of person objects. These person objects are
 * actually either faculty or student objects (derrived classes) that
 * are upcasted to person objects. Then, the program waits for user
 * input to specify what data it will print to the console. After 
 * stdin is closed, the code frees the memory used by the pointers
 * stored in the person vector.
 */

int main(int argc, char *argv[]) {
  /* If the wrong number of command line arguments are passed or if
   * argv[1] is not the name of an XML file, an error message is
   * printed to stderr, and the program exits.
   */
  if (argc != 2 || string(argv[1]).substr(string(argv[1]).size()-4, 4) != ".xml") {
      fprintf(stderr, "Usage: ./Data_processor data.xml\n");
      return -3;
  }

  /* Most of this code was provided by Dr. Gregor.
   * As a result, the only parts that will be commented are the
   * parts that I have written. There is another comment farther
   * below that specifies when the provided code has ended.
   */
  person *n_person;
  vector<person *> person_list;

  person_enum person_type = UNKNOWN;

  int line = 0;
  // width stores the length of the longest course name.
  int width = -1;
  size_t iL, iR;
  string input;
  string name;
  string category;
  double new_gp;
  vector<string> course;
  vector<double> gp;

  /* This code was added to fix a mistake in the provided code
   * where the data was wrongly assumed to be passed through stdin.
   * It opens a file stream in input mode using the file name passed
   * as argv[1]. It prints an error message to stderr if the file
   * could not be openned. Then, it exits.
   */
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

	  /* Makes a new faculty object using `name`, `faculty_type`,
       * and `course`. This object is immediately upcasted and added
       * to the person_list vector.
       */
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

	  /* Makes a new student object using `name`, `student_type`,
       * `course`, and `gp`. This object is immediately upcasted and added
       * to the person_list vector.
       */
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
  // THIS IS THE END OF THE PROVIDED CODE
  // Closes the file stream to prevent data loss.
  fin.close();

  // Sets the width member of each person object.
  for (person* p : person_list)
  {
      p->set_course_width(width);
  }

  /* Sorts the person_list vector using the 
   * smart pointer sort from Sptrsort.h
   */
  sptrsort<person>(person_list.begin(), person_list.end());
  // Prints the command options to stdout.
  printf("command: person\ncommand: faculty\ncommand: student\n\n");
  // Prints the command prompt
  printf("command> ");
  string type;
  // This loop continues until stdin is closed.
  while (cin >> type)
  {
      /* If the "person" command was enterred, prints
       * the entire contents of the person_list vector
       * to stdout.
       */
      if (type == "person")
      {
          for (int i=0; i<(int)person_list.size(); i++)
            cout << *person_list[i] << "\n";
      }
      /* If the "faculty" command was enterred, prints all the
       * elements of the person_list vector that can be downcasted
       * to faculty objects.
       */
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
      /* If the "student command was enterred, prints all the
       * elements of the person_list vector that can be downcasted
       * to faculty objects.
       */
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
      // Prints another command prompt
      printf("command> ");
  }
  printf("\n");

  // Frees up the memory taken up by the elements in person_list
  for (int i = 0; i < (int)(person_list.size()); i++)
  {
      n_person = person_list.back();
      person_list.pop_back();
      delete n_person;
  }
}
