#ifndef __PERSON_H__ 
#define __PERSON_H__

#include <iomanip>
#include <iostream>
#include <map>
#include <string>
#include <utility>
#include <vector>

using namespace std;

/*
 * Person.h
 * Author: Ian Lumsden
 * Date: 4/30/18
 *
 * This header file defines the abstract person class,
 * the derived faculty class, and the derived student
 * class. It also defines the person_enum, faculty_enum,
 * and student_enum.
 */

/* person_enum defines the type of person.
 * faculty_enum defines the type of faculty member.
 * student_enum defines the type of student.
 */
enum person_enum  { UNKNOWN, FACULTY, STUDENT };
enum faculty_enum { ASST_PROF, ASSOC_PROF, FULL_PROF };
enum student_enum { FRESHMAN, SOPHOMORE, JUNIOR, SENIOR };

/* The abstract class for any person (faculty, student,
 * or unknown). It defines several shared operators, virtual
 * members, and its own personal constructors and destructor.
 */
class person
{
    public:
        person(string);
        person(string, person_enum);
        virtual ~person() {;};
        // Setter function for the protected member `name`
        void set_name(string n) { name = n; }
        // Accessor function for the `name` member
        string get_name() { return name; }
        // Setter function for the formatting member `width`
        void set_course_width(int w) { width = w; }
        bool operator<(person&);
        friend ostream& operator<<(ostream&, person&);
    protected:
        /* name is the name of the person.
         * ptype is a person_enum variable that describes the type of person
         * width is the length of the longest course name amongst all the 
         *     person objects created.
         */
        string name;
        person_enum ptype;
        int width;
        virtual void print_details(ostream&) = 0;
        virtual void print_courses(ostream&) = 0;
};

/* The derrived class for any faculty person. It overloads
 * the virtual print_details and print_courses functions from
 * the person base class. It also implements several other
 * functions specific to the faculty class.
 */
class faculty : public person
{
    public:
        faculty(string);
        faculty(string, faculty_enum);
        faculty(string, faculty_enum, vector<string>&);
        void add_course(string);
        bool operator<(faculty&);
    protected:
        virtual void print_details(ostream&) override;
        virtual void print_courses(ostream&) override;
    private:
        /* ftype is a faculty_enum variable that describes the type
         *     of faculty person.
         * courses is a vector of strings that list the courses the
         *     faculty teaches.
         */
        faculty_enum ftype;
        vector<string> courses;
};

/* The derrived class for any student. It overloads the print_details
 * and print_courses functions from the person base class. It also
 * defines several other functions specific to the student class.
 */
class student : public person
{
    public:
        student(string);
        student(string, student_enum);
        student(string, student_enum, vector<string>&, vector<double>&);
        void add_course(string, int);
        bool operator<(student&);
    protected:
        virtual void print_details(ostream&) override;
        virtual void print_courses(ostream&) override;
    private:
        /* stype is a student_enum variable that describes the type
         *     of student person.
         * courses is a map of string keys and double data. The keys
         *     are the names of the courses the student is taking.
         *     The doubles are the grade points for the specific
         *     class taken.
         */
        student_enum stype;
        map<string, double> courses;
};

#endif
