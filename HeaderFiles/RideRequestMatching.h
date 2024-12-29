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
    ~RideRequestMatching(){}
    Graph getNustMap(){
    Graph nustMap;
    nustMap.addEdge("GATE 1", "NSHS", 840);
    nustMap.addEdge("NSHS", "GATE 1", 457);

    nustMap.addEdge("GATE 1", "NSTP", 650);
    nustMap.addEdge("NSTP", "GATE 1", 600);

    nustMap.addEdge("GATE 1", "JOHAR HOSTEL", 700);
    nustMap.addEdge("JOHAR HOSTEL", "GATE 1", 700);

    nustMap.addEdge("IQRA APARTMENTS", "NSHS", 550);
    nustMap.addEdge("NSHS", "IQRA APARTMENTS", 900);

    nustMap.addEdge("IQRA APARTMENTS", "GATE 1", 600);
    nustMap.addEdge("GATE 1","IQRA APARTMENTS", 800);

    nustMap.addEdge("NSTP", "NSHS", 700);
    nustMap.addEdge("NSHS", "NSTP", 550);

    nustMap.addEdge("IQRA APARTMENTS", "JOHAR HOSTELS", 350);
    nustMap.addEdge("JOHAR HOSTELS", "IQRA APARTMENTS", 140);

    nustMap.addEdge("IQRA APARTMENTS", "NSTP", 240);
    nustMap.addEdge("NSTP", "IQRA APARTMENTS", 500);

    nustMap.addEdge("NSTP", "NMC", 300);
    nustMap.addEdge("NMC", "NSTP", 400);

    nustMap.addEdge("JOHAR HOSTELS", "NSTP", 350);
    nustMap.addEdge("NSTP", "JOHAR HOSTELS", 450);

    nustMap.addEdge("GATE 10", "NSTP", 850);
    nustMap.addEdge("NSTP", "GATE 10", 750);

    nustMap.addEdge("JOHAR HOSTELS", "GYM", 200);
    nustMap.addEdge("GYM", "JOHAR HOSTELS", 200);

    nustMap.addEdge("JOHAR HOSTELS", "SCME", 550);
    nustMap.addEdge("SCME", "JOHAR HOSTELS", 550);

    nustMap.addEdge("SCME", "C1", 600);
    nustMap.addEdge("C1", "SCME", 600);

    nustMap.addEdge("C1", "IESE", 300);
    nustMap.addEdge("IESE", "C1", 200);

    nustMap.addEdge("C1", "NBS", 290);
    nustMap.addEdge("NBS", "C1", 290);

    nustMap.addEdge("NBS", "GYM", 280);
    nustMap.addEdge("GYM", "NBS", 300);

    nustMap.addEdge("IESE", "ASAB", 288);
    nustMap.addEdge("ASAB", "IESE", 288);

    nustMap.addEdge("SADA", "IQBAL SQUARE", 450);
    nustMap.addEdge("IQBAL SQUARE", "SADA", 450);

    nustMap.addEdge("SADA", "C1", 22);
    nustMap.addEdge("C1", "SADA", 22);

    nustMap.addEdge("SADA", "NBS", 270);
    nustMap.addEdge("NBS", "SADA", 270);

    nustMap.addEdge("SADA", "ASAB", 450);
    nustMap.addEdge("ASAB", "SADA", 450);

    nustMap.addEdge("ASAB", "IQBAL SQUARE", 300);
    nustMap.addEdge("IQBAL SQUARE", "ASAB", 300);

    nustMap.addEdge("ASAB", "NBS", 700);
    nustMap.addEdge("NBS", "ASAB", 700);

    nustMap.addEdge("IQBAL SQUARE", "GATE 2", 450);
    nustMap.addEdge("GATE 2", "IQBAL SQUARE", 450);

    nustMap.addEdge("IQBAL SQUARE", "ADMIN BLOCK", 180);
    nustMap.addEdge("ADMIN BLOCK", "IQBAL SQUARE", 130);

    nustMap.addEdge("GYM", "S3H", 130);
    nustMap.addEdge("S3H", "GYM", 130);

    nustMap.addEdge("GYM", "CIPS", 450);
    nustMap.addEdge("CIPS", "GYM", 450);

    nustMap.addEdge("GIRLS HOSTELS", "GYM", 130);
    nustMap.addEdge("GYM", "GIRLS HOSTELS", 130);

    nustMap.addEdge("S3H", "NBS", 290);
    nustMap.addEdge("NBS", "S3H", 337);

    nustMap.addEdge("CIPS", "MAIN OFFICE", 230);
    nustMap.addEdge("MAIN OFFICE", "CIPS", 230);

    nustMap.addEdge("CIPS", "GATE 10", 850);
    nustMap.addEdge("GATE 10", "CIPS", 850);

    nustMap.addEdge("CIPS", "NSTP", 700);
    nustMap.addEdge("NSTP", "CIPS", 600);

    nustMap.addEdge("CIPS", "NMC", 450);
    nustMap.addEdge("NMC", "CIPS", 450);

    nustMap.addEdge("MAIN OFFICE", "C3", 72);
    nustMap.addEdge("C3", "MAIN OFFICE", 72);

    nustMap.addEdge("MAIN OFFICE", "CENTRAL LIBRARY", 140);
    nustMap.addEdge("CENTRAL LIBRARY", "MAIN OFFICE", 140);

    nustMap.addEdge("CENTRAL LIBRARY", "SEECS", 1000);
    nustMap.addEdge("SEECS", "CENTRAL LIBRARY", 950);

    nustMap.addEdge("SEECS", "C2", 190);
    nustMap.addEdge("C2", "SEECS", 190);

    nustMap.addEdge("SEECS", "IAEC", 350);
    nustMap.addEdge("IAEC", "SEECS", 350);

    nustMap.addEdge("C2", "RUMI CHOWK", 450);
    nustMap.addEdge("RUMI CHOWK", "C2", 450);

    nustMap.addEdge("C2", "IAEC", 250);
    nustMap.addEdge("IAEC", "C2", 250);

    nustMap.addEdge("IAEC", "MASJID", 210);
    nustMap.addEdge("MASJID", "IAEC", 210);

    nustMap.addEdge("MASJID", "ADMIN BLOCK", 82);
    nustMap.addEdge("ADMIN BLOCK", "MASJID", 130);

    nustMap.addEdge("RIMMS", "MASJID", 150);
    nustMap.addEdge("MASJID", "RIMMS", 150);

    nustMap.addEdge("RIMMS", "IAEC", 52);
    nustMap.addEdge("IAEC", "RIMMS", 52);

    nustMap.addEdge("MASJID", "FATIMA HOSTELS", 130);
    nustMap.addEdge("FATIMA HOSTELS", "MASJID", 130);

    nustMap.addEdge("FATIMA HOSTELS", "RUMI CHOWK", 170);
    nustMap.addEdge("RUMI CHOWK", "FATIMA HOSTELS", 170);

    nustMap.addEdge("RUMI CHOWK", "NICE", 210);
    nustMap.addEdge("NICE", "RUMI CHOWK", 210);

    nustMap.addEdge("RUMI CHOWK", "USPCASE", 160);
    nustMap.addEdge("USPCASE", "RUMI CHOWK", 160);

    nustMap.addEdge("RUMI CHOWK", "SIR SYED MESS", 250);
    nustMap.addEdge("SIR SYED MESS", "RUMI CHOWK", 250);

    nustMap.addEdge("RUMI CHOWK", "BOYS HOSTELS", 290);
    nustMap.addEdge("BOYS HOSTELS", "RUMI CHOWK", 290);

    nustMap.addEdge("NICE", "GATE 4", 160);
    nustMap.addEdge("GATE 4", "NICE", 160);

    nustMap.addEdge("NICE", "USPCASE", 230);
    nustMap.addEdge("USPCASE", "NICE", 240);

    nustMap.addEdge("USPCASE", "GATE 4", 250);
    nustMap.addEdge("GATE 4", "USPCASE", 230);
    
    nustMap.addEdge("SIR SYED MESS", "BOYS HOSTELS", 200);
    nustMap.addEdge("BOYS HOSTELS", "SIR SYED MESS", 200);

    nustMap.addEdge("BOYS HOSTELS", "RETRO", 79);
    nustMap.addEdge("RETRO", "BOYS HOSTELS", 79);

    nustMap.addEdge("RETRO", "OLD SNS", 400);
    nustMap.addEdge("OLD SNS", "RETRO", 400);

    nustMap.addEdge("RETRO", "NEW SNS", 450);
    nustMap.addEdge("NEW SNS", "RETRO", 450);

    nustMap.addEdge("RETRO", "SMME", 400);
    nustMap.addEdge("SMME", "RETRO", 400);

    nustMap.addEdge("OLD SNS", "NCAI", 67);
    nustMap.addEdge("NCAI", "OLD SNS", 67);

    nustMap.addEdge("OLD SNS", "NEW SNS", 190);
    nustMap.addEdge("NEW SNS", "OLD SNS", 190);

    nustMap.addEdge("OLD SNS", "SMME", 72);
    nustMap.addEdge("SMME", "OLD SNS", 72);

    nustMap.addEdge("OLD SNS", "SPORTS COMPLEX", 800);
    nustMap.addEdge("SPORTS COMPLEX", "OLD SNS", 800);

    nustMap.addEdge("SMME", "SPORTS COMPLEX", 850);
    nustMap.addEdge("SPORTS COMPLEX", "SMME", 850);

    nustMap.addEdge("RESIDENTIAL AREA", "OLD SNS", 1200);
    nustMap.addEdge("OLD SNS", "RESIDENTIAL AREA", 1200);

    nustMap.addEdge("NEW SNS", "NCAI", 200);
    nustMap.addEdge("NCAI", "NEW SNS", 200);

    nustMap.addEdge("NEW SNS", "SMME", 120);
    nustMap.addEdge("SMME", "NEW SNS", 120);

    nustMap.addEdge("SMME", "NCAI", 77);
    nustMap.addEdge("NCAI", "SMME", 77);

    nustMap.addEdge("SMME", "RESIDENTIAL AREA", 1200);
    nustMap.addEdge("RESIDENTIAL AREA", "SMME", 1200);

    nustMap.addEdge("NCAI", "RESIDENTIAL AREA", 1200);
    nustMap.addEdge("RESIDENTIAL AREA", "NCAI", 1200);

    nustMap.addEdge("RESIDENTIAL AREA", "NMC", 450);
    nustMap.addEdge("NMC", "RESIDENTIAL AREA", 450);

    nustMap.addEdge("RESIDENTIAL AREA", "SPORTS COMPLEX", 750);
    nustMap.addEdge("SPORTS COMPLEX", "RESIDENTIAL AREA", 750);

    nustMap.addEdge("NMC", "GATE 10", 450);
    nustMap.addEdge("GATE 10", "NMC", 450);

    nustMap.addEdge("RESIDENTIAL AREA", "GATE 10", 850);
    nustMap.addEdge("GATE 10", "RESIDENTIAL AREA", 850);

    nustMap.addEdge("IQBAL SQUARE", "JOHAR HOSTELS", 800);
    nustMap.addEdge("JOHAR HOSTELS", "IQBAL SQUARE", 800);

    nustMap.addEdge("IQBAL SQUARE", "RUMI CHOWK", 500);
    nustMap.addEdge("RUMI CHOWK", "IQBAL SQUARE", 500);

    nustMap.addEdge("IQBAL SQUARE", "NBS", 550);
    nustMap.addEdge("NBS", "IQBAL SQUARE", 550);
    
    nustMap.addEdge("GATE 1", "JOHAR HOSTELS", 700);
    nustMap.addEdge("JOHAR HOSTELS","GATE 1", 700);
    
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
        for (size_t i = 0; i < locations.size(); ++i) {
            cout << i + 1 << ". " << locations[i] << "\n";
        }
    }
//   bool hanldeRide(int user_id,string userLocation, string destination,vector<int> driver_ids, vector<string> driverLocations,string & selected_driver,int numStops, vector <string> stops){
//         Graph nustMap = getNustMap();
//         // Here we will call the handleRide function from the Graph class nustMap
//        return nustMap.handleRide(user_id, userLocation, destination,driver_ids, driverLocations,selected_driver,numStops,stops);
//     }
};
