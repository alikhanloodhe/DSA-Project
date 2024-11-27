#include<iostream>
#include<cmath>
#include "RideRequestMatching.h"
using namespace std;
class RidePrice{
    public:
    RidePrice(){}; // Constructor
    int dynamicRidePrice(string current_location,string destination, string time, int no_of_available_drivers){
        // We are given the start and end location of the user only....
        // We have to calculate the distance between the two locations
        RideRequestMatching RRM;
        Graph nustMap = RRM.getNustMap();
        pair<int, vector<string>> path = nustMap.calculateShortestPath(current_location, destination); // The function will  calculate shortestdistance !
        int distance = path.first; // Now we have the shortest distance in between the two locations
        cout<<distance<<endl;
        int fuel_price = 250; // fuel price per litre
        int mileage = 1500; // mileage of the vehicle 
        float actual_ride_price = ((float)distance/mileage)*fuel_price; // Actual price of the ride
        float profit = 0.3*actual_ride_price; // 30% profit to the rider/driver
        if(time >= "06:00:00 AM" && time <= "10:00:00 AM" || time >= "05:00:00 PM" && time <= "10:00:00 PM"){ // Ride pricing will depend upon dist-- time of the day-- maybe which day
            actual_ride_price += 0.25*actual_ride_price; // 25% increase in the price  // This is due to high traffic delays and more time consumption of the rider
        }
        
        if(no_of_available_drivers < 5){  // ride price will depend no of drivers available //  // Less no of drivers leave less options for the user and it shows that the demand is more and supply is less so the price will be increased
            actual_ride_price += 0.15*actual_ride_price; // 15% increase in the price
        }
        float ride_price = actual_ride_price + profit; // Final price of the ride
        return round(ride_price); // Rounding off the price to the nearest integer
        // It should also cater multi-stop option --
        // At each additional stop a 5 min time will be given to the user. Each additional stop will costs 30% inc to ride pricing
        // If it is out of the way of the rider the price will be separately adjusted for both of them 
    }
};
