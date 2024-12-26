#ifndef MANAGEMENTSYSTEM_H
#define MANAGEMENTSYSTEM_H

#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include "User.h"
#include "Driver.h"
#include "LinkedList.h"

using namespace std;

// Hash Table for storing username and password pairs with Double Hashing
class HashTable {
private:
    struct Entry {
        string username;
        string password;
        bool isOccupied;  // To check if the slot is occupied or deleted
    };

    vector<Entry> table;
    int size;

    // Hash function 1 (Primary hash function)
    int hashFunction1(const string& key) {
        int hash = 0;
        for (char c : key) {
            hash += c;  // Simple hash function, sum of ASCII values
        }
        return hash % size;
    }

    // Hash function 2 (Secondary hash function)
    int hashFunction2(const string& key) {
        int hash = 0;
        for (char c : key) {
            hash = (hash * 31 + c) % size;  // Another hash function, using a prime multiplier
        }
        return 1 + (hash % (size - 1));  // Ensure it's not 0
    }

public:
    HashTable(int tableSize) : size(tableSize) {
        table.resize(size);
    }

    // Insert username and password
    void insert(const string& username, const string& password) {
        int index = hashFunction1(username);
        int step = hashFunction2(username);

        // Double hashing: resolve collision by probing
        while (table[index].isOccupied) {
            index = (index + step) % size;  // Linear probe with double hashing
        }

        table[index] = {username, password, true};
        cout << "Inserted: " << username << " at index " << index << endl; // Debugging output
    }

    // Search for password by username
    string search(const string& username) {
        int index = hashFunction1(username);
        int step = hashFunction2(username);

        // Double hashing: check if the slot is occupied and if the username matches
        int initialIndex = index;
        do {
            if (table[index].isOccupied && table[index].username == username) {
                return table[index].password;
            }
            index = (index + step) % size;
        } while (table[index].isOccupied && index != initialIndex); // Loop until the entire table is checked

        return "";  // return empty string if username is not found
    }

    // Save hash table to file
    void saveToFile(const string& filename) {
        ofstream outFile(filename);
        for (const auto& entry : table) {
            if (entry.isOccupied) {
                outFile << entry.username << " " << entry.password << endl;
            }
        }
        outFile.close();
    }

    // Load hash table from file
    void loadFromFile(const string& filename) {
        ifstream inFile(filename);
        string username, password;
        while (inFile >> username >> password) {
            insert(username, password);
        }
        inFile.close();
    }
};

class ManagementSystem {
private:
    LinkedList<User> users;
    LinkedList<Driver> drivers;
    HashTable userTable;  // Hash table for user credentials
    HashTable driverTable;  // Hash table for driver credentials
    int userIDCounter;
    int driverIDCounter;

    void loadIDCounters() {
        ifstream inFile("Files\\ids.txt");
        if (inFile) {
            inFile >> userIDCounter >> driverIDCounter;
        } else {
            userIDCounter = 1;
            driverIDCounter = 1;
        }
        inFile.close();
    }

    void saveIDCounters() {
        ofstream outFile("Files\\ids.txt");
        outFile << userIDCounter << " " << driverIDCounter;
        outFile.close();
    }

public:
    ManagementSystem() : userTable(100), driverTable(100) {  // initialize hash tables with size 100
        loadIDCounters();
        userTable.loadFromFile("Files\\users_hash.txt");
        driverTable.loadFromFile("Files\\drivers_hash.txt");
    }

    ~ManagementSystem() {
        saveIDCounters();
        userTable.saveToFile("Files\\users_hash.txt");
        driverTable.saveToFile("Files\\drivers_hash.txt");
    }

    void registerUser(const string& username, const string& password, const string& email) {
        if (!userTable.search(username).empty()) {
            cout << "Username already exists. Please enter a new one.\n";
            string username, email, password;
            cout << "Enter username: ";
            getline(cin, username);
            cout << "Enter email: ";
            getline(cin, email);
            cout << "Enter password: ";
            getline(cin, password);
            registerUser(username, password, email);
            return;
        }
        users.add(User(userIDCounter++, username, password, email));
        userTable.insert(username, password);  // Insert into hash table
        cout << "User registered successfully with ID: " << userIDCounter - 1 << ".\n";
    }

    void registerDriver(const string& username, const string& password, const string& license) {
        if (!driverTable.search(username).empty()) {
            cout << "Username already exists. Please enter a new one.\n";
            string username, password, license;
            cout << "Enter username: ";
            getline(cin, username);
            cout << "Enter password: ";
            getline(cin, password);
            cout << "Enter License: ";
            getline(cin, license);
            registerDriver(username, password, license);
            return;
        }
        drivers.add(Driver(driverIDCounter++, username, password, license));
        driverTable.insert(username, password);  // Insert into hash table
        cout << "Driver registered successfully with ID: " << driverIDCounter - 1 << ".\n";
    }

    void loginUser(const string& username, const string& password) {
        string storedPassword = userTable.search(username);
        if (storedPassword.empty()) {
            cout << "User not found.\n";
            return;
        }
        if (storedPassword == password) {
            cout << "Login successful for user: " << username << endl;
        } else {
            cout << "Invalid password.\n";
        }
    }

    void loginDriver(const string& username, const string& password) {
        string storedPassword = driverTable.search(username);
        if (storedPassword.empty()) {
            cout << "Driver not found.\n";
            return;
        }
        if (storedPassword == password) {
            cout << "Login successful for driver: " << username << endl;
        } else {
            cout << "Invalid password.\n";
        }
    }

    void saveData() {
        users.saveToFile("Files\\users.txt");
        drivers.saveToFile("Files\\drivers.txt");
        saveIDCounters();
    }

    void loadData() {
        users.loadFromFile("Files\\users.txt");
        drivers.loadFromFile("Files\\drivers.txt");
    }
};

#endif // MANAGEMENTSYSTEM_H
