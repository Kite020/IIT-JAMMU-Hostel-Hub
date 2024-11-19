#include <iostream>
#include <string>
#include <map>
#include <vector>

using namespace std;

class Student {
public:
    string name;
    string gender;
    int year;       // Year of study (1, 2, 3, 4)
    string branch;  // Branch of study (CSE, EE, MNC, CIVIL, MECHANICAL, MATERIALS, CHEMICAL)
    string hostel;  // Allocated hostel
    int floor;      // Allocated floor
    int room;       // Allocated room
    bool pairedWithRoommate; // If the student has been paired with a roommate

    Student(string n, string g, int y, string b) {
        name = n;
        gender = g;
        year = y;
        branch = b;
        hostel = "";
        floor = 0;
        room = 0;
        pairedWithRoommate = false;
    }
};

class HostelAllocator {
    vector<Student> students;
    map<string, vector<Student> > branchStudents; // Map to hold students by branch
    map<string, int> boysFloorTracker;           // Keeps track of room allocation per floor for boys
    map<string, int> girlsFloorTracker;          // Keeps track of room allocation per floor for girls

public:
    HostelAllocator() {
        // Initialize room trackers for boys and girls (room 1 starts at each floor)
        boysFloorTracker["Dedhar"] = 101;  // Dedhar for 1st year boys
        boysFloorTracker["Braeg"] = 101;   // Braeg for 2nd year boys
        boysFloorTracker["Fulgar"] = 101;  // Fulgar for 3rd year boys
        girlsFloorTracker["Egret"] = 101;  // Egret for girls
    }

    void addStudent() {
        string name, gender, branch;
        int year;

        cin.ignore(); // To flush input buffer
        cout << "\nEnter Student Name: ";
        getline(cin, name);
        cout << "Enter Gender (Male/Female): ";
        cin >> gender;
        cout << "Enter Year of Study (1st, 2nd, 3rd, 4th): ";
        cin >> year;
        cout << "Enter Branch (CSE, EE, MNC, CIVIL, MECHANICAL, MATERIALS, CHEMICAL): ";
        cin >> branch;

        students.push_back(Student(name, gender, year, branch));

        // Store student by branch for roommate pairing later
        branchStudents[branch].push_back(students.back());
    }

    void allocateHostel() {
        for (size_t i = 0; i < students.size(); ++i) {
            if (students[i].gender == "Female" || students[i].gender == "female") {
                allocateGirlsHostel(students[i]);
            } else if (students[i].gender == "Male" || students[i].gender == "male") {
                allocateBoysHostel(students[i]);
            } else {
                cout << "Invalid gender for student: " << students[i].name << endl;
            }
        }
    }

    void allocateGirlsHostel(Student &student) {
        student.hostel = "Egret (1-B)";
        student.floor = student.year; // Floors based on year
        student.room = girlsFloorTracker[student.hostel]++;
        pairRoommates(student);
    }

    void allocateBoysHostel(Student &student) {
        if (student.year == 1) {
            student.hostel = "Dedhar (1-A)";
            student.floor = 1; // First floor for 1st year
            student.room = boysFloorTracker[student.hostel]++;
        } else if (student.year == 2) {
            student.hostel = "Braeg (1-B)";
            student.floor = 2; // Second floor for 2nd year
            student.room = boysFloorTracker[student.hostel]++;
        } else if (student.year == 3 || student.year == 4) {
            student.hostel = "Fulgar (1-C)";
            student.floor = student.year == 3 ? 3 : 4; // Third floor for 3rd year and Fourth floor for 4th year
            student.room = boysFloorTracker[student.hostel]++;
        }

        pairRoommates(student);
    }

    void pairRoommates(Student &student) {
        // Pair students from the same branch for room allocation (in Dedhar, Braeg, Egret)
        if ((student.hostel == "Egret (1-B)" || student.hostel == "Dedhar (1-A)" || student.hostel == "Braeg (1-B)") && !student.pairedWithRoommate) {
            // Try to find a roommate from the same branch
            for (size_t i = 0; i < branchStudents[student.branch].size(); ++i) {
                if (!branchStudents[student.branch][i].pairedWithRoommate && branchStudents[student.branch][i].name != student.name) {
                    // Pair the students
                    branchStudents[student.branch][i].pairedWithRoommate = true;
                    student.pairedWithRoommate = true;
                    student.room = branchStudents[student.branch][i].room; // Assign same room
                    cout << "\nRoommates Assigned: " << student.name << " and " << branchStudents[student.branch][i].name << " (Branch: " << student.branch << ")\n";
                    break;
                }
            }
        }
    }

    void displayAllocations() {
        cout << "\nHostel Allocations:\n";
        for (size_t i = 0; i < students.size(); ++i) {
            cout << "\nName: " << students[i].name
                 << "\nGender: " << students[i].gender
                 << "\nYear: " << students[i].year
                 << "\nBranch: " << students[i].branch
                 << "\nHostel: " << students[i].hostel
                 << "\nFloor: " << students[i].floor
                 << "\nRoom: " << students[i].room
                 << "\n-----------------------------------";
        }
    }
};

int main() {
    HostelAllocator allocator;
    int choice;

    do {
        cout << "\n================= Hostel Room Allocation System =================\n";
        cout << "1. Add Student\n";
        cout << "2. Allocate Hostel Rooms\n";
        cout << "3. Display Allocations\n";
        cout << "4. Exit\n";
        cout << "==================================================================\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
            case 1:
                allocator.addStudent();
                break;
            case 2:
                allocator.allocateHostel();
                break;
            case 3:
                allocator.displayAllocations();
                break;
            case 4:
                cout << "Exiting... Goodbye!\n";
                break;
            default:
                cout << "Invalid choice. Try again.\n";
        }
    } while (choice != 4);

    return 0;
}
