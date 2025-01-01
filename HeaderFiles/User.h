#ifndef USER_H
#define USER_H
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include<algorithm>
#include <chrono>
#include <iomanip>
#include<windows.h>
#include<conio.h>
#include "DynamicRidePrice.h"
#include "RideRequestQueue.h"
#include "InAppPoint.h"
#include "Time.h"
#include "DriverRating.h"
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
        cout << "2. Show in Wallet points\n"; 
        cout << "3  Show recent rides\n";
        cout << "4. Exit\n";

    }
    void displayUserInterface(int id ){ 
        ErrorHandling ER;
        int choice;
        do{
        RideHistory RH;
        InAppPoint AP;
        displayMenu();
        choice = ER.getValidint(1,4);
        
        switch(choice) {
        case 1: 
            book_a_ride(ID);
            break;
        case 2: 
            AP.showInAppPoints(ID);
            AP.~InAppPoint();
            break;
        case 3: 
            RH.displayBackward(ID);
            break;
        case 4:
             cout << "Exiting User Interface...\n";
             ER.~ErrorHandling();
            break;
        default:
            cout << "Invalid choice. Please try again.\n";
        
    }
    }while(choice!=4);
}
    
//--------------------------------------BOOK A RIDE-----------------------------------------
void book_a_ride(int user_id){
    RideRequestMatching RRM;
    RRM.print_all_loc(); // printing all locations to the user
    vector<string> maplocations = RRM.get_All_Locations();
    string userLocation, destination;
    int NumStops=0;
    vector<string> stops(NumStops);
    get_ride_details(userLocation, destination, NumStops, &stops, maplocations); // assing the values to the variables
    // Search for available drivers
    RideRequestQueue RRQ;
    Queue user_queue;
    int choice = 1;

    vector<int>driver_ids; // will store driver ids of all available drivers
    vector<string>driverLocations; // will store locations of all available drivers

    RRQ.loadQueue(user_queue);  // Load Queued users from the file

    searchAvailableDrivers(driver_ids,driverLocations); // search driver availability file and give their locations and ids

    bool flag = false; // a flag that this user will not get the ride

    // If no drivers are available or user queue is not empty(some users are waiting in the queue), add user to the queue
    if(driver_ids.size()==0 || !user_queue.isEmpty()){
        RRQ.addToQueue(user_queue,user_id);
        flag = true;
    }
    
    // Search for the drivers again and again until it finds the driver
    while(flag || (driver_ids.size()==0 && choice == 1 && !user_queue.isEmpty() )){
        cout<<"Ops all drivers are busy."<<endl;
        cout<<"Press 1 to try again or 0 to CANCEL the ride!"<<endl;
        auto start = chrono::steady_clock::now();
        bool timed_out = false;

        while (true) {
            auto now = chrono::steady_clock::now();

            if (now - start >= chrono::seconds(30)) { // A 30 second timer
                cout << "\nRide Timeout :(\nKindly Book your ride again\n";
                choice = 0;
                timed_out = true;
                break;
            }

            // Check for user input using _kbhit() and _getch() (non-blocking input)
            if (_kbhit()) {
                char input = _getch(); // Read the character without waiting for Enter
                if (input == '1') {
                    choice = 1;
                } else if (input == '0') {
                    choice = 0;
                } else {
                    cout << "Invalid input. Try again!" << endl;
                    continue;
                }
                break;
            }

            Sleep(100); // Sleep for 1 second
        }

        if (timed_out || choice == 0) {
            cout<<"Ride Cancelled\n";
            break; // Exit loop if timeout occurs or user cancels the ride
        }

        if(choice ==1){
            user_queue.~Queue();
            RRQ.loadQueue(user_queue);
            if(user_queue.peek() ==user_id){
            flag = false;
            searchAvailableDrivers(driver_ids,driverLocations);
        }
        }
        else{ break; }
    }
    if(choice == 0){ 
        // If user cancels ride on its own will
        RRQ.removeFromQueue(user_queue,user_id);
        return;
    }
    
    RRQ.removeFromQueue(user_queue,user_id);  // Removing the user from the quue 
    string selected_driver;
    // Passing the user location, destination, driver locations, selected driver, number of stops and stops to handle ride function
    bool is_ride_accepted;
    Graph nustMap = RRM.getNustMap(); 
    is_ride_accepted = nustMap.handleRide(user_id,userLocation, destination,driver_ids, driverLocations,selected_driver,NumStops,stops); // This particular function will be modified here to handle ride
    if(is_ride_accepted){
        int current_id = find_Driver_Id(driver_ids,driverLocations,selected_driver); // Now we will have to find the id of nearest driver
        clearAvail_curr_driver(current_id); // Clear the availability of the driver selected
    
        Time t;
        DynamicRidePrice DRP;
        string time = t.get_current_time();
        t.~Time();
        int ride_price = DRP.getdynamicRidePrice(nustMap.get_total_Distance_with_Stops(), time, driver_ids.size(),stops);
        cout<<"The price of the ride is: "<<ride_price<<endl;
        rate_driver(current_id);
        SaveRideHistory(userLocation, destination);   // Add the ride to Ride History
        in_AppCheckout(user_id,ride_price);
    }
    else{
        cout<<"The driver seems to be busy. Please Book another one.\n";
    }
    
}

