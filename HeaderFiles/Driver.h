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

    // Loging in the driver!!
    void login(const string& pwd) {
        if (password == pwd) {
            cout << "Driver logged in successfully.\n";
            displayDriverInterface(ID);
        } else {
            int attempts = 1;
            cout << "Invalid password.\n";
            string pass;
            cout << "Incorrect, Enter again: ";
            getline(cin,pass);
            while (pass != password && attempts<=5) {
                cout << "Incorrect, Enter again: ";
                getline(cin,pass);
                attempts++;
            }
            if(attempts>5){
                cout<<"You have reached maximum limit. Kindly enter your Login again!!\n";
                return;
            }
            cout << "Driver logged in successfully.\n";
            displayDriverInterface(ID);
        }
    }

    void displayMenu() {
        cout << "\n--- Driver Interface Menu ---\n";
        cout << "1. Set your location\n";
        cout << "2. Set Availability\n";
        cout << "3. Check For Available Rides\n";
        cout << "4. View Rating\n";
        cout << "5. Exit\n";
    }

    void displayDriverInterface(int id) {
        int ch;
        ErrorHandling ER;
        RideRequestMatching RRM;
        do{
            displayMenu();
            
            ch = ER.getValidint(1,5);
            switch (ch) {
            case 1:
                setLocation(RRM);
                break;
            case 2:
                setAvailability();
                break;
            case 3:
                accept_a_ride(ID);
                break;
            case 4:
                showRating(ID);
                break;
            case 5:
                cout << "Exiting Driver Interface...\n";
                RRM.~RideRequestMatching();
                ER.~ErrorHandling();
                clearAvailability(); // Clear availability before exiting
                return;

            default:
                cout << "Invalid choice. Please try again.\n";
            }
        }while (ch!=5);
    }
    //------------------------------1--------------------------------------------
    void setLocation(RideRequestMatching& RRM) {
        clearAvailability();
        vector<string> locations = RRM.get_All_Locations(); // Fetch available locations
        cout << "Choose your location from the following options:\n";
        RRM.print_all_loc();
        cout << "Enter the number corresponding to your location: ";
        int locChoice;
        ErrorHandling ER;
        locChoice = ER.getValidint(1,locations.size());
        location = locations[locChoice - 1];
        cout<<"Locaiton is set to "<<locations[locChoice-1]<<"\n";
        ER.~ErrorHandling();
        
    }

    // -----------------------------------2----------------------------------------
     void setAvailability() {
        if (location.empty()) {
        cout << "You must set your location before setting availability.\n";
        return;
    }
        cout << "Set Availability\n";
        cout << "1. Available\n";
        cout << "2. Not Available\n";
        int choice;
        ErrorHandling ER;
        choice = ER.getValidint(1,2);
        ER.~ErrorHandling();
        if (choice == 1) {
            isAvailable = true;
            saveAvailability();
            cout << "Availability set to Available.\n";
        } else {
            isAvailable = false;
            clearAvailability(); // Remove availability from file
            cout << "Availability set to Not Available.\n";
        }
    }
    //-------------------------------------------------------------------------------------
    void saveAvailability() {
        // Clear any existing entries for this driver before saving new availability
        clearAvailability();

        ofstream outFile("Files\\driverAvailability.txt", ios::app);
        if (outFile.is_open()) {
            outFile << ID << "," << location << "\n";
            outFile.close();
        } else {
            cerr << "Unable to open file for saving availability.\n";
        }
    }

    // If the driver exits peacefully It clears its availability from the Availability file

    //--------------------------------------------------------------------------------------
    void clearAvailability() {
        ifstream inFile("Files\\driverAvailability.txt");
        ofstream tempFile("Files\\temp.txt");
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

    //-----------------------------------------------------------
    void accept_a_ride(int driver_id){
        ErrorHandling ER;
        int id;
        string name;
        string user_location;
        string destination;
        vector<string> stops;
        bool is_ride;
        is_ride = get_ride_detail(id,name,user_location,destination,stops,driver_id);
        if(is_ride){
            cout<<"Request Details: \n";
            cout<<name<<" requests Ride From "<<user_location<<" To "<<destination<<"\n";
            if(stops.size()>=1){
                cout<<"Ride Include stops at: ";
                for(string s: stops){
                    cout<<s<<" ";
            }
                cout<<endl;
            }
            print_driver_toUserPath(location,user_location);
            int choice;
            cout<<"\nEnter 1 to accept the ride :)\nEnter 2 to reject the ride :(\n";
            choice = ER.getValidint(1,2);
            is_ride = get_ride_detail(id,name,user_location,destination,stops,driver_id);
        if(choice == 1 && is_ride){
            pass_acceptance();
            clear_user_request();
        }
        else if(choice == 2 && is_ride){
            // clears the file and do write a reject flag
            pass_rejection();
            clear_user_request();
        }
        else{
            cout<<"Ride Acceptance Timeout!!\n";
        }
        }
        else{
            cout<<"No Available Rides Yet\nKeep your status available to get maximum rides\n";
        }
        
    }
    void print_driver_toUserPath(string driverLocation,string userLocation){
        RideRequestMatching RRM;
        Graph nustMap = RRM.getNustMap();
        vector<string> path;
        pair<int, vector<string>> result = nustMap.calculateShortestPath(driverLocation,userLocation);
        path = result.second;
        if(result.first>0){
            cout<<"You will take "<<result.first<<" meters to reach to "<<userLocation<<"\n";
            cout << "Your path to user: ";
            for (size_t i = 0; i < path.size(); ++i) {
            cout << path[i];
            if (i < path.size() - 1) cout << " -> ";
            // Sleep(1000);
        }
        cout << endl;
        }
        else{
            cout<<"You are at user Location!!\n";
        }
        
    }


    //-------------------------------------------------------------------------
    void pass_acceptance(){
        ofstream outFile("Files\\ride_accept.txt", ios::app);
        if (outFile.is_open()) {
            outFile << "1" << "\n";
            outFile.close();
        } else {
            cerr << "Unable to open file for saving availability.\n";
        }
    }
    //-------------------------------------------------------------------------
    void pass_rejection(){
        ofstream outFile("Files\\ride_accept.txt", ios::app);
        if (outFile.is_open()) {
            outFile << "0" << "\n";
            outFile.close();
        } else {
            cerr << "Unable to open file for saving availability.\n";
        }
    }
    //--------------------------------------------------------------------------
    void clear_user_request(){
        ofstream file("Files\\user_request.txt", ios::trunc); // Open the file in truncate mode
    if (file.is_open()) {
        file.close();
    } else {
        cerr << "Error: Unable to open file for clearing "  << endl;
    }
    }
    //---------------------------------------------------------------------------------
    bool get_ride_detail(int id, string& name, string& location, string& destination, vector<string>& stops, int driverID) {
    ifstream inFile("Files\\user_request.txt");
    if (!inFile.is_open()) {
        cerr << "Error opening driver availability file.\n";
        return false;
    }

    string line;
    bool found = false; // To track if the driver's ID is found
    while (getline(inFile, line)) {
        stringstream ss(line);
        string idStr, Name, loc, dest, driver_id, stopsField;

        getline(ss, idStr, ',');
        getline(ss, Name, ',');
        getline(ss, loc, ',');
        getline(ss, dest, ',');
        getline(ss, driver_id, ',');

        // Check if the driver's ID matches
        try {
            if (!driver_id.empty() && stoi(driver_id) == driverID) {
                name = Name;
                location = loc;
                destination = dest;

                // Extract stops if available
                if (getline(ss, stopsField, ',')) {
                    stops.clear(); // Clear any existing stops
                    stringstream stopsStream(stopsField.substr(stopsField.find(":") + 1));
                    string stop;
                    while (getline(stopsStream, stop, '|')) {
                        stops.push_back(stop); // Split stops by '|'
                    }
                }

                found = true;
                break;
            }
        } catch (const invalid_argument& e) {
            cerr << "Invalid ID in file: " << idStr << endl;
        } catch (const out_of_range& e) {
            cerr << "ID out of range in file: " << idStr << endl;
        }
    }

    inFile.close();
    if (!found) {
        return false;
    }

    return true; // Return true if driver's ID is found
}
    
    //--------------------------------------------------------------------------
    void showRating(int driverID) {
        DriverRating DR;
        DR.ShowDriverRating(driverID); // A destructor must be called to clear the memory
    }
    
    //---------------------------get details from the file of given driver------------------------------------
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
