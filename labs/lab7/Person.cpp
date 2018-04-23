using namespace std;

#include "Person.h"

person::person(string n)
    : name(n)
    , ptype(UNKOWN)
{}

person::person(string n, person_enum p)
    : name(n)
    , ptype(p)
{}

bool person::operator<(person& rhs)
{
    return (name < rhs.name) ? true : false;
}

ostream& operator<<(ostream& fout, person& per)
{
    fout << setw(10) << right  << "Name: " << name << endl;
    if (ptype == FACULTY || ptype == STUDENT)
    {
        print_details(fout);
        print_courses(fout);
    }
}

faculty::faculty(string n)
    : person(n, FACULTY)
{}

faculty::faculty(string n, faculty_enum f)
    : person(n, FACULTY)
    , ftype(f)
{}

void faculty::add_course(string c)
{
    courses.push_back(c);
}
