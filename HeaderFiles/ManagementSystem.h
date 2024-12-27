#include<iostream>
#include<string>
#include<fstream>
#include<functional>
#include "User.h"
#include "Driver.h"
#include "LinkedList.h"

using namespace std;

class ManagementSystem {
private:
    LinkedList<User> users;
    LinkedList<Driver> drivers;
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

    string hashPassword(const string& password, const string& salt) {
        hash<string> hasher;
        size_t firstHash = hasher(password);
        size_t secondHash = hasher(to_string(firstHash) + salt);
        return to_string(secondHash);
    }

public:
    ManagementSystem() {
        loadIDCounters();
    }

    ~ManagementSystem() {
        saveIDCounters();
    }

    void registerUser(const string& username, const string& password, const string& email) {
        if (users.usernameExists(username)) {
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

        // Generate a unique salt (e.g., using the user ID)
        string salt = to_string(userIDCounter);
        string hashedPassword = hashPassword(password, salt);

        users.add(User(userIDCounter++, username, hashedPassword, email));

        // Calculate index using hash
        hash<string> hasher;
        size_t index = hasher(username) % userIDCounter;

        cout << "Congratulations!! You got registered successfully!!!\n";
        cout << "You are registered at index " << index << " in the system.\n";
    }

    void registerDriver(const string& username, const string& password, const string& license) {
        if (drivers.usernameExists(username)) {
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

        // Generate a unique salt (e.g., using the driver ID)
        string salt = to_string(driverIDCounter);
        string hashedPassword = hashPassword(password, salt);

        drivers.add(Driver(driverIDCounter++, username, hashedPassword, license));

        // Calculate index using hash
        hash<string> hasher;
        size_t index = hasher(username) % driverIDCounter;

        cout << "Congratulations! You got registered successfully as a driver!!!\n";
        cout << "You are registered at index " << index << " in the system.\n";
    }

    void loginUser(const string& username, const string& password) {
        User* user = users.find(username);
        if (!user) {
            cout << "User not found.\n";
            return;
        }
        user->login(password);
    }

    void loginDriver(const string& username, const string& password) {
        Driver* driver = drivers.find(username);
        if (!driver) {
            cout << "Driver not found.\n";
            return;
        }
        driver->login(password);
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
