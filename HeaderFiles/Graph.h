#include <iostream>
#include <map>
#include <vector>
#include <string>
#include <queue>
#include <climits>
#include <algorithm> // Required for reverse
#include <unordered_map>
#include "ErrorHandling.h"

using namespace std;

class Graph {
    // vector<string> all_driver_names;
    unordered_map<int, string> driver_data;
    // vector<string> all_user_names;
    unordered_map<int, string> user_data;
private:
    map<string, map<string, int>> adjList;  
    int totalDistance;
public:
    Graph(){ 
        totalDistance = 0;
        load_all_driver_data();
        load_all_user_data();
    }
    void addEdge(const string& node1, const string& node2, int weight) {  
    adjList[node1][node2] = weight; // Add a directed edge from node1 to node2 with given weight  
}  
    
    // Using Dijkstra Algorithm to calculate shortest path
    pair<int, vector<string>> calculateShortestPath(const string& start, const string& end) {
        priority_queue<pair<int, string>, vector<pair<int, string>>, greater<>> pq;
        map<string, int> distances;
        map<string, string> parents;

        for (const auto& pair : adjList) {
            distances[pair.first] = INT_MAX; // INT_MAX refers to maximum value of integers
            parents[pair.first] = "";
        }

        distances[start] = 0;
        pq.emplace(0, start);

        while (!pq.empty()) {
            pair<int, string> current = pq.top();
            pq.pop();
            int currentDist = current.first;
            string currentNode = current.second;

            if (currentDist > distances[currentNode]) continue;

            for (const auto& neighbor : adjList[currentNode]) {
                string neighborNode = neighbor.first;
                int weight = neighbor.second;
                int newDist = currentDist + weight;

                if (newDist < distances[neighborNode]) {
                    distances[neighborNode] = newDist;
                    parents[neighborNode] = currentNode;
                    pq.emplace(newDist, neighborNode);
                }
            }
        }

        if (distances[end] == INT_MAX) { // If there is no  path exists to that node the distance in between returned - INT_MAX
            return {INT_MAX, {}};
        }

        vector<string> path;
        for (string at = end; !at.empty(); at = parents[at]) {
            path.push_back(at);
        }
        reverse(path.begin(), path.end());
        return {distances[end], path};
    }

