#include <algorithm>
#include <cstdio>

#include "Person.h"

using namespace std;

/*
 * Person.cpp
 * Author: Ian Lumsden
 * Date: 4/30/18
 *
 * This file provides the implementations for the methods defined
 * in the classes from Person.h
 */

/* Person Constructor V1:
 * Sets the name member using the parameter, and
 * sets ptype to UNKNOWN.
 */
person::person(string n)
    : name(n)
    , ptype(UNKNOWN)
{}

/* Person Constructor V2:
 * Uses the parameters to set the name and ptype members.
 */
person::person(string n, person_enum p)
    : name(n)
    , ptype(p)
{}

// An overloaded less-than operator for comparison of person objects
bool person::operator<(person& rhs)
{
    /* If the two person objects are actually faculty or students
     * objects, the less-than comparison operator for the specific derrived
     * class is called. Otherwise, the comparison is based on the 
     * name member.
     */
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
        return (name < rhs.name);
    }
    /* Returns true if the left hand side is a faculty object
     * and the right hand side is not a faculty object.
     */
    if (ptype == FACULTY && (rhs.ptype == STUDENT || rhs.ptype == UNKNOWN))
    {
        return true;
    }
    /* If neither object is a faculty object, returns true if the left
     * hand side is a student object, and the right hand side is not.
     */
    else if (ptype == STUDENT && rhs.ptype == UNKNOWN)
    {
        return true;
    }
    // Otherwise, returns false.
    return false;
}

/* This operator allows a person object to be printed
 * using any ostream object (i.e. cout).
 */
ostream& operator<<(ostream& fout, person& per)
{
    fout << "  " << setw(10) << right  << "Name: " << per.name << endl;
    /* If the person object is an upcasted faculty or student
     * object, call the object's print_details and print_courses
     * methods.
     */
    if (per.ptype == FACULTY || per.ptype == STUDENT)
    {
        per.print_details(fout);
        per.print_courses(fout);
    }
    return fout;
}

/* Faculty constructor V1:
 * Calls the base class constructor with the name parameter being passed.
 */
faculty::faculty(string n)
    : person(n, FACULTY)
{}

/* Faculty constructor V2:
 * Calls the base class constructor with the name parameter being passed.
 * Also sets the ftype member with the other parameter.
 */
faculty::faculty(string n, faculty_enum f)
    : person(n, FACULTY)
    , ftype(f)
{}

/* Faculty constructor V3:
 * Same as V2, but also sets the course vector using the last parameter.
 */
faculty::faculty(string n, faculty_enum f, vector<string>& clist)
    : person(n, FACULTY)
    , ftype(f)
{
    for (string c : clist)
    {
        add_course(c);
    }
}

// Adds an individual course to the courses vector and sorts the vector.
void faculty::add_course(string c)
{
    courses.push_back(c);
    sort(courses.begin(), courses.end());
}

// The less-than operator for comparing faculty objects
bool faculty::operator<(faculty& rhs)
{
    /* If the faculty types are the same, comparison is
     * based on the comparison of the names.
     */
    if (ftype == rhs.ftype)
    {
        return (name < rhs.name);
    }
    /* If the left hand side faculty type is ASST_PROF and the
     * right hand side's isn't, return true.
     */
    else if (ftype == ASST_PROF && rhs.ftype != ASST_PROF)
    {
        return true;
    }
    /* If the left hand side faculty type is ASSOC_PROF and the
     * right hand side's is FULL_PROF, return true.
     */
    else if (ftype == ASSOC_PROF && rhs.ftype == FULL_PROF)
    {
        return true;
    }
    // Otherwise, return false.
    return false;
}

// Prints the faculty type with correct formatting to fout.
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

// Prints the courses taught by the faculty member to fout
void faculty::print_courses(ostream& fout)
{
    for (string c : courses)
    {
        fout << "  " << setw(10) << right << "Course: ";
        fout << c << endl;
    }
}

/* Student constructor V1:
 * Calls the base class constructor with the name parameter being passed.
 */
student::student(string n)
    : person(n, STUDENT)
{}

/* Student constructor V2:
 * Calls the base class constructor with the name parameter being passed.
 * Also sets the stype member with the other parameter.
 */
student::student(string n, student_enum s)
    : person(n, STUDENT)
    , stype(s)
{}

/* Student constructor V3:
 * Same as V2, but also sets the course map using the last two parameters.
 */
student::student(string n, student_enum s, vector<string>& cnames, vector<double>& gp)
    : person(n, STUDENT)
    , stype(s)
{
    for (int i = 0; i < (int)(cnames.size()); i++)
    {
        add_course(cnames[i], gp[i]);
    }
}

// Adds a course/grade point pair to the courses map
void student::add_course(string c, int gpa)
{
    courses.insert(make_pair(c, gpa));
}

// The less-than operator for comparing student objects
bool student::operator<(student& rhs)
{
    /* If the student types of the two objects are the same, 
     * comparison is done by name.
     */
    if (stype == rhs.stype)
    {
        return (name < rhs.name);
    }
    // FRESHMAN has highest priority, so it is less-than any other student.
    else if (stype == FRESHMAN && rhs.stype != FRESHMAN)
    {
        return true;
    }
    /* SOPHOMORE has the second highest priority, so it is less-than any
     * non-FRESHMAN student.
     */
    else if (stype == SOPHOMORE && (rhs.stype != FRESHMAN && rhs.stype != SOPHOMORE))
    {
        return true;
    }
    /* JUNIOR has the second lowest priority, so it is only less-than a
     * SENIOR student.
     */
    else if (stype == JUNIOR && rhs.stype == SENIOR)
    {
        return true;
    }
    // Otherwise, return false.
    return false;
}

// Prints the student type with correct formatting to fout.
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

/* Prints the courses taken by the student, the course's
 * grade points, and the student's overall GPA to fout
 */
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
