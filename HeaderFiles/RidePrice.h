#include<iostream>
#include<cmath>
#include "RideRequestMatching.h"
using namespace std;
class RidePrice{
    public:
    RidePrice(){}; // Constructor
    int dynamicRidePrice(int total_distance, string time, int no_of_available_drivers) {
    int fuel_price = 250; // Cost per litre of fuel
    int mileage = 1500; // Vehicle mileage (meters per litre)

    // Calculate base ride cost based on total distance
    float base_price = ((float)total_distance / mileage) * fuel_price;

    // Add 30% profit margin
    float profit = 0.3 * base_price;

    // Apply surge pricing based on time
    if (time >= "06:00:00 AM" && time <= "10:00:00 AM" || time >= "05:00:00 PM" && time <= "10:00:00 PM") {
        base_price += 0.25 * base_price;
    }

    // Apply supply-demand pricing based on driver availability
    if (no_of_available_drivers < 5) {
        base_price += 0.15 * base_price;
    }

    // Final price = base price + profit
    return round(base_price + profit);
}
};