void in_AppCheckout(int userID,int ride_price){
    InAppPoint AP;
    int currentPoints = AP.getCurrentpoints(userID);
    ErrorHandling ER;
    if(currentPoints>100){
        cout << "Would you like to pay through your In Wallet Points: \n1. Yes \n2. No" << endl;
        int ch;
        ch = ER.getValidint(1,2);
    if(ch == 1){
        AP.checkOutUsingPoints(userID,ride_price);
    }
    else{
        return;
    }
    }
    ER.~ErrorHandling();

}

// rating the driver
void rate_driver(int driver_id){
    DriverRating DRs;
    DRs.UserRating(driver_id);
}

//--------------------------------------GET RIDE DETAILS-----------------------------------------
void get_ride_details(string & userLocation, string &destination, int &NumStops, vector<string> * stops, vector<string> mapLocations) {
    int choice;
    ErrorHandling ER;
    cout<< "------------Starting Location-------------\n";
    choice = ER.getValidint(1,mapLocations.size());
    userLocation = mapLocations[choice - 1];
    cout<<"Starting Location set to "<<userLocation<<"\n";
    cout << "------------Destination-------------\n";
    choice = ER.getValidint(1,mapLocations.size());
    while(userLocation == mapLocations[choice-1]){
        cout<<"Starting Location and Destination must be differnt\n";
        choice = ER.getValidint(1,mapLocations.size());
    }
    destination= mapLocations[choice - 1];
    cout<<"Destination set to "<<destination<<"\n";
    cout << "\nHow many stops do you want to take? 0-36\n";
    NumStops = ER.getValidint(0,mapLocations.size());
    stops->resize(NumStops); // Resize the vector
    for (int i = 0; i < NumStops; i++) {
        cout << "Enter stop " << i + 1 << "\n";
        int ch;
        ch = ER.getValidint(1,mapLocations.size());
        while(userLocation == mapLocations[ch-1] || destination == mapLocations[ch-1]){
        cout<<"Stop Location must be different than starting and ending location\n";
        ch = ER.getValidint(1,mapLocations.size());
    }
        (*stops)[i] = mapLocations[ch-1]; // Use pointer dereferencing to access the vector
    }
    ER.~ErrorHandling();
}

// finding id of the driver selected
int find_Driver_Id(vector<int> ids,vector<string>driverLocations, string driver){
    int index = 0;
    for(int i = 0;i<driverLocations.size();i++){
        if(driverLocations[i] == driver){
            break;
        }
        index++;
    }
    return ids[index];
}

// clear the availability of the driver assigned clearAvail_curr_driver
void clearAvail_curr_driver(int id) {
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
            if (stoi(idStr) != id) { // Keep entries not related to this driver
                tempFile << line << "\n";
            }
        }

        inFile.close();
        tempFile.close();

        remove("Files\\driverAvailability.txt");
        rename("Files\\temp.txt", "Files\\driverAvailability.txt");
    }

// Function to save ride history in a file
void SaveRideHistory(string userLocation,string destination){
    Time t;
    RideHistory RH;
    string time = t.get_current_time();
    string date = t.get_current_date();
    t.~Time();
    RH.addRide(ID, time, date, userLocation, destination);
    RH.saveToFile("Files\\RideHistory.txt");
    InAppPoint points;
    points.addPoints(ID,5,false);
    points.~InAppPoint();
    RH.~RideHistory();
}

static void searchAvailableDrivers(vector<int>& ids, vector<string>& locations) {
        ifstream inFile("Files\\driverAvailability.txt");
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
#endif