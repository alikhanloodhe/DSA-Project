#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include<algorithm>
#include "DriverRating.h"

using namespace std;

class Driver {
    public:
    int ID;
    string username;
    string password;
    string licenseNumber;
    string location;
    bool isAvailable;
    

    Driver() : ID(-1), username(""), password(""), licenseNumber(""), isAvailable(false), location("") {}
    Driver(int id, string uname, string pwd, string license)
        : ID(id), username(uname), password(pwd), licenseNumber(license), isAvailable(false), location("") {}

    void login(const string& pwd) {
        if (password == pwd) {
            cout << "Driver logged in successfully.\n";
            displayDriverInterface(ID);
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
            displayDriverInterface(ID);
        }
    }

    void displayMenu() {
        cout << "\n--- Driver Interface Menu ---\n";
        cout << "1. Set your location\n";
        cout << "2. Set Availability\n";
        cout << "3. View Rating\n";
        cout << "4. Exit\n";
        cout << "Enter your choice: ";
    }

    void displayDriverInterface(int id) {
        RideRequestMatching RRM; // Assume this class exists and handles locations.
        int choice;

        while (true) {
            displayMenu();
            cin >> choice;
            cin.ignore(); // Clear the input buffer

            switch (choice) {
            case 1:
                setLocation(RRM);
                break;

            case 2:
                setAvailability();
                break;

            case 3:
                showRating(ID);
                break;
            case 4:
                cout << "Exiting Driver Interface...\n";
                clearAvailability(); // Clear availability before exiting
                return;

            default:
                cout << "Invalid choice. Please try again.\n";
            }
        }
    }

    void setLocation(RideRequestMatching& RRM) {
        vector<string> locations = RRM.get_All_Locations(); // Fetch available locations
        cout << "Choose your location from the following options:\n";

        for (size_t i = 0; i < locations.size(); ++i) {
            cout << i + 1 << ". " << locations[i] << "\n";
        }

        cout << "Enter the number corresponding to your location: ";
        int locChoice;
        cin >> locChoice;

        if (locChoice > 0 && locChoice <= locations.size()) {
            location = locations[locChoice - 1];
            cout << "Location set to: " << location << "\n";
        } else {
            cout << "Invalid choice. Please try again.\n";
        }
    }

    void setAvailability() {
        if (location.empty()) {
        cout << "You must set your location before setting availability.\n";
        return;
    }
        cout << "Set Availability\n";
        cout << "1. Available\n";
        cout << "2. Not Available\n";
        cout << "Enter your choice: ";
        int choice;
        cin >> choice;

        if (choice == 1) {
            isAvailable = true;
            saveAvailability();
            cout << "Availability set to Available.\n";
        } else if (choice == 2) {
            isAvailable = false;
            clearAvailability(); // Remove availability from file
            cout << "Availability set to Not Available.\n";
        } else {
            cout << "Invalid choice. Please try again.\n";
        }
    }

    void showRating(int driverID) {
        DriverRating DR;
        DR.ShowDriverRating(driverID);
    }

    void saveAvailability() {
        // Clear any existing entries for this driver before saving new availability
        clearAvailability();

        std::ofstream outFile("Files\\driverAvailability.txt", std::ios::app);
        if (outFile.is_open()) {
            outFile << ID << "," << location << "\n";
            outFile.close();
        } else {
            cerr << "Unable to open file for saving availability.\n";
        }
    }

    void clearAvailability() {
        std::ifstream inFile("Files\\driverAvailability.txt");
        std::ofstream tempFile("Files\\temp.txt");
        if (!inFile.is_open() || !tempFile.is_open()) {
            cerr << "Error handling availability file.\n";
            return;
        }

        string line;
        while (getline(inFile, line)) {
            stringstream ss(line);
            string idStr, loc;
            getline(ss, idStr, ',');
            if (stoi(idStr) != ID) { // Keep entries not related to this driver
                tempFile << line << "\n";
            }
        }

        inFile.close();
        tempFile.close();

        remove("Files\\driverAvailability.txt");
        rename("Files\\temp.txt", "Files\\driverAvailability.txt");
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
