#include<iostream>
#include<cmath>
#include "RideRequestMatching.h"
using namespace std;
class DynamicRidePrice{
    public:
    DynamicRidePrice(){}; // Constructor
    int getdynamicRidePrice(int totalDistance, string time, int no_of_available_drivers,vector<string> stops){
        RideRequestMatching RRM;
        Graph nustMap = RRM.getNustMap();
        int distance = totalDistance;
        cout<<distance<<endl;
        int fuel_price = 250; // fuel price per litre
        int mileage = 1500; // mileage of the vehicle 
        float actual_ride_price = ((float)distance/mileage)*fuel_price; // Actual price of the ride
        int total_no_of_stops = stops.size();
        int multistop_extra_fare = actual_ride_price*total_no_of_stops*0.1; // 10% extra fare for each stop
        float profit = 0.3*actual_ride_price; // 30% profit to the rider/driver
        float Time_extra_fare = 0;
        if(time >= "06:00:00 AM" && time <= "10:00:00 AM" || time >= "05:00:00 PM" && time <= "10:00:00 PM"){ // Ride pricing will depend upon dist-- time of the day
            float Time_extra_fare = 0.25*actual_ride_price; // 25% increase in the price  // This is due to high traffic delays and more time consumption of the rider
        }
        float driver_extra_fare = 0;
        if(no_of_available_drivers < 3){  // ride price will depend no of drivers available //  // Less no of drivers leave less options for the user and it shows that the demand is more and supply is less so the price will be increased
            driver_extra_fare = 0.15*actual_ride_price; // 15% increase in the price
        }
        
        float ride_price = actual_ride_price +Time_extra_fare+driver_extra_fare+multistop_extra_fare + profit; // Final price of the ride
        return round(ride_price);
    }
    ~DynamicRidePrice(){}; // Destructor
};
