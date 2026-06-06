#include <iostream>
#include <vector>
#include <string>
#include <memory>
#include <stdexcept>
using namespace std;

class HostelException : public runtime_error {
public:
    explicit HostelException(const string& message) : runtime_error(message) {}
};

class Person {
protected:
    int id;
    string name;
    string contactNumber;

public:
    Person(int personId, string personName, string contact) 
        : id(personId), name(personName), contactNumber(contact) {}
    virtual ~Person() = default;
    int getId() const { return id; }
    string getName() const { return name; }
    string getContact() const { return contactNumber; }
    virtual string getRole() const = 0; 
    virtual void display() const = 0;   
};
class Student : public Person {
private:
    int assignedRoomNumber;
    bool feePaid;

public:
    Student(int id, string name, string contact) 
        : Person(id, name, contact), assignedRoomNumber(-1), feePaid(false) {}

    int getRoomNumber() const { return assignedRoomNumber; }
    bool isFeePaid() const { return feePaid; }

    void assignRoom(int roomNum) { assignedRoomNumber = roomNum; }
    void payFee() { feePaid = true; }

    string getRole() const override { 
        return "Student"; 
    }

    void display() const override {
        cout << "[" << getRole() << "] ID: " << id 
             << " | Name: " << name 
             << " | Phone: " << contactNumber;
        if (assignedRoomNumber == -1) {
            cout << " | Room: Unassigned";
        } else {
            cout << " | Room: " << assignedRoomNumber;
        }
        cout << " | Fees: " << (feePaid ? "Paid" : "Pending") << "\n";
    }
};
class Employee : public Person {
private:
    string jobTitle;
    double salary;

public:   // Constructor uses the parent constructor to setup common details
    Employee(int id, string name, string contact, string job, double sal) 
        : Person(id, name, contact), jobTitle(job), salary(sal) {}

    string getJobTitle() const { return jobTitle; }
    double getSalary() const { return salary; }
    string getRole() const override { 
        return "Employee"; 
    }

    void display() const override {
        cout << "[" << getRole() << "] ID: " << id 
             << " | Name: " << name 
             << " | Job: " << jobTitle 
             << " | Phone: " << contactNumber 
             << " | Salary: Rs." << salary << "\n";
    }
};

class Room {
private:
    int roomNumber;
    int capacity;
    int currentStudents;

public:
    Room(int num, int cap) : roomNumber(num), capacity(cap), currentStudents(0) {}

    int getRoomNumber() const { return roomNumber; }
    bool isAvailable() const { return currentStudents < capacity; }

    void allocateBed() {
        if (!isAvailable()) {
            throw HostelException("Room " + to_string(roomNumber) + " is already full!");
        }
        currentStudents++;
    }

    void displayRoom() const {
        cout << "Room Number: " << roomNumber 
             << " | Occupied Space: " << currentStudents << "/" << capacity << "\n";
    }
};

class HostelSystem {
private:
    vector<Room> rooms;
    vector<unique_ptr<Person>> people; 

public:
    void addRoom(Room r) { 
        rooms.push_back(r); 
    }
    void addPerson(unique_ptr<Person> p) {
        people.push_back(move(p));
    }
    Person* searchPerson(int id) {
        for (size_t i = 0; i < people.size(); i++) {
            if (people[i]->getId() == id) {
                return people[i].get();
            }
        }
        throw HostelException("Error: ID " + to_string(id) + " not found in the records.");
    }

    Room* searchRoom(int roomNum) {
        for (size_t i = 0; i < rooms.size(); i++) {
            if (rooms[i].getRoomNumber() == roomNum) {
                return &rooms[i];
            }
        }
        throw HostelException("Error: Room " + to_string(roomNum) + " does not exist.");
    }
    void allocateRoom(int studentId, int roomNum) {
        Person* p = searchPerson(studentId);
        Room* r = searchRoom(roomNum);
        Student* s = dynamic_cast<Student*>(p);
        if (s == nullptr) {
            throw HostelException("Error: Rooms can only be given to students, not employees!");
        }

        r->allocateBed();
        s->assignRoom(roomNum);
        
        cout << "Success: Room " << roomNum << " given to student " << s->getName() << ".\n";
    }