    bool handleRide(int user_id, const string& userLocation, const string& destination,const vector<int>& driver_ids, const vector<string>& driverLocations, string & selected_Driver, int numStops, vector <string> stops) {
        // Step 1: Find the nearest driver

        // In this particular function we will give the user the leverage to select the driver of his own choice
        ErrorHandling ER;
        int choice_pick;
        cout<<"Would you like to select the driver of your own choice: \n1. Yes \n2. No"<<endl;
        choice_pick = ER.getValidint(1,2);
        // show drivers based on 
        vector<string> names;
        string temp_name;
        double temp_rating;
        vector<double> ratings;
        if(choice_pick == 1){
            cout<<"Available drivers are at: "<<endl;
            for(int i = 0;i<driver_ids.size();i++){
                get_driver_nR(driver_ids[i],temp_name,temp_rating); // Here hashmaps have affected
                names.push_back(temp_name);
                ratings.push_back(temp_rating);
                
            }
            for(int i = 0;i<names.size();i++){
                cout<<i+1<<". "<<names[i]<<" at "<<driverLocations[i]<<" with ratings "<<setprecision(2)<<ratings[i]<<endl;
            }
            int choice;
            cout<<"Which driver you want to select!!"<<endl;
            choice = ER.getValidint(1,names.size());
            ER.~ErrorHandling();
            // Here we will implement that waiting for the driver to accept the ride!!!
            send_request(user_id, choice,userLocation,destination,stops);
            if(check_acceptance()){
                cout<<"Ride accepted\n";
                clear_acceptance();
            vector<string> driverToUserPath;
            selected_Driver = driverLocations[choice-1];
            pair<int, vector<string>> result = calculateShortestPath(driverLocations[choice-1], userLocation);
            int distance = result.first;
            driverToUserPath = result.second;
            if(distance >0){
                cout << "Selected driver and will take " << distance << " meters to reach " << userLocation << "." << endl;
                print_driver_path(driverToUserPath);
            }
            else{
                cout<<"Selected Driver is at your Location!!\n";
            }
            }
            else{
                cout<<"Ride Rejected\n";
                clear_acceptance();
               return false; // Returning from the handle ride function
            }
            
        }
        else{
            string nearestDriver;
            vector<string> driverToUserPath;
            int distance;
            get_nearest_driver(nearestDriver,driverLocations,userLocation,driverToUserPath,distance);
            int driverID = find_Driver_Id(driver_ids,driverLocations,nearestDriver);
            send_request(user_id,driverID,userLocation,destination,stops);
            if(check_acceptance()){
                cout<<"Ride accepted!!!\n";
                if(distance >0){
                cout << "Nearest driver will take " << distance << " meters to reach " << userLocation << "." << endl;
                print_driver_path(driverToUserPath);
                }else{
                    cout<<"Nearest driver is at you Location!!\n";
                }
                clear_acceptance();
            }
            else{
                cout<<"Ride Rejected\n";
                clear_acceptance();
                return false;
            }
            selected_Driver = nearestDriver;
        
        }

        string currentLocation = userLocation;

        for (int i = 0; i <= numStops; i++) {
            string nextLocation = (i < numStops) ? stops[i] : destination;
            pair<int, vector<string>> result = calculateShortestPath(currentLocation, nextLocation);

            if (result.first == INT_MAX) {
                cout << "No valid path exists from " << currentLocation << " to " << nextLocation << "." << endl;
                return false;
            }

            totalDistance += result.first;

            cout << "Path from " << currentLocation << " to " << nextLocation << ": ";
            for (size_t j = 0; j < result.second.size(); ++j) {
                cout << result.second[j];
                if (j < result.second.size() - 1) cout << " -> ";
                Sleep(1000);
            }
            cout << " (" << result.first << " meters)\n";

            currentLocation = nextLocation;
        }
        
        cout << "\nTotal distance for the journey is " << totalDistance << " meters.\n";
        return true;

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
    void get_nearest_driver(string & nearestDriver,vector<string> driverLocations,string userLocation,vector<string> &driver_to_userPath, int & Distance){
        int minDistance = INT_MAX;
            vector<string> driverToUserPath;
        // this logic actually finds the nearest driver from all the available drivers and give its distance
            for (const auto& driver : driverLocations) {
            pair<int, vector<string>> result = calculateShortestPath(driver, userLocation);
            int distance = result.first;
            vector<string> path = result.second;
                if (distance < minDistance) {
                    minDistance = distance;
                    nearestDriver = driver;
                    driverToUserPath = path;
                }
            }
        if(minDistance == INT_MAX) {
            cout << "No driver available to reach " << userLocation << "." << endl;
            return;
        }
        Distance = minDistance;
        driver_to_userPath = driverToUserPath;
    }
    void print_driver_path(vector<string> driverToUserPath){
        cout << "Driver's path to user: ";
        for (size_t i = 0; i < driverToUserPath.size(); ++i) {
            cout << driverToUserPath[i];
            if (i < driverToUserPath.size() - 1) cout << " -> ";
            Sleep(1000);
        }
        cout << endl;
    }
    
    

    int get_total_Distance_with_Stops(){ // A getter method to get total distance for a particular ride
        return totalDistance;
    }

    void load_all_driver_data() {
    ifstream driver_info_file("Files\\drivers.txt");
    if (!driver_info_file.is_open()) {
        cerr << "Error opening driver information file.\n";
        return;
    }

    string line;
    while (getline(driver_info_file, line)) {
        stringstream ss(line);
        string idStr, name;
        getline(ss, idStr, ',');
        getline(ss, name, ',');

        name.erase(name.find_last_not_of(" \t\n\r\f\v") + 1); // Trim whitespace

        try {
            if (!idStr.empty()) {
                int id = stoi(idStr);
                driver_data[id] = name; // Store the ID and name in the map
            }
        } catch (invalid_argument& e) {
            cerr << "Invalid ID in file: " << idStr << endl;
        } catch (out_of_range& e) {
            cerr << "ID out of range in file: " << idStr << endl;
        }
    }

    driver_info_file.close();
}
    void get_driver_nR(int ID,string &name,double &rating){
        name = driver_data[ID]; // It will load all drivers into a vector and then search them constantly
        ifstream ratingFile("Files\\DriverRating.txt");
        if (!ratingFile.is_open()) {
            cerr << "Error: Could not open DriverRating.txt for reading." << endl;
            return;
        }
        string line2;
        vector<int> ratings;
        while (getline(ratingFile, line2)) {
            stringstream ss(line2);
            string idStr, ratingStr;
            getline(ss, idStr, ':');  // Split the line into driverID and rating
            getline(ss, ratingStr);

            if (stoi(idStr) == ID) {  // Check if the driver ID matches
                ratings.push_back(stoi(ratingStr));  // Store the rating in the vector
            }
        }
        ratingFile.close();
        if (ratings.empty()) {
            rating = 0;
        } else {
            double avgRating = 0;
            for (int r : ratings) {
                avgRating += r;  // Calculate the average rating
            }
            avgRating /= ratings.size();
            rating = avgRating;
        } 
    }
    void clear_user_request(){
        ofstream file("Files\\user_request.txt", ios::trunc); // Open the file in truncate mode
        if (file.is_open()) {
            file.close();
        } else {
            cerr << "Error: Unable to open file for clearing "  << endl;
        }
    }
    //----------------------------------------------------------------------------------
    void clear_acceptance(){
        ofstream file("Files\\ride_accept.txt", ios::trunc); // Open the file in truncate mode
        if (file.is_open()) {
            file.close();
        } else {
            cerr << "Error: Unable to open file for clearing "  << endl;
        }
    }
    //-------------------------------------------------------------------------------------
    void send_request(int user_id, int driver_id, const string& userLocation, const string& destination, const vector<string>& stops) {
    // Retrieve user name
    string name = user_data[user_id];

    // Open file in append mode
    ofstream outFile("Files\\user_request.txt", ios::app);
    if (outFile.is_open()) {
        // Write data in comma-separated format
        outFile << user_id << "," << name << "," << userLocation << "," << destination << "," << driver_id;

        // Add stops (if any)
        if (!stops.empty()) {
            outFile << ",Stops: ";
            for (size_t i = 0; i < stops.size(); ++i) {
                outFile << stops[i];
                if (i != stops.size() - 1) { // Add separator for all but the last stop
                    outFile << "|";
                }
            }
        }

        outFile << "\n"; // End the line
        outFile.close();
    } else {
        cerr << "Unable to open file: Files\\user_request.txt\n";
    }
}

    bool check_acceptance() {
    cout<<"Waiting for the driver to respond....\n";
    using namespace chrono;
    
    auto start_time = steady_clock::now();
    const int timeout_seconds = 30;

    while (true) {
        // Check if timeout is reached
        auto current_time = steady_clock::now();
        if (duration_cast<seconds>(current_time - start_time).count() >= timeout_seconds) {
            cout << "Timeout reached. Ride rejected by default." << endl;
            clear_user_request();
            return false;
        }

        ifstream ride_accept_file("Files\\ride_accept.txt"); // Open the file
        if (!ride_accept_file.is_open()) {
            cerr << "Error opening ride_accept file.\n";
            return false;
        }

        string line;
        while (getline(ride_accept_file, line)) {
            stringstream ss(line);
            int response;
            ss >> response;

            if (response == 1) {
                return true;
            } else if (response == 0) {
                return false;
            }
        }

        ride_accept_file.close();

        // Wait for a short duration before checking again
        Sleep(500);
    }
}
    void load_all_user_data() {
    ifstream user_info_file("Files\\users.txt");
    if (!user_info_file.is_open()) {
        cerr << "Error opening user information file.\n";
        return;
    }

    string line;
    while (getline(user_info_file, line)) {
        stringstream ss(line);
        string idStr, name;

        // Parse ID
        getline(ss, idStr, ',');
        if (idStr.empty()) {
            cerr << "Invalid entry: Missing ID.\n";
            continue;
        }

        // Parse Name
        getline(ss, name, ',');
        if (name.empty()) {
            cerr << "Invalid entry: Missing Name for ID " << idStr << ".\n";
            continue;
        }

        // Trim whitespace from Name
        name.erase(name.find_last_not_of(" \t\n\r\f\v") + 1);
        name.erase(0, name.find_first_not_of(" \t\n\r\f\v"));

        // Store in the map
        try {
            int id = stoi(idStr);
            user_data[id] = name; // Store ID and Name in the map
        } catch (const invalid_argument& e) {
            cerr << "Invalid ID in file: " << idStr << endl;
        } catch (const out_of_range& e) {
            cerr << "ID out of range in file: " << idStr << endl;
        }
    }

    user_info_file.close();
}

};