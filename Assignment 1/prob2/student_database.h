#ifndef STUDENTDATABASE_H
#define STUDENTDATABASE_H

#include "student.h"
#include <vector>
#include <fstream>

// manages a collection of students
class StudentDatabase {
public:
    StudentDatabase(const std::string& filename);

    // load/save student data from/to a file
    void load_from_file();
    void save_to_file();

    // manage/search students
    void add_student(const Student& student);
    std::vector<Student> search_by_name(const std::string& name) const;
    std::vector<Student> search_by_student_id(const std::string& student_id) const;
    std::vector<Student> search_by_admission_year(int year) const;
    std::vector<Student> search_by_birth_year(int year) const;
    std::vector<Student> search_by_department(const std::string& department) const;
    std::vector<Student> list_all_students() const;

    // sort options
    void sort_by_name();
    void sort_by_student_id();
    void sort_by_birth_year();
    void sort_by_department();

private:
    std::vector<Student> students;  // collection of students
    std::string filename;           // file to read/write student data

    // function to check if a student already exists by ID
    bool student_exists(const std::string& student_id) const;
};

#endif
