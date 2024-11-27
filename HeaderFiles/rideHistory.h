#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
using namespace std;

// Node structure for each ride
struct RideNode {
    int id;              // Ride ID
    string time;         // Time of the ride
    string date;         // Date of the ride
    string source;       // Source location of the ride
    string destination;  // Destination location of the ride
    RideNode* prev;      // Pointer to the previous ride
    RideNode* next;      // Pointer to the next ride

    // Constructor
    RideNode(int id, string time, string date, string source, string destination) 
        : id(id), time(time), date(date), source(source), destination(destination), prev(nullptr), next(nullptr) {}
};

// Doubly linked list for user ride history
class RideHistory {
private:
    RideNode* head; // Pointer to the first ride in history
    RideNode* tail; // Pointer to the last ride in history

public:
    // Constructor
    RideHistory()   {
        head = nullptr;
        tail = nullptr;
        loadFromFile("Files\\RideHistory.txt");
    }

    // Add a new ride to the history
    void addRide(int id, string time, string date, string source, string destination) {
        RideNode* newRide = new RideNode(id, time, date, source, destination);

        // If the list is empty
        if (!head) {
            head = tail = newRide;
        } else {
            tail->next = newRide;
            newRide->prev = tail;
            tail = newRide;
        }
        //cout << "Ride added: ID=" << id << ", Time=" << time << ", Date=" << date 
        //     << ", From=" << source << ", To=" << destination << endl;
    }

    // Updated displayForward to filter by user ID
void displayForward(int userId) {
    if (!head) {
        cout << "No ride history available.\n";
        return;
    }

    RideNode* current = head;
    bool found = false;

    //cout << "Ride History for User ID: " << userId << " (Forward):\n";
    while (current) {
        if (current->id == userId) { // Check if ride belongs to the given user ID
           // cout << "ID: " << current->id 
            cout << "Time: " << current->time 
                 << ", Date: " << current->date 
                 << ", From: " << current->source 
                 << ", To: " << current->destination << endl;
            found = true;
        }
        current = current->next;
    }

    if (!found) {
        cout << "No ride history found for this user" << endl;
    }
}

// Updated displayBackward to filter by user ID
void displayBackward(int userId) {
    if (!tail) {
        cout << "No ride history available.\n";
        return;
    }

    RideNode* current = tail;
    bool found = false;

    //cout << "Ride History for User ID: " << userId << " (Backward):\n";
    while (current) {
        if (current->id == userId) { // Check if ride belongs to the given user ID
            //cout << "ID: " << current->id 
            cout << "Time: " << current->time 
                 << ", Date: " << current->date 
                 << ", From: " << current->source 
                 << ", To: " << current->destination << endl;
            found = true;
        }
        current = current->prev;
    }

    if (!found) {
        cout << "No ride history found for this user  " << endl;
    }
}

    // Save ride history to a file
    void saveToFile(const string& filename) {
        ofstream file(filename);

        if (!file) {
            cout << "Error opening file for writing.\n";
            return;
        }

        RideNode* current = head;
        file << "Ride History:\n";
        while (current) {
            file << current->id << "," 
                 << current->time << "," 
                 << current->date << "," 
                 << current->source << "," 
                 << current->destination << endl;
            current = current->next;
        }

        file.close();
        cout << "Ride history saved to " << filename << endl;
    }

    // Load ride history from a file
    void loadFromFile(const string& filename) {
        ifstream file(filename);

        if (!file) {
            cout << "Error opening file for reading or file not found.\n";
            return;
        }
        clear();
        string line;
        // Skip the header line ("Ride History:")
        getline(file, line);

        while (getline(file, line)) {
            stringstream ss(line);
            string idStr, time, date, source, destination;

            // Extract ride details from the CSV line
            getline(ss, idStr, ',');
            getline(ss, time, ',');
            getline(ss, date, ',');
            getline(ss, source, ',');
            getline(ss, destination, ',');

            // Add the ride to the list
            addRide(stoi(idStr), time, date, source, destination);
        }

        file.close();
        // cout << "Ride history loaded from " << filename << endl;
    }
    void clear() {
    RideNode* current = head;
    while (current) {
        RideNode* temp = current;
        current = current->next;
        delete temp;
    }
    head = tail = nullptr;
    }

    // Free memory
    ~RideHistory() {
        RideNode* current = head;
        while (current) {
            RideNode* temp = current;
            current = current->next;
            delete temp;
        }
        //cout << "Ride history cleared.\n";
    }
};

