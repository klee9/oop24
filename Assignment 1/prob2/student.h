#ifndef STUDENT_H
#define STUDENT_H

#include <string>
#include <iostream>
#include <iomanip>

class Student {
public:
    // constructor for initializing all attributes
    Student(std::string name, std::string student_id, int birth_year, std::string department, std::string tel);

    // getters for each attribute
    std::string get_name() const;
    std::string get_student_id() const;
    int get_birth_year() const;
    std::string get_department() const;
    std::string get_tel() const;

    // overload << operator to print student details
    friend std::ostream& operator<<(std::ostream &os, const Student &student);

private:
    std::string name;
    std::string student_id;
    int birth_year;
    std::string department;
    std::string tel;
};

#endif
