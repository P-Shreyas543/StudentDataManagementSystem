#include <iostream>
#include <vector>
#include <algorithm> 
#include <fstream> 
using namespace std;

class college {
    string name, add;

public:
    college(){
	 name = "";
	 add = "";
    }

    college(string n, string a){
	 name = n; 
	 add = a;
    }

    college(const college &obj){
	 name = obj.name;
	 add = obj.add;
    }

    void display() const {
        cout << "College name: " << name << endl;
        cout << "Address: " << add << endl;
    }

    void setName(const string &n) {
	 name = n;
    }
    void setAddress(const string &a) {
	 add = a;
    }
    string getName() const {
	 return name;
    }
    string getAddress() const {
	 return add;
    }

    void saveToFile(ofstream &file) const {
        file << name << endl << add << endl;
    }

    void loadFromFile(ifstream &file) {
        getline(file, name);
        getline(file, add);
    }

    ~college() {
    }
};

class department : public college {
    string name;
    int staff;

public:
    department() : college() {
	 name = "";
	 staff = 0; 
    }

    department(string n, int s1, string n1, string a) : college(n1, a){
	 name = n; 
	staff = s1;
    }

    department(const department &obj) : college(obj){
	 name = obj.name; 
	staff = obj.staff; 
    }

    void display() const {
        cout << "Department name: " << name << endl;
        cout << "Number of staff: " << staff << endl;
        college::display();
    }

    void setDepartmentName(const string &n) {
	 name = n;
    }
    void setStaff(int s) {
	 staff = s;
    }
    string getDepartmentName() const {
	 return name;
    }
    int getStaff() const {
  	 return staff;
    }

    void saveToFile(ofstream &file) const {
        college::saveToFile(file);
        file << name << endl << staff << endl;
    }

    void loadFromFile(ifstream &file) {
        college::loadFromFile(file);
        getline(file, name);
        file >> staff;
        file.ignore();
    }

    ~department() {
    }
};

class student : public department {
    string name;
    int roll;

public:
    student() : department(), name(""), roll(0) {}

    student(string n1, int r, string n2, int s, string n3, string a) 
        : department(n2, s, n3, a), name(n1), roll(r) {}

    student(const student &obj) : department(obj), name(obj.name), roll(obj.roll) {}

    void display() const {
        cout << "Student name: " << name << endl;
        cout << "Roll no: " << roll << endl;
        department::display();
    }

    int getRoll() const { 
	return roll;
     }

    void editDetails(string newName, int newRoll, string newDeptName, int newStaff, string newCollegeName, string newAddress) {
        name = newName;
        roll = newRoll;
        department::setDepartmentName(newDeptName);
        department::setStaff(newStaff);
        college::setName(newCollegeName);
        college::setAddress(newAddress);
    }

    void saveToFile(ofstream &file) const {
        department::saveToFile(file);
        file << name << endl << roll << endl;
    }

    void loadFromFile(ifstream &file) {
        department::loadFromFile(file);
        getline(file, name);
        file >> roll;
        file.ignore();
    }

    ~student() {
    }
};

void menu() {
    cout << "1. Enter the details" << endl;
    cout << "2. Display" << endl;
    cout << "3. Edit" << endl;
    cout << "4. Delete" << endl;
    cout << "5. Save to file" << endl;
    cout << "6. Load from file" << endl;
    cout << "0. Exit" << endl;
}

void saveToFile(const vector<student> &students) {
    ofstream file("students_data.txt");
    if (!file) {
        cerr << "Error opening file for writing." << endl;
        return;
    }

    if (!students.empty()) {
        const student &firstStudent = students.front();
        firstStudent.saveToFile(file);

        file << students.size() << endl; 

        for (const student &s : students) {
            s.saveToFile(file);
        }
    }

    file.close();
    cout << "Data saved to file." << endl;
}

void loadFromFile(vector<student> &students) {
    ifstream file("students_data.txt");
    if (!file) {
        cerr << "Error opening file for reading." << endl;
        return;
    }

    students.clear();

    student tempStudent;
    tempStudent.loadFromFile(file);

    int numStudents;
    file >> numStudents;
    file.ignore();

    for (int i = 0; i < numStudents; ++i) {
        student s;
        s.loadFromFile(file);
        students.push_back(s);
    }

    file.close();
    cout << "Data loaded from file." << endl;
}

int main() {
    int opt;
    vector<student> students;
    string collegeName, collegeAddress, departmentName;
    int departmentStaff;

    while (true) {
        menu();
        cout << "Enter the option: ";
        cin >> opt;

        switch (opt) {
            case 1: {
                cout << "Enter the college name: ";
                cin.ignore();
                getline(cin, collegeName);

                cout << "Enter the college address: ";
                getline(cin, collegeAddress);

                cout << "Enter the department name: ";
                getline(cin, departmentName);

                cout << "Enter the number of staff in the department: ";
                cin >> departmentStaff;

                string studentName;
                int rollNumber;

                while (true) {
                    cout << "Enter the student name (or type 'done' to finish): ";
                    cin.ignore();
                    getline(cin, studentName);

                    if (studentName == "done") break;

                    cout << "Enter the Roll no: ";
                    cin >> rollNumber;

                    student s(studentName, rollNumber, departmentName, departmentStaff, collegeName, collegeAddress);
                    students.push_back(s);
                }
                break;
            }
            case 2: {
                for (const student &s : students) {
                    cout << "--------------------------" << endl;
                    s.display();
                    cout << "--------------------------" << endl;
                }
                break;
            }
            case 3: { 
                int rollNumber;
                cout << "Enter the roll number of the student to edit: ";
                cin >> rollNumber;

                auto it = find_if(students.begin(), students.end(), [rollNumber](const student &s) {
                    return s.getRoll() == rollNumber;
                });

                if (it != students.end()) {
                    string newStudentName, newDeptName, newCollegeName, newAddress;
                    int newRollNumber, newStaffCount;

                    cout << "Enter the new student name: ";
                    cin.ignore();
                    getline(cin, newStudentName);

                    cout << "Enter the new Roll no: ";
                    cin >> newRollNumber;

                    cout << "Enter the new department name: ";
                    cin.ignore();
                    getline(cin, newDeptName);

                    cout << "Enter the new number of staff: ";
                    cin >> newStaffCount;

                    cout << "Enter the new college name: ";
                    cin.ignore();
                    getline(cin, newCollegeName);

                    cout << "Enter the new college address: ";
                    getline(cin, newAddress);

                    it->editDetails(newStudentName, newRollNumber, newDeptName, newStaffCount, newCollegeName, newAddress);
                } else {
                    cout << "Student with roll number " << rollNumber << " not found." << endl;
                }
                break;
            }
            case 4: {
                int rollNumber;
                cout << "Enter the roll number of the student to delete: ";
                cin >> rollNumber;

                auto it = remove_if(students.begin(), students.end(), [rollNumber](const student &s) {
                    return s.getRoll() == rollNumber;
                });

                if (it != students.end()) {
                    students.erase(it, students.end());
                    cout << "Student with roll number " << rollNumber << " has been deleted." << endl;
                } else {
                    cout << "Student with roll number " << rollNumber << " not found." << endl;
                }
                break;
            }
            case 5:
                saveToFile(students);
                break;
            case 6:
                loadFromFile(students);
                break;
            case 0:
                return 0;
            default:
                cout << "Invalid option. Please try again." << endl;
                break;
        }
    }
}

