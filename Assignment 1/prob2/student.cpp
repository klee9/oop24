#include "student.h"

using namespace std;

// constructor to initialize all attributes
Student::Student(string name, string student_id, int birth_year, string department, string tel)
    : name(name), student_id(student_id), birth_year(birth_year), department(department), tel(tel) {}

// getters for each attribute
string Student::get_name() const { return name; }
string Student::get_student_id() const { return student_id; }
int Student::get_birth_year() const { return birth_year; }
string Student::get_department() const { return department; }
string Student::get_tel() const { return tel; }

// for printing a student's details
ostream& operator<<(ostream &os, const Student &student) {
    os << left << setw(15) << student.name
       << left << setw(12) << student.student_id
       << left << setw(20) << student.department
       << left << setw(6) << student.birth_year
       << left << setw(15) << student.tel;
    return os;
}
