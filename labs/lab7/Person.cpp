#include <algorithm>
#include <cstdio>

#include "Person.h"

using namespace std;

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
    sort(courses.begin(), courses.end());
}

void faculty::print_details(ostream& fout)
{
    fout << setw(10) << right << "Category: ";
    switch (ftype)
    {
        case ASST_PROF: fout << "Assistant Professor\n"; break;
        case ASSOC_PROF: fout << "Associate Professor\n"; break; 
        case FULL_PROF: fout << "Full Professor\n"; break;
        default: fprintf(stderr, "Internal Error: Incorrect Faculty Type\n"); exit(-1);
    }
}

void faculty::print_courses(ostream& fout)
{
    for (string c : courses)
    {
        fout << setw(10) << right << "Course: ";
        fout << c << endl;
    }
}

student::student(string n)
    : person(n, STUDENT)
{}

stdent::student(string n, student_enum s)
    : person(n, STUDENT)
    , stype(s)
{}

void student::add_course(string c, int gpa)
{
    courses.insert(make_pair(c, gpa));
}

void student::print_details(ostream& fout)
{
    fout << setw(10) << right << "Category: ";
    switch (stype)
    {
        case FRESHMAN: fout << "Freshman\n"; break;
        case SOPHOMORE: fout << "Sophomore\n"; break;
        case JUNIOR: fout << "Junior\n"; break;
        case SENIOR: fout << "Senior\n"; break;
        default: fprintf(stderr, "Internal Error: Incorrect Student Type\n"); exit(-2);
    }
}

int student::course_width()
{
    int width = -1;
    for (auto c : courses)
    {
        if (c.first.size() > width)
        {
            width = c.first.size();
        }
    }
    return width;
}

double calc_gpa()
{
    double gpa;

}

void student::print_courses(ostream& fout)
{
    int width = course_width();
    width += 6;
    for (auto c : courses)
    {
        fout << setw(10) << right << "Course: ";
        fout << setw(width) << left << c.first;
    }
}
