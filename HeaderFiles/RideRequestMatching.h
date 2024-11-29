#include <iostream>
#include <map>
#include <vector>
#include <string>
#include <queue>
#include <climits>
#include <algorithm> // Required for reverse
#include "Graph.h"
class RideRequestMatching {
    public:
    RideRequestMatching() {}
    Graph getNustMap(){
    Graph nustMap;
    nustMap.addEdge("Gate 1", "NSHS", 500);
    nustMap.addEdge("Gate 1", "NSTP", 750);
    nustMap.addEdge("Gate 1", "Johar Hostel", 1100);
    nustMap.addEdge("Iqra Apartments", "NSHS", 550);
    nustMap.addEdge("NSTP", "NSHS", 550);
    nustMap.addEdge("Iqra Apartments", "Johar Hostels", 450);
    nustMap.addEdge("Iqra Apartments", "NSTP", 290);
    nustMap.addEdge("NSTP", "NMC", 300);
    nustMap.addEdge("Johar Hostels", "NSTP", 700);
    nustMap.addEdge("Gate 10", "NSTP", 750);
    nustMap.addEdge("Johar Hostels", "Gym", 290);

    nustMap.addEdge("Johar Hostels", "SCME", 650);
    nustMap.addEdge("SCME", "C1", 270);
    nustMap.addEdge("C1", "IESE", 160);
    nustMap.addEdge("C1", "NBS", 200);
    nustMap.addEdge("IESE", "ASAB", 300);
    nustMap.addEdge("ASAB", "Iqbal Square", 400);
    nustMap.addEdge("ASAB", "NBS", 614);
    nustMap.addEdge("Iqbal Square", "Gate 2", 550);
    nustMap.addEdge("Iqbal Square", "Admin Block", 110);
    nustMap.addEdge("Gym", "S3H", 130);
    nustMap.addEdge("Gym", "CIPS", 390);
    nustMap.addEdge("Girls Hostels", "Gym", 50);
    nustMap.addEdge("S3H", "NBS", 180);
    nustMap.addEdge("CIPS", "Main Office", 156);
    nustMap.addEdge("CIPS", "Gate 10", 850);
    nustMap.addEdge("CIPS", "NSTP", 600);
    nustMap.addEdge("CIPS", "NMC", 450);
    nustMap.addEdge("Main Office", "C3", 72);
    nustMap.addEdge("Main Office", "Central Library", 120);
    nustMap.addEdge("Central Library", "SEECS", 500);
    nustMap.addEdge("SEECS", "C2",190);
    nustMap.addEdge("SEECS", "IAEC", 400);
    nustMap.addEdge("C2", "Rumi Chowk", 450);
    nustMap.addEdge("C2", "IAEC", 290);
    nustMap.addEdge("IAEC", "Masjid", 230);
    nustMap.addEdge("Masjid", "Admin Block", 120);
    nustMap.addEdge("Masjid", "Fatima Hostels", 82);
    nustMap.addEdge("Fatima Hostels", "Rumi Chowk", 170);
    nustMap.addEdge("Rumi Chowk", "NICE", 230);
    nustMap.addEdge("Rumi Chowk", "USPCASE", 130);
    nustMap.addEdge("Rumi Chowk", "Sir Syed Mess", 230);
    nustMap.addEdge("NICE", "Gate 4", 160);
    nustMap.addEdge("NICE", "USPCASE", 240);
    nustMap.addEdge("USPCASE", "Gate 4",234);
    nustMap.addEdge("Sir Syed Mess", "Boys Hostels", 270);
    nustMap.addEdge("Boys Hostels", "Retro", 120);
    nustMap.addEdge("Retro", "Old SNS", 400);
    nustMap.addEdge("Retro", "New SNS", 450);
    nustMap.addEdge("Retro", "SMME", 520);
    nustMap.addEdge("Old SNS", "NCAI", 67);
    nustMap.addEdge("Old SNS", "New SNS", 110);
    nustMap.addEdge("Old SNS", "SMME", 190);
    nustMap.addEdge("Old SNS", "Sports Complex", 800);
    nustMap.addEdge("New SNS", "NCAI", 80);
    nustMap.addEdge("New SNS", "SMME", 120); 
    nustMap.addEdge("SMME", "NCAI",197);
    nustMap.addEdge("SMME", "Residential Area", 1200);
    nustMap.addEdge("NCAI", "Residential Area",1200);
    nustMap.addEdge("Residential Area", "NMC", 280);
    nustMap.addEdge("Residential Area", "Sports Complex", 650);
    nustMap.addEdge("NMC", "Gate 10", 450);
    return nustMap;
    }
    vector<string> get_All_Locations(){
    vector<string> locations = {"Gate 1", "NSHS",
                                "NSTP", "Johar Hostels",
                                "Iqra Apartments", "Gate 10",
                                "Girls Hostels", "Gym",
                                "S3H", "CIPS",
                                "Main Office", "Central Library",
                                "SEECS", "NBS",
                                "C2", "IAEC",
                                "C1", "Fatima Hostels",
                                "Masjid", "ASAB",
                                "IESE", "SCME",
                                "Iqbal Square", "Gate 2",
                                "Admin Block", "NICE",
                                "USPCASE", "Rumi Chowk",
                                "Sir Syed Mess", "Boys Hostels",
                                "Retro", "Old SNS",
                                "New SNS",
                                "SMME", "NCAI",
                                "Residential Area"
                                };
    return locations;
}
    void print_all_loc(){
        int linecount = 0;
        vector<string> locations = get_All_Locations();
        for (int i = 0; i<locations.size();i++){
            cout<<locations[i]<<",  ";
            linecount++;
            if(linecount%5==0){
            cout<<endl;
        }
    }
    cout<<endl;
    }
    void hanldeRide(string userLocation, string destination, vector<string> driverLocations,string & nearest_driver){
        Graph nustMap = getNustMap();
        nustMap.handleRide(userLocation, destination, driverLocations,nearest_driver);
    }
};
