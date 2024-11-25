#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include<algorithm>
using namespace std;

class Driver {
public:
    int ID;
    string username;
    string password;
    string licenseNumber;
    bool isAvailable;

    Driver() : ID(-1), username(""), password(""), licenseNumber(""), isAvailable(true) {}
    Driver(int id, string uname, string pwd, string license)
        : ID(id), username(uname), password(pwd), licenseNumber(license), isAvailable(true) {}

    void login(const string& pwd) {
        if (password == pwd) {
            cout << "Driver logged in successfully.\n";
        } else {
            cout << "Invalid password.\n";
            string pass;
            cout << "Incorrect, Enter again: ";
            cin >> pass;
            while (pass != password) {
                cout << "Incorrect, Enter again: ";
                cin >> pass;
            }
            cout << "Driver logged in successfully.\n";
        }
    }

    string serialize() const {
        return to_string(ID) + "," + username + "," + password + "," + licenseNumber;
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
    getline(ss, licenseNumber, ',');
    string availabilityStr;
    getline(ss, availabilityStr);

    isAvailable = (availabilityStr == "1"); // Assume 1 means available
}
};
