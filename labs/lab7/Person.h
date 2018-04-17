#ifndef __PERSON_H__ 
#define __PERSON_H__

#include <string>
#include <vector>

using namespace std;

enum person_enum  { UNKNOWN, FACULTY, STUDENT };
enum faculty_enum { ASST_PROF, ASSOC_PROF, FULL_PROF };
enum student_enum { FRESHMAN, SOPHOMORE, JUNIOR, SENIOR };

class person
{
    public:
        void set_name(string n) { name = n; }
        void set_category(string c) { category = c; }
        void add_courses(string course) { courses.push_back(course); }
        void add_courses(vector<string>&);
        string get_name() { return name; }
        string get_category() { return category; }
        vector<string>& get_courses();
        bool operator<(person&);
        friend ostream& operator<<(ostream&, person&);
    private:
        string name, category;
        vector<string> courses;
};

class faculty : public person
{

};

class student : public person
{

};

#endif
