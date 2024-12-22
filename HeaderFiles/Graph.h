#include <iostream>
#include <map>
#include <vector>
#include <string>
#include <queue>
#include <climits>
#include <algorithm> // Required for reverse
using namespace std;

class Graph {
private:
    map<string, map<string, int>> adjList; // {fromNode: {toNode: forwardWeight}}  

public:
    // void addEdge(const string& node1, const string& node2, int weight) {
    //     adjList[node1].emplace_back(node2, weight);
    //     adjList[node2].emplace_back(node1, weight); // Bidirectional connection
    // }
    void addEdge(const string& node1, const string& node2, int weight) {  
    adjList[node1][node2] = weight; // Add a directed edge from node1 to node2 with given weight  
}  

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

        if (distances[end] == INT_MAX) {
            return {INT_MAX, {}};
        }

        vector<string> path;
        for (string at = end; !at.empty(); at = parents[at]) {
            path.push_back(at);
        }
        reverse(path.begin(), path.end());
        return {distances[end], path};
    }

    void handleRide(const string& userLocation, const string& destination,const vector<int>& driver_ids, const vector<string>& driverLocations, string & selected_Driver, int numStops, vector <string> stops) {
        // Step 1: Find the nearest driver

        // In this particular function we will give the user the leverage to select the driver of his own choice
        int choice_pick;
        cout<<"Would you like to select the driver of your own choice: \n1. Yes \n2. No"<<endl;
        cin>>choice_pick;
        // show drivers based on 
        vector<string> names;
        string temp_name;
        double temp_rating;
        vector<double> ratings;
        if(choice_pick == 1){
            cout<<"Available drivers are at: "<<endl;
            for(int i = 0;i<driver_ids.size();i++){
                get_driver_nR(driver_ids[i],temp_name,temp_rating);
                names.push_back(temp_name);
                ratings.push_back(temp_rating);
                
            }
            for(int i = 0;i<names.size();i++){
                cout<<i+1<<". "<<names[i]<<" at "<<driverLocations[i]<<" with ratings "<<ratings[i]<<endl;
            }
            int choice;
            cout<<"Which driver you want to select!!"<<endl;
            cin>>choice;
            cin.ignore();
            vector<string> driverToUserPath;
            selected_Driver = driverLocations[choice-1];
            pair<int, vector<string>> result = calculateShortestPath(driverLocations[choice-1], userLocation);
            int distance = result.first;
            cout <<  "The Driver" << " and will take " << distance << " meters to reach " << userLocation << "." << endl;
            driverToUserPath = result.second;
            print_driver_path(driverToUserPath);
        }
        else{
            string nearestDriver;
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
        selected_Driver = nearestDriver;
        cout << "Nearest driver is at " << nearestDriver << " and will take " << minDistance << " meters to reach " << userLocation << "." << endl;
        print_driver_path(driverToUserPath);
        }

        int totalDistance = 0;
        string currentLocation = userLocation;

        for (int i = 0; i <= numStops; ++i) {
            string nextLocation = (i < numStops) ? stops[i] : destination;
            pair<int, vector<string>> result = calculateShortestPath(currentLocation, nextLocation);

            if (result.first == INT_MAX) {
                cout << "No valid path exists from " << currentLocation << " to " << nextLocation << "." << endl;
                return;
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
    
    

    int get_total_Distance_with_Stops(const string& userLocation, const string& destination,vector <string> stops){
        int totalDistance = 0;
        string currentLocation = userLocation;

        for (int i = 0; i <= stops.size(); ++i) {
            string nextLocation = (i < stops.size()) ? stops[i] : destination;
            pair<int, vector<string>> result = calculateShortestPath(currentLocation, nextLocation);

            if (result.first == INT_MAX) {
                cout << "No valid path exists from " << currentLocation << " to " << nextLocation << "." << endl;
                return 0;
            }

            totalDistance += result.first;
             currentLocation = nextLocation;
    }
        return totalDistance;
    }
    void get_driver_nR(int ID,string &name,double &rating){
        ifstream driver_info_file("Files\\drivers.txt");
        if (!driver_info_file.is_open()) {
            cerr << "Error opening driver infromation file.\n";
            return;
        }
        string line;
        while (getline(driver_info_file, line)) {
        stringstream ss(line);
        string idStr, Name;
        getline(ss, idStr, ',');
        getline(ss, Name, ',');
        Name.erase(Name.find_last_not_of(" \t\n\r\f\v") + 1);// Trim whitespace from Name (to ensure clean data)
        try {
            if (!idStr.empty() && stoi(idStr) == ID) {  // Trim whitespace from Name (optional, to ensure clean data)
                name = Name; // Assign the matching name
                break;       // Exit loop once found
            }
        } catch (invalid_argument &e) {
            cerr << "Invalid ID in file: " << idStr << endl;
        } catch (out_of_range &e) {
            cerr << "ID out of range in file: " << idStr << endl;
        }
    }

    driver_info_file.close();

        // Now using rating
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

};