#include <iostream>   // Used for input and output operations (cout, cin)
#include <vector>    // Used for dynamic array vectors to store data lists
#include <string>    // Used to work with text string data smoothly
#include <memory>    // Used for smart pointer features (unique_ptr, make_unique)
#include <fstream>   // Used for File Handling streams (ifstream to read, ofstream to write files)
#include <sstream>   // Used for stringstream parsing to divide file text easily
#include <stdexcept> // Used for basic system runtime error primitives
using namespace std; // Avoids writing std:: before every line of standard code

// CUSTOM ERROR CLASS FOR SYSTEM SAFET
class HostelException : public runtime_error {
public:  // Takes a message string and forwards it safely to the runtime_error system
    explicit HostelException(const string& message) : runtime_error(message) {}
};

// GENERIC TEMPLATE CLASS (ACTS AS A VERSATILE STORAGE CONTAINER)
template <typename T>
class Repository {
private:
    vector<T> dataList; // A generic vector that can store any data type (T)
public:   // Function to add a new element item into our template array list
    void add(T item) { 
        dataList.push_back(move(item)); 
    }
       // Function to return the current total count of items in the container
    size_t size() const { 
        return dataList.size(); 
    }
      // Function that gives us reading/writing access to an item at a specific index
    T& getAt(size_t index) { 
        return dataList[index]; 
    } 
    // Constant function version to read-only items at a specific index location
    const T& getAt(size_t index) const { 
        return dataList[index]; 
    } 
    // Function to completely clear all data items inside this repository vector
    void clearAll() {
        dataList.clear();
    }
};

// ABSTRACT BASE PARENT CLASS (INHERITANCE STRUCTURING)
class Person {
protected:
    int id;          // Stores the unique ID number of a person
    string name;     // Stores the alphabetic name of a person
public:   // Simple constructor to initialize core fields for any type of person
    Person(int personId, string personName) : id(personId), name(personName) {}
      // Virtual destructor guarantees child elements are wiped cleanly from RAM memory
    virtual ~Person() = default;   // Getter functions to let external systems read our private/protected properties
    int getId() const { return id; }
    string getName() const { return name; }
    // Operator Overloading: Allows checking matching values easily via (person == id)
    bool operator==(int searchId) const {
        return this->id == searchId;
    }
    // Pure virtual functions that force child classes to build custom rules later
    virtual string getRole() const = 0;   // Returns user type label
    virtual void display() const = 0;     // Prints specific log layout
    virtual string serialize() const = 0; // Converts object properties to data string
};

// DERIVED CHILD CLASS: STUDENT (INHERITS FROM PERSON)
class Student : public Person {
private:
    int roomNumber; // Stores the room number assigned (-1 means unassigned)

public:   // Constructor uses parent setup rules and initiates the room number as empty (-1)
    Student(int id, string name) : Person(id, name), roomNumber(-1) {}
    // Getter and setter functions for the student room numbers
    int getRoomNumber() const { return roomNumber; }
    void assignRoom(int roomNum) { roomNumber = roomNum; }
    // Overriding the pure virtual functions with specific student actions
    string getRole() const override { return "Student"; }
    void display() const override {
        cout << "[Student Log] ID: " << id << " | Name: " << name;
        if (roomNumber == -1) cout << " | Status: Awaiting Room Assignment\n";
        else cout << " | Assigned Room: " << roomNumber << "\n";
    }
    // Formats student object properties into a neat text layout to save inside files
    string serialize() const override {
        return "STUDENT," + to_string(id) + "," + name + "," + to_string(roomNumber);
    }
};

// ROOM MODEL ENTITY CLASS
class Room {
private:
    int roomNumber;      // Tracking number identification code
    int capacity;        // Max capacity threshold limits
    int currentStudents; // Active occupant counts counter

public:   // Basic constructor setup parameters
    Room(int num, int cap) : roomNumber(num), capacity(cap), currentStudents(0) {}
   // Reader getter functions
    int getRoomNumber() const { return roomNumber; }
    int getCapacity() const { return capacity; }
    int getCurrentStudents() const { return currentStudents; }
       // Sets exact count during file loading updates
    void setCurrentStudents(int count) { currentStudents = count; }
  // Checks availability status constraints
    bool isAvailable() const { return currentStudents < capacity; }
       // Operator Overloading shortcut to match rooms directly via (room == targetRoomNumber)
    bool operator==(int num) const {
        return this->roomNumber == num;
    }
    // Increments counter state with a safety barrier rule
    void allocateBed() {
        if (!isAvailable()) throw HostelException("Room Allocation Failed: Target room is full!");
        currentStudents++;
    }

