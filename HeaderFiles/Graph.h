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
    map<string, vector<pair<string, int>>> adjList;

public:
    void addEdge(const string& node1, const string& node2, int weight) {
        adjList[node1].emplace_back(node2, weight);
        adjList[node2].emplace_back(node1, weight); // Bidirectional connection
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

    void handleRide(const string& userLocation, const string& destination, const vector<string>& driverLocations,string & nearest_Driver) {
        // Step 1: Find the nearest driver
        string nearestDriver;
        int minDistance = INT_MAX;
        vector<string> driverToUserPath;

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

        if (minDistance == INT_MAX) {
            cout << "No driver available to reach " << userLocation << "." << endl;
            return;
        }
        nearest_Driver = nearestDriver;
        // Step 2: Bring the driver to the user's location
        cout << "Nearest driver is at " << nearestDriver << " and will take " << minDistance << " meters to reach " << userLocation << "." << endl;
        cout << "Driver's path to user: ";
        for (size_t i = 0; i < driverToUserPath.size(); ++i) {
            cout << driverToUserPath[i];
            if (i < driverToUserPath.size() - 1) cout << " -> ";
        }
        cout << endl;

        // Step 3: Find the shortest path to the destination
        pair<int, vector<string>> destinationResult = calculateShortestPath(userLocation, destination);
        int userToDestinationDistance = destinationResult.first;
        vector<string> userToDestinationPath = destinationResult.second;

        if (userToDestinationDistance == INT_MAX) {
            cout << "No path exists from " << userLocation << " to " << destination << "." << endl;
            return;
        }

        cout << "Shortest distance from " << userLocation << " to " << destination << " is " << userToDestinationDistance << " meters." << endl;
        cout << "Path: ";
        for (size_t i = 0; i < userToDestinationPath.size(); ++i) {
            cout << userToDestinationPath[i];
            if (i < userToDestinationPath.size() - 1) cout << " -> ";
        }
        cout << endl;
    }
};