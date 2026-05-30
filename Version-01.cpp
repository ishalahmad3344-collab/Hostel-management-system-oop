// Hostel Management System
#include <iostream>
#include <vector>
#include <string>

using namespace std;

// Class representing a Hostel Room
class Room {
private:
    int roomNumber;
    int capacity;
    int currentStudents;

public:
    // Simple constructor to initialize room details
    Room(int num, int cap) {
        roomNumber = num;
        capacity = cap;
        currentStudents = 0; // Starts empty
    }

    int getRoomNumber() const { return roomNumber; }
    
    bool isAvailable() const { 
        return currentStudents < capacity; 
    }

    void allocateBed() {
        if (isAvailable()) {
            currentStudents++;
        }
    }

    void displayRoom() const {
        cout << "Room #" << roomNumber 
             << " | Capacity: " << capacity 
             << " | Occupied: " << currentStudents << "/" << capacity << "\n";
    }
};

// Class representing a Student occupant
class Student {
private:
    int studentId;
    string name;
    int assignedRoomNumber;

public:
    Student(int id, string studentName) {
        studentId = id;
        name = studentName;
        assignedRoomNumber = -1; // -1 means no room assigned yet
    }

    int getId() const { return studentId; }
    string getName() const { return name; }
    int getRoomNumber() const { return assignedRoomNumber; }

    void assignRoom(int roomNum) {
        assignedRoomNumber = roomNum;
    }

    void displayStudent() const {
        cout << "ID: " << studentId 
             << " | Name: " << name;
        if (assignedRoomNumber == -1) {
            cout << " | Room: Not Assigned\n";
        } else {
            cout << " | Room: " << assignedRoomNumber << "\n";
        }
    }
};

// Main execution block containing the application dashboard structure
int main() {
    // Temporary storage vectors for runtime testing
    vector<Room> rooms;
    vector<Student> students;

    // Seed some initial data for testing execution paths
    rooms.push_back(Room(101, 2));
    rooms.push_back(Room(102, 3));

    int choice;
    do {
        cout << "\n==================================\n";
        cout << "   HOSTEL MANAGEMENT SYSTEM   \n";
        cout << "==================================\n";
        cout << "1. Display All Rooms\n";
        cout << "2. Register New Student\n";
        cout << "3. Allocate Room to Student\n";
        cout << "4. Display All Students\n";
        cout << "5. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
            case 1: {
                cout << "\n--- Current Room Profiles ---\n";
                for (const auto& room : rooms) {
                    room.displayRoom();
                }
                break;
            }
            case 2: {
                int id;
                string name;
                cout << "\nEnter Student ID (Integer): ";
                cin >> id;
                cin.ignore(); // Clean leftover newline from buffer
                cout << "Enter Student Name: ";
                getline(cin, name);

                students.push_back(Student(id, name));
                cout << "Student account created successfully!\n";
                break;
            }
            case 3: {
                int id, roomNum;
                cout << "\nEnter Student ID to update: ";
                cin >> id;
                cout << "Enter target Room Number: ";
                cin >> roomNum;

                // Simple search iteration
                bool studentFound = false;
                bool roomFound = false;

                for (auto& r : rooms) {
                    if (r.getRoomNumber() == roomNum) {
                        roomFound = true;
                        if (!r.isAvailable()) {
                            cout << "Operation failed. Target room is full.\n";
                            break;
                        }
                        
                        // Look for matching student record
                        for (auto& s : students) {
                            if (s.getId() == id) {
                                studentFound = true;
                                s.assignRoom(roomNum);
                                r.allocateBed();
                                cout << "Allocation completed successfully!\n";
                                break;
                            }
                        }
                    }
                }

                if (!roomFound) cout << "Error: Room does not exist.\n";
                else if (roomFound && !studentFound) cout << "Error: Student ID not found.\n";
                break;
            }
            case 4: {
                cout << "\n--- Registered Student Profiles ---\n";
                if (students.empty()) {
                    cout << "No active student records found.\n";
                } else {
                    for (const auto& student : students) {
                        student.displayStudent();
                    }
                }
                break;
            }
            case 5:
                cout << "\nExiting system core. Thank you!\n";
                break;
            default:
                cout << "Invalid selection. Please retry operational input.\n";
        }
    } while (choice != 5);

    return 0;
}
