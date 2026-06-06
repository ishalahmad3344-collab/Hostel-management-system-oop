#include <iostream>
#include <vector>
#include <string>
#include <stdexcept> 
using namespace std;

class HostelException : public runtime_error {
public:
    explicit HostelException(const string& message) : runtime_error(message) {}
};

class Room {
private:
    int roomNumber;
    int capacity;
    int currentStudents;

public:     // Constructure
    Room(int num, int cap) {
        roomNumber = num;
        capacity = cap;
        currentStudents = 0;
    }
    int getRoomNumber() const { return roomNumber; }
    int getCapacity() const { return capacity; }
    int getCurrentStudents() const { return currentStudents; }
    bool isAvailable() const { 
        return currentStudents < capacity; 
    }
    void allocateBed() {
        if (!isAvailable()) { // Error if rooms are fill
            throw HostelException("Room " + to_string(roomNumber) + " is already full!");
        }
        currentStudents++;
    }

    void displayRoom() const {
        cout << "Room Number: " << roomNumber 
             << " | Max Capacity: " << capacity 
             << " | Students Inside: " << currentStudents << "/" << capacity << "\n";
    }
};
class Student {
private:
    int studentId;
    string name;
    int assignedRoomNumber;

public:
    Student(int id, string studentName) {
        studentId = id;
        name = studentName;
        assignedRoomNumber = -1; // -1 means no room is given yet
    }
    int getId() const { return studentId; }
    string getName() const { return name; }
    int getRoomNumber() const { return assignedRoomNumber; }
    void assignRoom(int roomNum) {
        assignedRoomNumber = roomNum;
    }
    void displayStudent() const {
        cout << "ID: " << studentId << " | Name: " << name;
        if (assignedRoomNumber == -1) {
            cout << " | Room: Not Assigned Yet\n";
        } else {
            cout << " | Assigned Room: " << assignedRoomNumber << "\n";
        }
    }
};
class HostelSystem {
private:
    vector<Room> rooms;
    vector<Student> students;

public:
    void addRoom(Room r) { 
        rooms.push_back(r); 
    }
    void registerStudent(Student s) { 
        students.push_back(s); 
    }
    Student* searchStudent(int id) {
        for (size_t i = 0; i < students.size(); i++) {
            if (students[i].getId() == id) {
                return &students[i]; // Returns a pointer to the student found
            }
        }
        throw HostelException("Error: No student found with ID " + to_string(id));
    }
    Student* searchStudent(string studentName) {
        for (size_t i = 0; i < students.size(); i++) {
            if (students[i].getName() == studentName) {
                return &students[i]; // Returns a pointer to the student found
            }
        }
        throw HostelException("Error: No student found with name \"" + studentName + "\"");
    }
    Room* searchRoom(int roomNum) {
        for (size_t i = 0; i < rooms.size(); i++) {
            if (rooms[i].getRoomNumber() == roomNum) {
                return &rooms[i]; // Returns a pointer to the room found
            }
        }
        throw HostelException("Error: Room " + to_string(roomNum) + " does not exist.");
    }
    void allocateRoom(int studentId, int roomNum) {
        Student* s = searchStudent(studentId);
        Room* r = searchRoom(roomNum);
        r->allocateBed();
        s->assignRoom(roomNum);
        
        cout << "Success: " << s->getName() << " is now inside Room " << roomNum << ".\n";
    }  // Displays all rooms in the system
    void displayAllRooms() const {
        for (size_t i = 0; i < rooms.size(); i++) {
            rooms[i].displayRoom();
        }
    }  // Displays all students in the system
    void displayAllStudents() const {
        for (size_t i = 0; i < students.size(); i++) {
            students[i].displayStudent();
        }
    }
};

int main() {
    HostelSystem hostel;   // Add some sample data for testing
    hostel.addRoom(Room(101, 2)); // Room 101 can hold 2 students
    hostel.addRoom(Room(102, 1)); // Room 102 can hold only 1 student

    int choice;
    do {
        cout << "\n==================================\n";
        cout << "   HOSTEL MANAGEMENT SYSTEM \n";
        cout << "==================================\n";
        cout << "1. Display All Rooms\n";
        cout << "2. Register New Student\n";
        cout << "3. Allocate Room to Student\n";
        cout << "4. Display All Students\n";
        cout << "5. Search Student Utility\n";
        cout << "6. Exit\n";
        cout << "Enter your choice (1-6): ";
        cin >> choice;  // Try block to watch for any errors
        try {
            switch (choice) {
                case 1:
                    cout << "\n--- Current Rooms ---\n";
                    hostel.displayAllRooms();
                    break;

                case 2: {
                    int id;
                    string name;
                    cout << "\nEnter Student ID (Numbers only): ";
                    cin >> id;
                    cin.ignore(); // Clears the buffer
                    cout << "Enter Student Name: ";
                    getline(cin, name);

                    hostel.registerStudent(Student(id, name));
                    cout << "Student added successfully!\n";
                    break;
                }
                case 3: {
                    int id, roomNum;
                    cout << "\nEnter Student ID: ";
                    cin >> id;
                    cout << "Enter Room Number: ";
                    cin >> roomNum;

                    hostel.allocateRoom(id, roomNum);
                    break;
                }
                case 4:
                    cout << "\n--- Registered Students ---\n";
                    hostel.displayAllStudents();
                    break;

                case 5: {
                    int searchType;
                    cout << "\nHow do you want to search?\n1. By ID\n2. By Name\nChoice: ";
                    cin >> searchType;
                    cin.ignore();

                    if (searchType == 1) {
                        int id;
                        cout << "Enter ID to search: ";
                        cin >> id;
                        Student* s = hostel.searchStudent(id); // Calls the integer ID search function
                        cout << "Found: "; s->displayStudent();
                    } else if (searchType == 2) {
                        string name;
                        cout << "Enter Name to search: ";
                        getline(cin, name);
                        Student* s = hostel.searchStudent(name); // Calls the string Name search function
                        cout << "Found: "; s->displayStudent();
                    } else {
                        cout << "Invalid choice selection.\n";
                    }
                    break;
                }
                case 6:
                    cout << "\nClosing the application. Goodbye!\n";
                    break;

                default:
                    cout << "Invalid option. Please type a number from 1 to 6.\n";
            }
        }    // Catch block to capture and print our custom hostel errors smoothly
        catch (const HostelException& error) {
            cout << "\n[Hostel System Alert]: " << error.what() << "\n";
        }     // General catch block for anything else
        catch (const exception& error) {
            cout << "\n[System Alert]: " << error.what() << "\n";
        }

    } while (choice != 6);

    return 0;
}
