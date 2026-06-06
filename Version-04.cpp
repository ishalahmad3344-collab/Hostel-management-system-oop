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
template <typename T>
class Repository {
private:
    vector<T> dataList;
public:
    void add(T item) { 
        dataList.push_back(move(item)); 
    }
    
    size_t size() const { 
        return dataList.size(); 
    }
    T& getAt(size_t index) { 
        return dataList[index]; 
    }
    
    const T& getAt(size_t index) const { 
        return dataList[index]; 
    }
};

class Person {
protected:
    int id;
    string name;
public:
    Person(int personId, string personName) : id(personId), name(personName) {}
    virtual ~Person() = default;
    int getId() const { return id; }
    string getName() const { return name; }
    bool operator==(int searchId) const {
        return this->id == searchId;
    }
    virtual void display() const = 0;   
};
class Student : public Person {
private:
    int roomNumber;
public:
    Student(int id, string name) : Person(id, name), roomNumber(-1) {}
    void assignRoom(int roomNum) { roomNumber = roomNum; }
    void display() const override {
        cout << "[Student] ID: " << id << " | Name: " << name;
        if (roomNumber == -1) cout << " | Room: Unassigned\n";
        else cout << " | Room: " << roomNumber << "\n";
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
    bool operator==(int num) const {
        return this->roomNumber == num;
    }
    void allocateBed() {
        if (!isAvailable()) throw HostelException("Room is full!");
        currentStudents++;
    }
    void displayRoom() const {
        cout << "Room Number: " << roomNumber << " | Occupied: " << currentStudents << "/" << capacity << "\n";
    }
};
class HostelSystem {
private:   // Using our template repository instead of plain vectors
    Repository<Room> rooms;
    Repository<unique_ptr<Person>> people;
public:
    void addRoom(Room r) { rooms.add(r); }
    void addPerson(unique_ptr<Person> p) { people.add(move(p)); }
    Person* searchPerson(int id) {
        for (size_t i = 0; i < people.size(); i++) {
            if (*people.getAt(i) == id) {
                return people.getAt(i).get();
            }
        }
        throw HostelException("Error: Person ID not found.");
    }
    Room* searchRoom(int roomNum) {
        for (size_t i = 0; i < rooms.size(); i++) {
            if (rooms.getAt(i) == roomNum) {
                return &rooms.getAt(i);
            }
        }
        throw HostelException("Error: Room not found.");
    }
    void allocateRoom(int studentId, int roomNum) {
        Student* s = dynamic_cast<Student*>(searchPerson(studentId));
        if (s == nullptr) throw HostelException("Error: Not a student profile.");     
        Room* r = searchRoom(roomNum);
        r->allocateBed();
        s->assignRoom(roomNum);
        cout << "Success: Room allocated successfully!\n";
    }
    void displayAll() const {
        cout << "\n--- Rooms Log ---\n";
        for (size_t i = 0; i < rooms.size(); i++) rooms.getAt(i).displayRoom();
        cout << "\n--- People Log ---\n";
        for (size_t i = 0; i < people.size(); i++) people.getAt(i)->display();
    }
};

int main() {
    HostelSystem hostel;
    hostel.addRoom(Room(101, 2));

    int choice;
    do {
        cout << "\n==================================\n";
        cout << "   HOSTEL MANAGEMENT SYSTEM  \n";
        cout << "==================================\n";
        cout << "1. Display All Records\n";
        cout << "2. Register Student\n";
        cout << "3. Allocate Room\n";
        cout << "4. Exit\n";
        cout << "Choice: ";
        cin >> choice;
        try {
            switch (choice) {
                case 1:
                    hostel.displayAll();
                    break;
                case 2: {
                    int id; string name;
                    cout << "Enter ID: "; cin >> id; cin.ignore();
                    cout << "Enter Name: "; getline(cin, name);
                    hostel.addPerson(make_unique<Student>(id, name));
                    cout << "Student Registered!\n";
                    break;
                }
                case 3: {
                    int id, roomNum;
                    cout << "Enter Student ID: "; cin >> id;
                    cout << "Enter Room Number: "; cin >> roomNum;
                    hostel.allocateRoom(id, roomNum);
                    break;
                }
                case 4:
                    cout << "Goodbye!\n";
                    break;
                default:
                    cout << "Invalid Option.\n";
            }
        } 
        catch (const HostelException& e) { cout << "\n[Alert]: " << e.what() << "\n"; }
    } while (choice != 4);

    return 0;
}
