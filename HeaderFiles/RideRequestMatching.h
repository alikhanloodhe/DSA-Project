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
    nustMap.addEdge("GATE 1", "NSHS", 500);
    nustMap.addEdge("GATE 1", "NSTP", 750);
    nustMap.addEdge("GATE 1", "JOHAR HOSTELS", 1100);
    nustMap.addEdge("IQRA APARTMENTS", "NSHS", 550);
    nustMap.addEdge("NSTP", "NSHS", 550);
    nustMap.addEdge("IQRA APARTMENTS", "JOHAR HOSTELS", 450);
    nustMap.addEdge("IQRA APARTMENTS", "NSTP", 290);
    nustMap.addEdge("NSTP", "NMC", 300);
    nustMap.addEdge("JOHAR HOSTELS", "NSTP", 700);
    nustMap.addEdge("GATE 10", "NSTP", 750);
    nustMap.addEdge("JOHAR HOSTELS", "GYM", 290);
    nustMap.addEdge("JOHAR HOSTELS", "SCME", 650);
    nustMap.addEdge("SCME", "C1", 270);
    nustMap.addEdge("C1", "IESE", 160);
    nustMap.addEdge("C1", "NBS", 200);
    nustMap.addEdge("IESE", "ASAB", 300);
    nustMap.addEdge("ASAB", "IQBAL SQUARE", 400);
    nustMap.addEdge("ASAB", "NBS", 614);
    nustMap.addEdge("IQBAL SQUARE", "GATE 2", 550);
    nustMap.addEdge("IQBAL SQUARE", "ADMIN BLOCK", 110);
    nustMap.addEdge("GYM", "S3H", 130);
    nustMap.addEdge("GYM", "CIPS", 390);
    nustMap.addEdge("GIRLS HOSTEL", "GYM", 50);
    nustMap.addEdge("S3H", "NBS", 180);
    nustMap.addEdge("CIPS", "MAIN OFFICE", 156);
    nustMap.addEdge("CIPS", "GATE 10", 850);
    nustMap.addEdge("CIPS", "NSTP", 600);
    nustMap.addEdge("CIPS", "NMC", 450);
    nustMap.addEdge("MAIN OFFICE", "C3", 72);
    nustMap.addEdge("MAIN OFFICE", "CENTRAL LIBRARY", 120);
    nustMap.addEdge("CENTRAL LIBRARY", "SEECS", 500);
    nustMap.addEdge("SEECS", "C2",190);
    nustMap.addEdge("SEECS", "IAEC", 400);
    nustMap.addEdge("C2", "RUMI CHOWK", 450);
    nustMap.addEdge("C2", "IAEC", 290);
    nustMap.addEdge("IAEC", "MASJID", 230);
    nustMap.addEdge("MASJID", "ADMIN BLOCK", 120);
    nustMap.addEdge("MASJID", "FATIMA HOSTELS", 82);
    nustMap.addEdge("FATIMA HOSTELS", "RUMI CHOWK", 170);
    nustMap.addEdge("RUMI CHOWK", "NICE", 230);
    nustMap.addEdge("RUMI CHOWK", "USPCASE", 130);
    nustMap.addEdge("RUMI CHOWK", "SIR SYED MESS", 230);
    nustMap.addEdge("NICE", "GATE 4", 160);
    nustMap.addEdge("NICE", "USPCASE", 240);
    nustMap.addEdge("USPCASE", "GATE 4",234);
    nustMap.addEdge("SIR SYED MESS", "BOYS HOSTELS", 270);
    nustMap.addEdge("BOYS HOSTELS", "RETRO", 120);
    nustMap.addEdge("RETRO", "OLD SNS", 400);
    nustMap.addEdge("RETRO", "NEW SNS", 450);
    nustMap.addEdge("RETRO", "SMME", 520);
    nustMap.addEdge("OLD SNS", "NCAI", 67);
    nustMap.addEdge("OLD SNS", "NEW SNS", 110);
    nustMap.addEdge("OLD SNS", "SMME", 190);
    nustMap.addEdge("OLD SNS", "Sports Complex", 800);
    nustMap.addEdge("NEW SNS", "NCAI", 80);
    nustMap.addEdge("NEW SNS", "SMME", 120); 
    nustMap.addEdge("SMME", "NCAI",197);
    nustMap.addEdge("SMME", "RESIDENTIAL AREA", 1200);
    nustMap.addEdge("NCAI", "RESIDENTIAL AREA",1200);
    nustMap.addEdge("RESIDENTIAL AREA", "NMC", 280);
    nustMap.addEdge("RESIDENTIAL AREA", "SPORTS COMPLEX", 650);
    nustMap.addEdge("NMC", "GATE 10", 450);
    return nustMap;
    }
    vector<string> get_All_Locations(){
    vector<string> locations = {"GATE 1", "NSHS",
                                "NSTP", "JOHAR HOSTELS",
                                "IQRA APARTMENTS", "GATE 10",
                                "GIRLS HOSTEL", "GYM",
                                "S3H", "CIPS",
                                "MAIN OFFICE", "CENTRAL LIBRARY",
                                "SEECS", "NBS",
                                "C2", "IAEC",
                                "C1", "FATIMA HOSTELS",
                                "MASJID", "ASAB",
                                "IESE", "SCME",
                                "IQBAL SQUARE", "GATE 2",
                                "ADMIN BLOCK", "NICE",
                                "USPCASE", "RUMI CHOWK",
                                "SIR SYED MESS", "BOYS HOSTELS",
                                "RETRO", "OLD SNS",
                                "NEW SNS",
                                "SMME", "NCAI",
                                "RESIDENTIAL AREA"
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
    void hanldeRide(string userLocation, string destination, vector<string> driverLocations,string & selected_driver,int numStops, vector <string> stops){
        Graph nustMap = getNustMap();
        nustMap.handleRide(userLocation, destination, driverLocations,selected_driver,numStops,stops);
    }
};
