#ifndef USER_H
#define USER_H
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include<algorithm>
#include <chrono>
#include <iomanip>
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
        cout << "2. Show in Wallet points\n"; 
        cout << "3  Show recent rides\n";
        cout << "4. Exit\n";
        cout << "Enter your choice: ";
    }
    void displayUserInterface(int id ){ // It would be great if a user object is passed over here
        RideHistory RH;
        InAppPoint AP;
        int choice;
        do{
        displayMenu();
        if (!(cin >> choice)) {
            cout << "Invalid input. Please enter a valid choice.\n";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            continue;
        }
        
        cin.ignore(); // Clear the input buffer for getline
        switch(choice) {
        case 1: 
            book_a_ride(ID);
            break;
        case 2: 
            AP.showInAppPoints(ID);
            AP.~InAppPoint();
            break;
        case 3: 
            RH.displayForward(ID);
            break;
        case 4:
             cout << "Exiting User Interface...\n";
            // // Save ride history data and in app points data before exiting
            break;
        default:
            cout << "Invalid choice. Please try again.\n";
        
    }
    }while(choice!=4);
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
    cout << "\nHow many stops do you want to take? ";
        int NumStops;
        cin >> NumStops;
        cin.ignore();
        cout << endl;
        vector<string> stops(NumStops);
        for (int i = 0; i < NumStops; ++i) {
            cout << "Enter stop " << i + 1 << ": ";
            getline(cin, stops[i]);
        }
    // Search for available drivers
    RideRequestQueue RRQ;
    Queue user_queue;
    int choice = 1;
    vector<int>ids;
    vector<string>driverLocations;
    RRQ.loadQueue(user_queue);
    searchAvailableDrivers(ids,driverLocations);
    bool flag = false;
    if(ids.size()==0 || !user_queue.isEmpty()){
        RRQ.addToQueue(user_queue,ID);
        flag = true;
    }
    
    // Search for the drivers again and again until it finds the driver
    while(flag || (ids.size()==0 && choice == 1 && !user_queue.isEmpty() )){
        cout<<"Ops all drivers are busy."<<endl;
        cout<<"Press 1 to try again or 0 to CANCEL the ride!"<<endl;
        cin>>choice;
        if(choice ==1){
            user_queue.~Queue();
            RRQ.loadQueue(user_queue);
            if(user_queue.peek() ==ID){
            flag = false;
            searchAvailableDrivers(ids,driverLocations);
        }
        }
        else{ break; }
    }
    if(choice == 0){
        RRQ.removeFromQueue(user_queue,ID);
        return;
    }
    RRQ.removeFromQueue(user_queue,ID);
    string selected_driver;
    RRM.hanldeRide(userLocation, destination, driverLocations,selected_driver,NumStops,stops);
    
    int current_id = find_Driver_Id(ids,driverLocations,selected_driver); // Now we will have to find the id of nearest driver
    clearAvailability(current_id); // Clear the availability of the driver selected
    Time t;
    // Calculate the price of the ride
    DynamicRidePrice DRP;
    string time = t.get_current_time();
    t.~Time();
    int price = DRP.getdynamicRidePrice(userLocation, destination, time, driverLocations.size(),stops);
    cout<<"The price of the ride is: "<<price<<endl;
    DriverRating DRs;
    DRs.UserRating(current_id);
    SaveRideHistory(userLocation, destination);   // Add the ride to Ride History
    InAppPoint AP;
    int currentPoints = AP.getCurrentpoints(ID);
    if(currentPoints>20){
        cout << "Would you like to pay through your In Wallet Points: \n1. Yes \n 2.No" << endl;
        int ch;
        cin >> ch;
    if(ch == 1){
        AP.checkOutUsingPoints(ID,price);
    }
    else{
        return;
    }
    }

}
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
void clearAvailability(int id) {
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
void SaveRideHistory(string userLocation,string destination){
    Time t;
    RideHistory RH;
    string time = t.get_current_time();
    string date = t.get_current_date();
    t.~Time();
    RH.addRide(ID, time, date, userLocation, destination);
    RH.saveToFile("Files\\RideHistory.txt");
    InAppPoint points;
    points.addPoints(ID,5);
    points.~InAppPoint();
    RH.~RideHistory();
}
static void searchAvailableDrivers(vector<int>& ids, vector<string>& locations) {
        std::ifstream inFile("Files\\driverAvailability.txt");
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
};
#endif