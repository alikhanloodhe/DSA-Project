#include<iostream>
#include<sstream>
#include<fstream>
#include<string>
#include<algorithm>
#include "Queue.h"
class RideRequestQueue{
    public:
    RideRequestQueue(){}
    void addToQueue(Queue& userQueue, int userID) {
        userQueue.enqueue(userID);

        // Save to file
        ofstream outFile("Files\\userQueue.txt", ios::app);
        if (outFile.is_open()) {
            outFile << userID << "\n";
            outFile.close();
           // cout << "User ID " << userID << " added to the file-based queue.\n";
        } else {
            cerr << "Unable to open userQueue.txt for writing.\n";
        }
    }
    void removeFromQueue(Queue& userQueue, int userID) {
        // Temporary queue for processing
        Queue tempQueue;

        // Process queue and rebuild it
        while (!userQueue.isEmpty()) {
            int currentID = userQueue.peek();
            userQueue.dequeue();

            if (currentID != userID) {
                tempQueue.enqueue(currentID);
            }
        }

        // Swap contents back to original queue
        while (!tempQueue.isEmpty()) {
            userQueue.enqueue(tempQueue.peek());
            tempQueue.dequeue();
        }

        // Update file
        updateQueueFile(userQueue);
    }

    // Load queue from file
    void loadQueue(Queue& userQueue) {
        std::ifstream inFile("Files\\userQueue.txt");
        if (!inFile.is_open()) {
            cerr << "Unable to open userQueue.txt for reading.\n";
            return;
        }

        string line;
        while (getline(inFile, line)) {
            if (!line.empty() && all_of(line.begin(), line.end(), ::isdigit)) {
                userQueue.enqueue(stoi(line));
            }
        }

        inFile.close();
    }

    // Update queue file
    void updateQueueFile(const Queue& userQueue) {
    std::ofstream outFile("Files\\userQueue.txt", std::ios::trunc);
    if (!outFile.is_open()) {
        cerr << "Unable to open userQueue.txt for writing.\n";
        return;
    }
    
    // Use the front pointer of the queue
    QueueNode* current = userQueue.getFront(); // Ensure access via friend, getter, or public
    while (current) {
        outFile << current->data << "\n";
        current = current->next;
    }

    outFile.close();
}
};