    void displayRoom() const {
        cout << "Room #" << roomNumber << " | Occupancy Capacity Status: " << currentStudents << "/" << capacity << "\n";
    }
    // Formats room configurations into comma-separated text values for files
    string serialize() const {
        return "ROOM," + to_string(roomNumber) + "," + to_string(capacity) + "," + to_string(currentStudents);
    }
};

// CENTRAL CONTROLLER SYSTEM (FILE HANDLING INTEGRATION)
class HostelSystem {
private:
    Repository<Room> rooms;               // Using our generic template box to store rooms
    Repository<unique_ptr<Person>> people; // Using our generic template box to store polymorphic people

public:    // Insert commands to feed the underlying structures
    void addRoom(Room r) { rooms.add(r); }
    void addPerson(unique_ptr<Person> p) { people.add(move(p)); }
   // Loops and searches records using our overloaded equivalence identifier
    Person* searchPerson(int id) {
        for (size_t i = 0; i < people.size(); i++) {
            if (*people.getAt(i) == id) return people.getAt(i).get();
        }
        throw HostelException("Process Aborted: Checked User ID does not exist.");
    }
    // Loops and searches rooms using our overloaded equivalence identifier
    Room* searchRoom(int roomNum) {
        for (size_t i = 0; i < rooms.size(); i++) {
            if (rooms.getAt(i) == roomNum) return &rooms.getAt(i);
        }
        throw HostelException("Process Aborted: Checked Room Number does not exist.");
    }
    // Executes validation logic before routing assignments
    void allocateRoom(int studentId, int roomNum) {
        Student* s = dynamic_cast<Student*>(searchPerson(studentId));
        if (s == nullptr) throw HostelException("Security Rejection: Selected profile is not a valid student type.");        
        Room* r = searchRoom(roomNum);
        r->allocateBed();
        s->assignRoom(roomNum);
        cout << "Success Notification: Student room mapping finalized successfully!\n";
    }
    // Prints all data tracking tables cleanly
    void displayAll() const {
        cout << "\n========================================\n";
        cout << "      CURRENT HOSTEL SYSTEM LOGS       \n";
        cout << "========================================\n";
        for (size_t i = 0; i < rooms.size(); i++) rooms.getAt(i).displayRoom();
        cout << "----------------------------------------\n";
        if(people.size() == 0) cout << "No profiles currently registered.\n";
        for (size_t i = 0; i < people.size(); i++) people.getAt(i)->display();
        cout << "========================================\n";
    }

