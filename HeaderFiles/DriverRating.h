#ifndef DRIVER_RATING_H
#define DRIVER_RATING_H
#include<iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <limits>

using namespace std;

class DriverRating{
    public:
    DriverRating(){
    }

    void UserRating(int driverID) {
        int rating;
        cout << "\nKindly rate the ride experience from 1 to 5:"<<endl;
        cout<<"1. Poor"<<endl;
        cout<<"2. Fair"<<endl;
        cout<<"3. Good"<<endl;
        cout<<"4. Very Good"<<endl;
        cout<<"5. Excellent"<<endl;

        // Input validation loop
        while (true) {
            cin >> rating;

            // Check if the input is a valid number and within the range 1 to 5
            if (cin.fail() || rating < 1 || rating > 5) {
                cin.clear(); // Clear the error state
                cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Discard invalid input
                cout << "Invalid input. Please enter a rating between 1 and 5: ";
            } else {
                break; // Valid input, exit loop
            }
        }

        // Clear the buffer to remove any leftover newline character
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        // Store the rating in DriverRating.txt
        ofstream ratingFile("Files\\DriverRating.txt", ios::app);
        if (ratingFile.is_open()) {
            ratingFile << driverID << ":" << rating << endl; // Append rating to file
            ratingFile.close();
        } else {
            cerr << "Error: Could not open DriverRating.txt for writing." << endl;
            return;
        }

        cout << "Thank you for rating the ride!\n"; // Confirmation message
    }


    
    void ShowDriverRating(int driverID) {
        ifstream ratingFile("Files\\DriverRating.txt");
        if (!ratingFile.is_open()) {
            cerr << "Error: Could not open DriverRating.txt for reading." << endl;
            return;
        }

        string line;
        vector<int> ratings;
        while (getline(ratingFile, line)) {
            stringstream ss(line);
            string idStr, ratingStr;
            getline(ss, idStr, ':');  // Split the line into driverID and rating
            getline(ss, ratingStr);

            if (stoi(idStr) == driverID) {  // Check if the driver ID matches
                ratings.push_back(stoi(ratingStr));  // Store the rating in the vector
            }
        }
        ratingFile.close();

        if (ratings.empty()) {
            cout << "No ratings available for Driver ID " << driverID << "." << endl;
        } else {
            cout << "Ratings for Driver ID " << driverID << ": ";
            for (int r : ratings) {
                cout << r << " ";  // Print all ratings for this driver
            }
            cout << endl;

            double avgRating = 0;
            for (int r : ratings) {
                avgRating += r;  // Calculate the average rating
            }
            avgRating /= ratings.size();
            
            cout << "Average Rating: " <<setprecision(2)<< avgRating << endl;
        }
    }

};
#endif