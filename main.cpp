#include <iostream>
#include <string>
#include "HeaderFiles/ManagementSystem.h"


using namespace std;
void get_user_LoginInfo(string & username,string &password);
void get_driver_LoginInfo(string & username,string &password);
// Menu Display Function
void displayMenu() {
    cout << "\n--- Management System Menu ---\n";
    cout << "1. Register User\n";
    cout << "2. Register Driver\n";
    cout << "3. Login User\n";
    cout << "4. Login Driver\n";
    cout << "5. Exit\n";
    // cout << "Enter your choice: ";
}

int main() {
    ManagementSystem system;
    system.loadData(); // Load existing data from files
    cout<<"Welcome to Smart Ride Application";

    int choice;
    do {
        displayMenu();
        ErrorHandling ER;
        choice = ER.getValidint(1,5);
        ER.~ErrorHandling();

        string username, password, email, license;
        int id;

        switch (choice) {
        case 1: // Register User
            cout << "Enter username: ";
            getline(cin, username);
            cout << "Enter email: ";
            getline(cin, email);
            cout << "Enter password: ";
            getline(cin, password);
            system.registerUser(username, password, email);
            system.saveData(); // Save data after registration
            break;

        case 2: // Register Driver
            cout << "Enter username: ";
            getline(cin, username);
            cout << "Enter password: ";
            getline(cin, password);
            cout << "Enter license number: ";
            getline(cin, license);
            system.registerDriver(username, password, license);
            system.saveData(); // Save data after registration
            break;

        case 3:   // Login User
            get_user_LoginInfo(username,password);
            system.loginUser(username,password);
            break;

        case 4: // Login Driver
            get_driver_LoginInfo(username,password);
            system.loginDriver(username, password);
            break;

        case 5: // Exit
            cout << "Exiting program...\n";
            system.saveData(); // Save data before exiting
            break;

        default:
            cout << "Invalid choice. Please try again.\n";
        }
    } while (choice != 5);

    return 0;
}

void get_user_LoginInfo(string & username,string &password){
    cout << "Enter username: ";
    getline(cin,username);
    cout << "Enter password: ";
    getline(cin, password);
}
void get_driver_LoginInfo(string & username,string &password){
    cout << "Enter username: ";
    getline(cin,username);
    cout << "Enter password: ";
    getline(cin, password);
}