    // FILE STREAM CONVERSION MECHANICS (PERSISTENCE LAYER)
    // Writes all runtime data down into an external text document file
    void saveDataToFile() const {
        ofstream outFile("hostel_data.txt"); // Opens/Creates text file to write data
        if (!outFile) {
            cout << "Warning: Could not open data stream file to back up state records.\n";
            return;
        }
      // Loop and write room tracking logs down string by string
        for (size_t i = 0; i < rooms.size(); i++) {
            outFile << rooms.getAt(i).serialize() << "\n";
        }
              // Loop and write user tracking records down string by string
        for (size_t i = 0; i < people.size(); i++) {
            outFile << people.getAt(i)->serialize() << "\n";
        }
        outFile.close(); // Closes file channel cleanly
        cout << "[System Sync]: System current state saved into 'hostel_data.txt' successfully.\n";
    }
         // Reads records out from the external text document file to reconstruct state matrices
    void loadDataFromFile() {
        ifstream inFile("hostel_data.txt"); // Opens existing text file to read data
        if (!inFile) {
            // File not found is expected on first launch setup runs
            cout << "[System Notice]: Welcome! No matching data storage trace found. Starting fresh.\n";
            return;
        }
        // Wipe baseline seed variables to prevent appending multiple copies together
        rooms.clearAll();
        people.clearAll();
        string line;
        // Read file line-by-line until we reach the end of document
        while (getline(inFile, line)) {
            if (line.empty()) continue; // Skip random blank divider rows safely
            stringstream ss(line); // Turn row line text into a breakable string token stream
            string entityType;
            getline(ss, entityType, ','); // Isolate header row identifier prefix tag
            if (entityType == "ROOM") {
                string rNumStr, capStr, currStr;
                getline(ss, rNumStr, ',');
                getline(ss, capStr, ',');
                getline(ss, currStr, ',');
                // Convert file strings back to integers
                Room loadedRoom(stoi(rNumStr), stoi(capStr));
                loadedRoom.setCurrentStudents(stoi(currStr));
                rooms.add(loadedRoom); // Feed item back inside repositories
            } 
            else if (entityType == "STUDENT") {
                string idStr, name, rNumStr;
                getline(ss, idStr, ',');
                getline(ss, name, ',');
                getline(ss, rNumStr, ',');
       // Reconstruct student pointer variables
                auto loadedStudent = make_unique<Student>(stoi(idStr), name);
                loadedStudent->assignRoom(stoi(rNumStr));
                people.add(move(loadedStudent)); // Feed pointer back inside repositories
            }
        }
        inFile.close(); // Closes file channel cleanly
        cout << "[System Sync]: Data properties parsed and loaded from 'hostel_data.txt' successfully.\n";
    }
};

// MAIN MENU SYSTEM APPLICATION DASHBOARD
int main() {
    HostelSystem hostel;
    // Load old files right away before drawing the interaction view metrics
    hostel.loadDataFromFile();
   // Default room values if the file is blank or running for the very first time
    // System automatically overrides these defaults when files successfully load
    // hostel.addRoom(Room(101, 3)); 
    int choice;
    do {
        cout << "\n==================================\n";
        cout << "   HOSTEL MANAGEMENT SYSTEM  \n";
        cout << "==================================\n";
        cout << "1. Inject System Configuration Room\n";
        cout << "2. Register New Student Profile\n";
        cout << "3. Allocate Room to Student\n";
        cout << "4. Display All Records\n";
        cout << "5. Save and Close System\n";
        cout << "==================================\n";
        cout << "Enter selection selection: ";
        cin >> choice;

        try {
            switch (choice) {
                case 1: {
                    int roomNum, cap;
                    cout << "\nEnter New Room Number ID: "; cin >> roomNum;
                    cout << "Set Max Room Capacity Limits: "; cin >> cap;
                    hostel.addRoom(Room(roomNum, cap));
                    cout << "Configuration Update: New room loaded into volatile state logs.\n";
                    break;
                }

                    break;
                case 2: {
                    int id; string name;
                    cout << "\nEnter Student ID (Numbers only): "; cin >> id; cin.ignore();
                    cout << "Enter Student Full Name: "; getline(cin, name);
                    hostel.addPerson(make_unique<Student>(id, name));
                    cout << "Registration Confirmation: Profile registered successfully.\n";
                    break;
                }
                case 3: {
                    int id, roomNum;
                    cout << "\nEnter Student ID to modify: "; cin >> id;
                    cout << "Enter Target Room Allocation Number: "; cin >> roomNum;
                    hostel.allocateRoom(id, roomNum);
                    break;
                }
                case 4: 
                    hostel.displayAll();
                    break;
                case 5:                  // Save everything securely right before exiting runtime memory structures
                    hostel.saveDataToFile();
                    cout << "\nSystem Shutdown Process Complete. Session Ended.\n";
                    break;
                default:
                    cout << "Invalid Input Selection: Please submit an integer key choice from 1-5.\n";
            }
        } 
        catch (const HostelException& e) { 
            cout << "\n[System Operational Alert]: " << e.what() << "\n"; 
        }
        catch (const exception& e) { 
            cout << "\n[Fatal Core Crash Caught]: " << e.what() << "\n"; 
        }
    } while (choice != 5);

    return 0; // Terminate execution safely
}
