#ifndef USER_H
#define USER_H
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include<algorithm>
#include <chrono>
#include <iomanip>
#include "rideHistory.h"
#include "RidePrice.h"
#include "Queue.h"
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
    Queue user_queue;
    int choice = 1;
    vector<int>ids;
    vector<string>driverLocations;
    loadQueue(user_queue);
    searchAvailableDrivers(ids,driverLocations);
    bool flag = false;
    if(ids.size()==0 || !user_queue.isEmpty()){
        addToQueue(user_queue,ID);
        flag = true;
    }
    
    // Search for the drivers again and again until it finds the driver
    while(flag || (ids.size()==0 && choice == 1 && !user_queue.isEmpty() )){
        cout<<"Ops all drivers are busy."<<endl;
        cout<<"Press 1 to try again or 0 to CANCEL the ride!"<<endl;
        cin>>choice;
        if(choice ==1){
            user_queue.~Queue();
            loadQueue(user_queue);
            if(user_queue.peek() ==ID){
            flag = false;
            searchAvailableDrivers(ids,driverLocations);
        }
        }
        else{
            break;
        }
        
        
    }
    if(choice == 0){
        removeFromQueue(user_queue,ID);
        return;
    }
    removeFromQueue(user_queue,ID);
    string selected_driver;
    RRM.hanldeRide(userLocation, destination, driverLocations,selected_driver);
    // Now we will have to find the id of nearest driver
    int current_id = findId(ids,driverLocations,selected_driver);
    clearAvailability(current_id); // Clear the availability of the driver selected
    
    // Calculate the price of the ride
    RidePrice RP;
    string time = get_current_time();
    int price = RP.dynamicRidePrice(userLocation, destination, time, driverLocations.size());
    cout<<"The price of the ride is: "<<price<<endl;
    SaveRideHistory(userLocation, destination);   // Add the ride to Ride History
    
}
//-------------------------------------Ride Request Queue Implementation--------------------------------------------
    void addToQueue(Queue& userQueue, int userID) {
        userQueue.enqueue(userID);

        // Save to file
        ofstream outFile("Files\\userQueue.txt", ios::app);
        if (outFile.is_open()) {
            outFile << userID << "\n";
            outFile.close();
           // cout << "User ID " << userID << " added to the file-based queue.\n";
        } else {
            cerr << "Unable to open userQueue.txt for writing.\n";
        }
    }
    void removeFromQueue(Queue& userQueue, int userID) {
        // Temporary queue for processing
        Queue tempQueue;

        // Process queue and rebuild it
        while (!userQueue.isEmpty()) {
            int currentID = userQueue.peek();
            userQueue.dequeue();

            if (currentID != userID) {
                tempQueue.enqueue(currentID);
            }
        }

        // Swap contents back to original queue
        while (!tempQueue.isEmpty()) {
            userQueue.enqueue(tempQueue.peek());
            tempQueue.dequeue();
        }

        // Update file
        updateQueueFile(userQueue);
    }

    // Load queue from file
    void loadQueue(Queue& userQueue) {
        std::ifstream inFile("Files\\userQueue.txt");
        if (!inFile.is_open()) {
            cerr << "Unable to open userQueue.txt for reading.\n";
            return;
        }

        string line;
        while (getline(inFile, line)) {
            if (!line.empty() && all_of(line.begin(), line.end(), ::isdigit)) {
                userQueue.enqueue(stoi(line));
            }
        }

        inFile.close();
    }

    // Update queue file
    void updateQueueFile(const Queue& userQueue) {
    std::ofstream outFile("Files\\userQueue.txt", std::ios::trunc);
    if (!outFile.is_open()) {
        cerr << "Unable to open userQueue.txt for writing.\n";
        return;
    }
    
    // Use the front pointer of the queue
    QueueNode* current = userQueue.front; // Ensure access via friend, getter, or public
    while (current) {
        outFile << current->data << "\n";
        current = current->next;
    }

    outFile.close();
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
    RideHistory RH;
    string time = get_current_time();
    string date = get_current_date();
    RH.addRide(ID, time, date, userLocation, destination);
    RH.saveToFile("Files\\RideHistory.txt");
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
#endif