    void displayAllRooms() const {
        for (size_t i = 0; i < rooms.size(); i++) {
            rooms[i].displayRoom();
        }
    }
    void displayAllPeople() const {
        for (size_t i = 0; i < people.size(); i++) {
            people[i]->display(); 
        }
    }
};

int main() {
    HostelSystem hostel;
    hostel.addRoom(Room(101, 2));
    hostel.addRoom(Room(102, 1));

    int choice;
    do {
        cout << "\n==================================\n";
        cout << "   HOSTEL MANAGEMENT SYSTEM   \n";
        cout << "==================================\n";
        cout << "1. Display All Rooms\n";
        cout << "2. Register a New Student\n";
        cout << "3. Register a New Employee\n";
        cout << "4. Allocate Room to Student\n";
        cout << "5. View All Registered Profiles\n";
        cout << "6. Record Student Fee Payment\n";
        cout << "7. Exit\n";
        cout << "Enter your choice (1-7): ";
        cin >> choice;

        try {
            switch (choice) {
                case 1:
                    cout << "\n--- Current Rooms ---\n";
                    hostel.displayAllRooms();
                    break;

                case 2: {
                    int id;
                    string name, contact;
                    cout << "\nEnter Student ID (Numbers): ";
                    cin >> id;
                    cin.ignore();
                    cout << "Enter Student Name: ";
                    getline(cin, name);
                    cout << "Enter Contact Number: ";
                    getline(cin, contact);
                    hostel.addPerson(make_unique<Student>(id, name, contact));
                    cout << "Student profile registered successfully!\n";
                    break;
                }
                case 3: {
                    int id;
                    string name, contact, job;
                    double salary;
                    cout << "\nEnter Employee ID (Numbers): ";
                    cin >> id;
                    cin.ignore();
                    cout << "Enter Employee Name: ";
                    getline(cin, name);
                    cout << "Enter Contact Number: ";
                    getline(cin, contact);
                    cout << "Enter Job Title (e.g., Warden, Cleaner): ";
                    getline(cin, job);
                    cout << "Enter Salary: ";
                    cin >> salary;

                    hostel.addPerson(make_unique<Employee>(id, name, contact, job, salary));
                    cout << "Employee profile registered successfully!\n";
                    break;
                }
                case 4: {
                    int id, roomNum;
                    cout << "\nEnter Student ID: ";
                    cin >> id;
                    cout << "Enter Room Number: ";
                    cin >> roomNum;

                    hostel.allocateRoom(id, roomNum);
                    break;
                }
                case 5:
                    cout << "\n--- Registered Users (Polymorphic List) ---\n";
                    hostel.displayAllPeople();
                    break;

                case 6: {
                    int id;
                    cout << "\nEnter Student ID to pay fees: ";
                    cin >> id;

                    Person* p = hostel.searchPerson(id);
                    Student* s = dynamic_cast<Student*>(p);
                    
                    if (s == nullptr) {
                        throw HostelException("Error: Selected ID belongs to an employee. Only students pay fees!");
                    }
                    
                    s->payFee();
                    cout << "Success: Fees marked as paid for student " << s->getName() << ".\n";
                    break;
                }
                case 7:
                    cout << "\nClosing the application. Goodbye!\n";
                    break;

                default:
                    cout << "Invalid option. Please type a number from 1 to 7.\n";
            }
        } 
        catch (const HostelException& error) {
            cout << "\n[Hostel System Alert]: " << error.what() << "\n";
        }
        catch (const exception& error) {
            cout << "\n[System Error]: " << error.what() << "\n";
        }

    } while (choice != 7);

    return 0;
}
