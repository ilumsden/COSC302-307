#ifndef __PERSON_H__ 
#define __PERSON_H__

#include <iomanip>
#include <iostream>
#include <map>
#include <string>
#include <utility>
#include <vector>

using namespace std;

enum person_enum  { UNKNOWN, FACULTY, STUDENT };
enum faculty_enum { ASST_PROF, ASSOC_PROF, FULL_PROF };
enum student_enum { FRESHMAN, SOPHOMORE, JUNIOR, SENIOR };

class person
{
    public:
        person(string);
        person(string, person_enum);
        virtual ~person() {;};
        void set_name(string n) { name = n; }
        string get_name() { return name; }
        bool operator<(person&);
        friend ostream& operator<<(ostream&, person&);
    protected:
        string name;
        person_enum ptype;
        virtual void print_details(ostream&) = 0;
        virtual void print_courses(ostream&) = 0;
};

class faculty : public person
{
    public:
        faculty(string);
        faculty(string, faculty_enum);
        void add_course(string);
    protected:
        virtual void print_details(ostream&) override;
        virtual void print_courses(ostream&) override;
    private:
        faculty_enum ftype;
        vector<string> courses;
};

class student : public person
{
    public:
        student(string);
        student(string, student_enum);
        void add_course(string, int);
    protected:
        virtual void print_details(ostream&) override;
        virtual void print_courses(ostream&) override;
    private:
        student_enum stype;
        vector<string> courses;
        vector<int> gp;
};

#endif
