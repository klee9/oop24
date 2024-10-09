#include "student_database.h"
#include <iostream>

using namespace std;

// function to display the main menu
void display_main_menu() {
    cout << "1. Insertion" << endl;
    cout << "2. Search" << endl;
    cout << "3. Sorting Option" << endl;
    cout << "4. Exit" << endl;
}

// function to display the search menu
void display_search_menu() {
    cout << "- Search -" << endl;
    cout << "1. Search by name" << endl;
    cout << "2. Search by student ID (10 digits)" << endl;
    cout << "3. Search by admission year (4 digits)" << endl;
    cout << "4. Search by birth year (4 digits)" << endl;
    cout << "5. Search by department name" << endl;
    cout << "6. List All" << endl;
}

// function to display the sorting menu
void display_sorting_menu() {
    cout << "- Sorting Options -" << endl;
    cout << "1. Sort by name" << endl;
    cout << "2. Sort by student ID" << endl;
    cout << "3. Sort by birth year" << endl;
    cout << "4. Sort by department name" << endl;
}

// function to handle the insertion of a new student
void handle_insertion(StudentDatabase& db) {
    string name, student_id, department, tel;
    int birth_year;

    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    
    do {
        cout << "Name: ";
        getline(cin, name);
        if(name.size() > 15 || name.empty())
            cout << "Name must be 1 - 15 characters. Try again." << endl;
    } while(name.size() > 15 || name.empty());
    
    do {
        cout << "Student ID (10 digits): ";
        getline(cin, student_id);
        if(student_id.size() != 10)
            cout << "Student ID must be 10 characters long. Try again." << endl;
    } while(student_id.size() != 10);
    
    do {
        cout << "Birth Year (4 digits): ";
        cin >> birth_year;
        if(to_string(birth_year).size() != 4)
            cout << "Birth year must be 4 characters long. Try again." << endl;
    } while(to_string(birth_year).size() != 4);
    
    // for ignoring leftover '\n' in the buffer
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    
    cout << "Department: ";
    getline(cin, department);
    
    do {
        cout << "Tel: ";
        cin >> tel;
        if(tel.size() > 12)
            cout << "Phone number must be less than 13 characters. Try again." << endl;
    } while(tel.size() > 12);
    
    Student new_student(name, student_id, birth_year, department, tel);
    db.add_student(new_student);
}

// function to handle searching
void handle_search(StudentDatabase& db) {
    display_search_menu();
    int choice;
    cout << "> ";
    cin >> choice;
    
    switch (choice) {
    case 1: {
        string name;
        cout << "Enter name: ";
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        getline(cin, name);
        auto results = db.search_by_name(name);
        for (const auto& student : results) cout << student << endl;
        break;
    }
    case 2: {
        string student_id;
        cout << "Enter student ID (10 digits): ";
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        getline(cin, student_id);
        auto results = db.search_by_student_id(student_id);
        for (const auto& student : results) cout << student << endl;
        break;
    }
    case 3: {
        int year;
        cout << "Enter admission year (4 digits): ";
        cin >> year;
        auto results = db.search_by_admission_year(year);
        for (const auto& student : results) cout << student << endl;
        break;
    }
    case 4: {
        int year;
        cout << "Enter birth year (4 digits): ";
        cin >> year;
        auto results = db.search_by_birth_year(year);
        for (const auto& student : results) cout << student << endl;
        break;
    }
    case 5: {
        string department;
        cout << "Enter department: ";
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        getline(cin, department);
        auto results = db.search_by_department(department);
        for (const auto& student : results) cout << student << endl;
        break;
    }
    case 6: {
        auto results = db.list_all_students();
        for (const auto& student : results) cout << student << endl;
        break;
    }
    default:
        cout << "Invalid option. Returning to main menu." << endl;
    }
}

// function to handle sorting
void handle_sorting(StudentDatabase& db) {
    display_sorting_menu();
    int choice;
    cout << "> ";
    cin >> choice;

    switch (choice) {
    case 1:
        db.sort_by_name();
        break;
    case 2:
        db.sort_by_student_id();
        break;
    case 3:
        db.sort_by_birth_year();
        break;
    case 4:
        db.sort_by_department();
        break;
    default:
        cout << "Invalid option. Returning to main menu." << endl;
        return;
    }

    auto students = db.list_all_students();
    for (const auto& student : students) cout << student << endl;
}

int main(int argc, char *argv[]) {
    // if file1.txt is not given, exit
    if (argc < 2) {
        cout << "Usage: " << argv[0] << " file1.txt" << endl;
        return 1;
    }

    string filename = argv[1];
    StudentDatabase db(filename);

    int choice = 0;
    do {
        display_main_menu();
        cout << "> ";
        cin >> choice;
        switch (choice) {
        case 1: // insertion
            handle_insertion(db);
            break;
        case 2: // search
            handle_search(db);
            break;
        case 3: // sort
            handle_sorting(db);
            break;
        case 4: // exit
            cout << "Exiting..." << endl;
            break;
        default:
            cout << "Invalid option. Please try again." << endl;
        }
    } while (choice != 4);

    return 0;
}
