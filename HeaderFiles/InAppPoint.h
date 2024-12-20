#ifndef IN_APP_POINT_H
#define IN_APP_POINT_H

#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <algorithm>
#include "rideHistory.h"
using namespace std;

struct PointNode {
    int userID;
    int points;
    PointNode* next;

    PointNode(int id, int pts) : userID(id), points(pts), next(nullptr) {}
};

class InAppPoint {
private:
    PointNode* head;
    const string filename = "Files\\InAppPoint.txt";

    // Load points for all users from the file into a linked list
    void loadPointsFromFile() {
        ifstream file(filename);
        string line;

        while (getline(file, line)) {
            stringstream ss(line);
            int fileUserID, filePoints;
            ss >> fileUserID >> filePoints;

            // Add to linked list
            PointNode* newNode = new PointNode(fileUserID, filePoints);
            newNode->next = head;
            head = newNode;
        }
    }

    // Clear the linked list to avoid stale data
    void clearPointsList() {
        while (head) {
            PointNode* temp = head;
            head = head->next;
            delete temp;
        }
        head = nullptr;
    }

    // Update points in the file
    void updatePointsInFile(int userID, int updatedPoints) {
        vector<pair<int, int>> users;
        ifstream inFile(filename);
        string line;
        bool userExists = false;

        while (getline(inFile, line)) {
            stringstream ss(line);
            int fileUserID, filePoints;
            ss >> fileUserID >> filePoints;

            if (fileUserID == userID) {
                filePoints = updatedPoints;
                userExists = true;
            }
            users.emplace_back(fileUserID, filePoints);
        }
        inFile.close();

        if (!userExists) {
            users.emplace_back(userID, updatedPoints);
        }

        ofstream outFile(filename, ios::trunc);
        for (const auto& user : users) {
            outFile << user.first << " " << user.second << endl;
        }
    }

public:
    // Constructor
    InAppPoint() : head(nullptr) {}

    // Destructor
    ~InAppPoint() {
        clearPointsList();
    }

    // Show points of a specific user
    void showInAppPoints(int userID) {
        clearPointsList();
        loadPointsFromFile();
        PointNode* current = head;
        while (current) {
            if (current->userID == userID) {
                cout << "You have " << current->points << " points in your wallet" << endl;
                return;
            }
            current = current->next;
        }
        cout << "You Don't have any points!!\nBook Rides to daily to get points and amazing discounts on rides :)"<< endl;
    }

    // Add points to a specific user
    void addPoints(int userID, int pointsToAdd) {
        RideHistory rh;
        int rideCount = rh.getRidesCountForToday(userID);

        if (rideCount <= 1) {
            //cout << "User ID " << userID << " has not completed enough rides today to earn points." << endl;
            return;
        }

        vector<pair<int, int>> users;
        ifstream inFile(filename);
        bool userExists = false;
        string line;

        while (getline(inFile, line)) {
            stringstream ss(line);
            int fileUserID, filePoints;
            ss >> fileUserID >> filePoints;

            if (fileUserID == userID) {
                filePoints += pointsToAdd;
                userExists = true;
            }
            users.emplace_back(fileUserID, filePoints);
        }
        inFile.close();

        if (!userExists) {
            users.emplace_back(userID, pointsToAdd);
        }

        ofstream outFile(filename, ios::trunc);
        for (const auto& user : users) {
            outFile << user.first << " " << user.second << endl;
        }
    }

    // Redeem points for a ride
    void redeemPoints(int userID, int pointsToRedeem,int ridePrice) {
        clearPointsList();
        loadPointsFromFile();
        PointNode* current = head;
        while (current) {
            if (current->userID == userID) {
                int pointsAvailable = current->points;
                if(pointsAvailable<pointsToRedeem){
                    cout<<"Sorry Dear User!!\nYou don't have enough points to redeem.\nTake Rides daily to get maximum points"<<endl;
                    return;
                }
                else if (pointsAvailable >= pointsToRedeem && pointsToRedeem>ridePrice) {
                    pointsAvailable -= pointsToRedeem;
                    updatePointsInFile(userID, pointsAvailable);
                    cout << "Ride fully paid using points. Remaining points: " << pointsAvailable << endl;
                } else {
                    pointsAvailable -= pointsToRedeem;
                    int remainingAmount = ridePrice - pointsToRedeem;
                    updatePointsInFile(userID, pointsAvailable);
                    cout << "Points used: " << pointsAvailable << ". Remaining to pay: " << remainingAmount << endl;
                }
                return;
            }
            current = current->next;
        }
        cout << "Sorry Dear User!!\n You have no points to redeem.\nTake Rides daily to get maximum points" << endl;
    }
    void checkOutUsingPoints(int id,int ridePrice){
        showInAppPoints(id);
        cout << "How many points you want to redeem: " << endl;
        int points;
        cin >> points;
        redeemPoints(id, points, ridePrice);
    }
    int getCurrentpoints(int id){
        clearPointsList();
        loadPointsFromFile();
        PointNode* current = head;
        while (current) {
            if (current->userID == id) {
                return current->points;
            }
            current = current->next;
        }
        return 0;
    }
};

#endif // IN_APP_POINT_H
