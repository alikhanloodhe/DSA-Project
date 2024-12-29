#include<iostream>
#include<string>
#include "User.h"
#include "Driver.h"
#include "HashTable.h"

using namespace std;
class ManagementSystem {
private:
    HashTable<User> users; // Creating a Hash Table for the user
    HashTable<Driver> drivers; // Creating a Hash Table for the driver
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
    ManagementSystem() {
        loadIDCounters();
    }

    ~ManagementSystem() {
        saveIDCounters();
    }

    void registerUser(const string& username, const string& password, const string& email) {
        if (users.usernameExists(username)) {
            cout << "Username already exists. Please enter your details again.\n";
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
        users.insert(User(userIDCounter++, username, password, email));
        cout << "Congratulations!! You got registered successfully!!!" << ".\n";
        InAppPoint AP;
        AP.addPoints(userIDCounter-1,50,true);
        cout<<"Bonus!!! You got 50 free points on signup!!\n";
        User* user = users.find(username);
        user->login(password);
    }

    void registerDriver(const string& username, const string& password, const string& license) {
        if (drivers.usernameExists(username)) {
            cout << "Username already exists. Please enter your details again.\n";
            string username,password,license;
            cout << "Enter username: ";
            getline(cin, username);
            cout << "Enter password: ";
            getline(cin, password);
            cout << "Enter License: ";
            getline(cin, license);
            registerDriver(username,password,license);
            return;
        }
        drivers.insert(Driver(driverIDCounter++, username, password, license));
        cout << "Congratulations You got registered successfully as a driver!!!." << ".\n";
        Driver* driver = drivers.find(username);
        driver->login(password);
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
        // Loading the data into a hash Tables at the start of the program
        users.loadFromFile("Files\\users.txt");
        drivers.loadFromFile("Files\\drivers.txt");
    }
};
