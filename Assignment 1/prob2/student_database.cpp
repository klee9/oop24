#include "student_database.h"
#include <algorithm>
#include <iostream>
#include <iomanip>
#include <sstream>

using namespace std;

// constructor that sets the filename
StudentDatabase::StudentDatabase(const string& filename) : filename(filename) {
    load_from_file();  // load existing data from the file
}

// load student data from the file
void StudentDatabase::load_from_file() {
    ifstream infile(filename);
    if (!infile) {
        cout << "Error: Could not open the file" << endl;
        return;
    }

    string line;
    while (getline(infile, line)) {
        stringstream ss(line);
        string name, student_id, department, tel;
        int birth_year;

        // read each field
        if (getline(ss, name, '|') &&
            getline(ss, student_id, '|') &&
            getline(ss, department, '|') &&
            ss >> birth_year) {
            ss.ignore(1, '|');
            getline(ss, tel, '|');

            // create a student object and add it to the student list
            students.push_back(Student(name, student_id, birth_year, department, tel));
        }
    }

    infile.close();
}



// save student data to the file
void StudentDatabase::save_to_file() {
    ofstream outfile(filename);
    for (const auto& student : students) {
        outfile << student.get_name() << "|" << student.get_student_id() << "|"
                << student.get_department() << "|" << student.get_birth_year() << "|"
                << student.get_tel() << endl;
    }
    outfile.close();
}

// add a new student to the database
void StudentDatabase::add_student(const Student& student) {
    if (student_exists(student.get_student_id())) {
        cout << "Error: Already inserted." << endl;
        return;
    }
    students.push_back(student);
    sort_by_name();  // default sorting option
    save_to_file();  // save immediately after addition
    cout << "Student added successfully." << endl;
}

// check if a student with the same ID already exists. If so, throw error message
bool StudentDatabase::student_exists(const string& student_id) const {
    for (const auto& student : students)
        if (student.get_student_id() == student_id) return true;
    return false;
}


/*
 * searching part
 * 1. search_by_name: search students by name
 * 2. search_by_student_id: search students by student ID
 * 3. search_by_addmission_year: search students by admission year (student ID prefix)
 * 4. search_by_birth_year: search students by birth year
 * 5. search_by_department: search students by department
 * 6. list_all_students: list all students in the database
 */


vector<Student> StudentDatabase::search_by_name(const string& name) const {
    vector<Student> result;
    for (const auto& student : students) {
        if (student.get_name() == name) result.push_back(student);
    }
    return result;
}

vector<Student> StudentDatabase::search_by_student_id(const string& student_id) const {
    vector<Student> result;
    for (const auto& student : students) {
        if (student.get_student_id() == student_id) result.push_back(student);
    }
    return result;
}

vector<Student> StudentDatabase::search_by_admission_year(int year) const {
    vector<Student> result;
    string year_prefix = to_string(year);
    for (const auto& student : students) {
        if (student.get_student_id().substr(0, 4) == year_prefix) result.push_back(student);
    }
    return result;
}

vector<Student> StudentDatabase::search_by_birth_year(int year) const {
    vector<Student> result;
    for (const auto& student : students) {
        if (student.get_birth_year() == year) result.push_back(student);
    }
    return result;
}

vector<Student> StudentDatabase::search_by_department(const string& department) const {
    vector<Student> result;
    for (const auto& student : students) {
        if (student.get_department() == department) result.push_back(student);
    }
    return result;
}

vector<Student> StudentDatabase::list_all_students() const {
    return students;
}


/*
 * sorting part
 * 1. sort_by_name: sort students by name
 * 2. sort_by_student_id: sort students by student ID
 * 3. sort_by_birth_year: sort students by birth year
 * 4. sort_by_department: sorts student by department
 */


void StudentDatabase::sort_by_name() {
    sort(students.begin(), students.end(), [](const Student& a, const Student& b) { return a.get_name() < b.get_name(); });
}

void StudentDatabase::sort_by_student_id() {
    sort(students.begin(), students.end(), [](const Student& a, const Student& b) { return a.get_student_id() < b.get_student_id(); });
}

void StudentDatabase::sort_by_birth_year() {
    sort(students.begin(), students.end(), [](const Student& a, const Student& b) { return a.get_birth_year() < b.get_birth_year(); });
}

void StudentDatabase::sort_by_department() {
    sort(students.begin(), students.end(), [](const Student& a, const Student& b) { return a.get_department() < b.get_department(); });
}
