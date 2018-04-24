#include <algorithm>
#include <cstdio>

#include "Person.h"

using namespace std;

person::person(string n)
    : name(n)
    , ptype(UNKNOWN)
{}

person::person(string n, person_enum p)
    : name(n)
    , ptype(p)
{}

bool person::operator<(person& rhs)
{
    if (ptype == rhs.ptype)
    {
        if (ptype == FACULTY)
        {
            return *dynamic_cast<faculty*>(this) < *dynamic_cast<faculty*>(&rhs);
        }
        else if (ptype == STUDENT) 
        {
            return *dynamic_cast<student*>(this) < *dynamic_cast<student*>(&rhs);
        }
        return (name < rhs.name) ? true : false;
    }
    if (ptype == FACULTY && (rhs.ptype == STUDENT || rhs.ptype == UNKNOWN))
    {
        return true;
    }
    else if (ptype == STUDENT && rhs.ptype == UNKNOWN)
    {
        return true;
    }
    return false;
}

ostream& operator<<(ostream& fout, person& per)
{
    fout << "  " << setw(10) << right  << "Name: " << per.name << endl;
    if (per.ptype == FACULTY || per.ptype == STUDENT)
    {
        per.print_details(fout);
        per.print_courses(fout);
    }
    return fout;
}

faculty::faculty(string n)
    : person(n, FACULTY)
{}

faculty::faculty(string n, faculty_enum f)
    : person(n, FACULTY)
    , ftype(f)
{}

faculty::faculty(string n, faculty_enum f, vector<string>& clist)
    : person(n, FACULTY)
    , ftype(f)
{
    for (string c : clist)
    {
        add_course(c);
    }
}

void faculty::add_course(string c)
{
    courses.push_back(c);
    sort(courses.begin(), courses.end());
}

bool faculty::operator<(faculty& rhs)
{
    if (ftype == rhs.ftype)
    {
        return (name < rhs.name) ? true : false;
    }
    else if (ftype == ASST_PROF && rhs.ftype != ASST_PROF)
    {
        return true;
    }
    else if (ftype == ASSOC_PROF && rhs.ftype == FULL_PROF)
    {
        return true;
    }
    return false;
}

void faculty::print_details(ostream& fout)
{
    fout << "  " << setw(10) << right << "Category: ";
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
        fout << "  " << setw(10) << right << "Course: ";
        fout << c << endl;
    }
}

student::student(string n)
    : person(n, STUDENT)
{}

student::student(string n, student_enum s)
    : person(n, STUDENT)
    , stype(s)
{}

student::student(string n, student_enum s, vector<string>& cnames, vector<double>& gp)
    : person(n, STUDENT)
    , stype(s)
{
    for (int i = 0; i < (int)(cnames.size()); i++)
    {
        add_course(cnames[i], gp[i]);
    }
}

void student::add_course(string c, int gpa)
{
    courses.insert(make_pair(c, gpa));
}

bool student::operator<(student& rhs)
{
    if (stype == rhs.stype)
    {
        return (name < rhs.name) ? true : false;
    }
    else if (stype == FRESHMAN && rhs.stype != FRESHMAN)
    {
        return true;
    }
    else if (stype == SOPHOMORE && (rhs.stype != FRESHMAN && rhs.stype != SOPHOMORE))
    {
        return true;
    }
    else if (stype == JUNIOR && rhs.stype == SENIOR)
    {
        return true;
    }
    return false;
}

void student::print_details(ostream& fout)
{
    fout << "  " << setw(10) << right << "Category: ";
    switch (stype)
    {
        case FRESHMAN: fout << "Freshman\n"; break;
        case SOPHOMORE: fout << "Sophomore\n"; break;
        case JUNIOR: fout << "Junior\n"; break;
        case SENIOR: fout << "Senior\n"; break;
        default: fprintf(stderr, "Internal Error: Incorrect Student Type\n"); exit(-2);
    }
}

void student::print_courses(ostream& fout)
{
    width += 4;
    double gpa = 0;
    int num = 0;
    for (auto c : courses)
    {
        fout << "  " << setw(10) << right << "Course: ";
        fout << setw(width) << left << c.first;
        gpa += c.second;
        num++;
        fout << fixed << setprecision(2) << c.second << " ";
        fout << fixed << setprecision(2) << (gpa/num) << endl;
    }
}
