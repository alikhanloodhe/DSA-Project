#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include<algorithm>
#include "rideHistory.h"
using namespace std;

class User {
public:
    int ID;
    string username;
    string password;
    string email;
    bool isLoggedIn;

    User() : ID(-1), username(""), password(""), email(""), isLoggedIn(false) {}
    User(int id, string uname, string pwd, string mail)
        : ID(id), username(uname), password(pwd), email(mail), isLoggedIn(false) {}

    void login(const string& pwd) {
        if (password == pwd) {
            isLoggedIn = true;
            cout << "User logged in successfully.\n";
            displayUserInterface(ID);
        } else {
            cout << "Invalid password.\n";
            string pass;
            cout << "Incorrect, Enter again: ";
            cin >> pass;
            while (pass != password) {
                cout << "Incorrect, Enter again: ";
                cin >> pass;
            }
            cout << "User logged in successfully.\n";
            displayUserInterface(ID);
        }
    }

    string serialize() const {
        return to_string(ID) + "," + username + "," + password + "," + email;
    }

    
    void displayMenu(){
        cout << "\n--- User Interface Menu ---\n";
        cout << "1. Book A Ride\n";
        cout << "2. Show recent rides\n";
        cout << "3. Show in app points\n";
        cout << "4. Exit\n";
        cout << "Enter your choice: ";
    }
    void displayUserInterface(int id){ // It would be great if a user object is passed over here
        RideHistory RH;
        int choice;
        displayMenu();
        cin >> choice;
        cin.ignore(); // Clear the input buffer for getline
        switch (choice) {
        case 1: 
            // appropriate function would be called
            break;

        case 2: 
            // here recent ride function will be called and id will be passed to it
            
            //RH.loadFromFile("RideHistory.txt"); // This has to be called once and it is already called in the constructor
            RH.displayForward(ID);
            RH.~RideHistory();
            displayUserInterface(ID);
            break;

        case 3: // Login User
            // In app points function will be called 
            break;
        case 4:
             cout << "Exiting User Interface...\n";
            // // Save ride history data and in app points data before exiting
            break;
        default:
            cout << "Invalid choice. Please try again.\n";
        
    }
}
    void deserialize(const string& line) {
        stringstream ss(line);
        string idStr;
         getline(ss, idStr, ',');
    if (!idStr.empty() && all_of(idStr.begin(), idStr.end(), ::isdigit)) {
        ID = stoi(idStr); // Safe stoi
    } else {
        cerr << "Error: Invalid ID format in input data.\n";
        ID = -1; // Assign a default or invalid ID
    }
        getline(ss, username, ',');
        getline(ss, password, ',');
        getline(ss, email);
    }
    
};
