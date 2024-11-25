#include<iostream>
#include<string>
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
        ifstream inFile("ids.txt");
        if (inFile) {
            inFile >> userIDCounter >> driverIDCounter;
        } else {
            userIDCounter = 1;
            driverIDCounter = 1;
        }
        inFile.close();
    }

    void saveIDCounters() {
        ofstream outFile("ids.txt");
        outFile << userIDCounter << " " << driverIDCounter;
        outFile.close();
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
            string username,email,password;
            cout << "Enter username: ";
            getline(cin, username);
            cout << "Enter email: ";
            getline(cin, email);
            cout << "Enter password: ";
            getline(cin, password);
            registerUser(username,password,email);
            return;
        }
        users.add(User(userIDCounter++, username, password, email));
        cout << "User registered successfully with ID: " << userIDCounter - 1 << ".\n";
    }

    void registerDriver(const string& username, const string& password, const string& license) {
        if (drivers.usernameExists(username)) {
            cout << "Username already exists. Please enter a new one.\n";
            string username,email,password;
            cout << "Enter username: ";
            getline(cin, username);
            cout << "Enter email: ";
            getline(cin, email);
            cout << "Enter password: ";
            getline(cin, password);
            registerDriver(username,password,license);
            return;
        }
        drivers.add(Driver(driverIDCounter++, username, password, license));
        cout << "Driver registered successfully with ID: " << driverIDCounter - 1 << ".\n";
    }
// Login user by username:
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


// Login user by id
//     void loginUser(int id, const string& password) {
//     // User* user = users.find(username); // findByID returns a User*, not a Node<User>*
//     User* user = users.findByID(id);
//     if (!user) {
//         cout << "User not found.\n";
//         return;
//     }
//     user->login(password); // Call login on the User object
// }

// Login Driver by ID
// void loginDriver(int id, const string& password) {
//     Driver* driver = drivers.findByID(id); // findByID returns a Driver*, not a Node<Driver>*
//     if (!driver) {
//         cout << "Driver not found.\n";
//         return;
//     }
//     driver->login(password); // Call login on the Driver object
// }
    void saveData() {
        users.saveToFile("users.txt");
        drivers.saveToFile("drivers.txt");
        saveIDCounters();
    }

    void loadData() {
        users.loadFromFile("users.txt");
        drivers.loadFromFile("drivers.txt");
    }
};
