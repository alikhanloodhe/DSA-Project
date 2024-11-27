#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include<algorithm>
#include <chrono>
#include <iomanip>
#include "rideHistory.h"
//#include "RideRequestMatching.h"
#include "RidePrice.h"
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
        switch(choice) {
        case 1: 
            // from the file and then based on an algorithm will calculate price for the ride
            // After Giving details about a ride
            // Here we will have a check about if the user availbility of all drivers is false
            // We add the user id in a file and a queue will read from that file.
            // If three or four users are in the file they will be loaded into the queue ig
            // and then they will be served accordingly
            book_a_ride(ID);
            displayUserInterface(ID);
            break;

        case 2: 
            // here recent ride function will be called and id will be passed to it
            
            //RH.loadFromFile("RideHistory.txt"); // This has to be called once and it is already called in the constructor
            
            RH.displayForward(ID);
            RH.~RideHistory();
            displayUserInterface(ID);
            break;

        case 3: // Lo
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
    
void book_a_ride(int ID){
    RideRequestMatching RRM;
    RRM.print_all_loc();
    string userLocation, destination;
    cout << "Enter your current location: ";
    getline(cin, userLocation);
    cout << "Enter your destination: ";
    getline(cin, destination);
    // Search for available drivers
    int choice = 1;
    vector<int>ids;
    vector<string>driverLocations;
    
    searchAvailableDrivers(ids,driverLocations);
 
    // Search for the drivers again and again until it finds the driver
    while(ids.size()==0 && choice == 1){
        cout<<"Ops all drivers are busy."<<endl;
        cout<<"Press 1 to try again or 0 to exit"<<endl;
        cin>>choice;
        searchAvailableDrivers(ids,driverLocations);
    }
    if(choice == 0){
        return;
    }
    string nearest_driver;
    RRM.hanldeRide(userLocation, destination, driverLocations,nearest_driver);
    // Now we will have to find the id of nearest driver
    int current_id = findId(ids,driverLocations,nearest_driver);
    clearAvailability(current_id);
    // Clear the availability of the driver selected
    // Calculate the price of the ride
    RidePrice RP;
    string time = get_current_time();
    int price = RP.dynamicRidePrice(userLocation, destination, time, driverLocations.size());
    cout<<"The price of the ride is: "<<price<<endl;
    SaveRideHistory(userLocation, destination);   // Add the ride to Ride History
    displayUserInterface(ID);
}

int findId(vector<int> ids,vector<string>driverLocations, string driver){
    int index = 0;
    for(int i = 0;i<driverLocations.size();i++){
        if(driverLocations[i] == driver){
            break;
        }
        index++;
    }
    return ids[index];
}
void clearAvailability(int id) {
        std::ifstream inFile("driverAvailability.txt");
        std::ofstream tempFile("temp.txt");
        if (!inFile.is_open() || !tempFile.is_open()) {
            cerr << "Error handling availability file.\n";
            return;
        }

        string line;
        while (getline(inFile, line)) {
            stringstream ss(line);
            string idStr, loc;
            getline(ss, idStr, ',');
            if (stoi(idStr) != id) { // Keep entries not related to this driver
                tempFile << line << "\n";
            }
        }

        inFile.close();
        tempFile.close();

        remove("driverAvailability.txt");
        rename("temp.txt", "driverAvailability.txt");
    }
void SaveRideHistory(string userLocation,string destination){
    RideHistory RH;
    string time = get_current_time();
    string date = get_current_date();
    RH.addRide(ID, time, date, userLocation, destination);
    RH.saveToFile("Files\\RideHistory.txt");
    RH.~RideHistory();
}
static void searchAvailableDrivers(vector<int>& ids, vector<string>& locations) {
        std::ifstream inFile("driverAvailability.txt");
        if (!inFile.is_open()) {
            cerr << "Error opening driver availability file.\n";
            return;
        }

        string line;
        while (getline(inFile, line)) {
            stringstream ss(line);
            string idStr, loc;
            getline(ss, idStr, ',');
            getline(ss, loc, ',');

            if (!idStr.empty() && !loc.empty()) {
                ids.push_back(stoi(idStr));
                locations.push_back(loc);
            }
        }

        inFile.close();
}


string get_current_time() {
    // Get current time
    auto now = std::chrono::system_clock::now();
    std::time_t now_time = std::chrono::system_clock::to_time_t(now);

    // Convert to string format (12-hour format with AM/PM)
    std::tm* tm_now = std::localtime(&now_time);
    std::ostringstream oss;
    oss << std::put_time(tm_now, "%I:%M:%S %p");  // 12-hour format with AM/PM
    return oss.str();
}
string get_current_date() {
    // Get current time
    auto now = std::chrono::system_clock::now();
    std::time_t now_time = std::chrono::system_clock::to_time_t(now);

    // Convert to string format (YYYY-MM-DD)
    std::tm* tm_now = std::localtime(&now_time);
    std::ostringstream oss;
    oss << std::put_time(tm_now, "%Y-%m-%d");
    return oss.str();
}
};