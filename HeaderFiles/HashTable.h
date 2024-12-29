#ifndef HASHTABLE_H
#define HASHTABLE_H

#include <iostream>
#include <vector>
#include <list>
#include <string>
#include <functional>
using namespace std;

// HashNode class for storing user data
template <typename T>
class HashNode {
public:
    T data;
    HashNode(const T& value) : data(value) {}
};

// HashTable class
template <typename T>
class HashTable {
private:
    vector<list<HashNode<T>>> table; // Vector of linked lists for chaining
    int capacity;                   // Total number of buckets

    // Hash function to calculate index
    int hash(const string& key) const {
        std::hash<std::string> hashFn;
        return hashFn(key) % capacity;
    }

public:
    HashTable(int size = 100) : capacity(size) {
        table.resize(capacity);
    }

    // Insert data into the hash table
    void insert(const T& value) {
        int index = hash(value.username);
        table[index].emplace_back(value);
    }

    // Find a user by username
    T* find(const string& username) {
        int index = hash(username);
        for (auto& node : table[index]) {
            if (node.data.username == username) {
                return &node.data;
            }
        }
        return nullptr; // Not found
    }

    // Check if a username exists
    bool usernameExists(const string& username) {
        return find(username) != nullptr;
    }

    // Load data from a file into the hash table
    void loadFromFile(const string& filename) {
        ifstream inFile(filename);
        if (!inFile) {
            cerr << "Error opening file for reading: " << filename << endl;
            return;
        }
        string line;
        while (getline(inFile, line)) {
            T data;
            data.deserialize(line);
            insert(data);
        }
        inFile.close();
    }

    // Save hash table data to a file
    void saveToFile(const string& filename) {
        ofstream outFile(filename);
        if (!outFile) {
            cerr << "Error opening file for writing: " << filename << endl;
            return;
        }
        for (const auto& bucket : table) {
            for (const auto& node : bucket) {
                outFile << node.data.serialize() << "\n";
            }
        }
        outFile.close();
    }
};

#endif
