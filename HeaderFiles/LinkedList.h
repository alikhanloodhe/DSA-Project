#ifndef LINKEDLIST_H
#define LINKEDLIST_H

#include <iostream>
#include <string>
using namespace std;

// Node Class
template <typename T> // Template is used as it can be of any datatype
class Node {
public:
    T data;
    Node* next;

    Node(const T& value) : data(value), next(nullptr) {}
};

// LinkedList Class
template <typename T>
class LinkedList {
private:
    Node<T>* head;

public:
    LinkedList() : head(nullptr) {}

    // Add an element to the linked list
    void add(const T& value) {
        Node<T>* newNode = new Node<T>(value);
        if (!head) {
            head = newNode;
        } else {
            Node<T>* temp = head;
            while (temp->next) {
                temp = temp->next;
            }
            temp->next = newNode;
        }
    }
    // Find a node by ID
    T* findByID(int id) {
        Node<T>* temp = head;
        while (temp) {
            if (temp->data.ID == id) {
                return &temp->data;
            }
            temp = temp->next;
        }
        return nullptr;
    }

    // Check if a username already exists
    bool usernameExists(const string& username) {
        return find(username) != nullptr;
    }

    // Save linked list data to a file
    void saveToFile(const string& filename) {
        ofstream outFile(filename);
        if (!outFile) {
            cerr << "Error opening file for writing: " << filename << endl;
            return;
        }
        Node<T>* temp = head;
        while (temp) {
            outFile << temp->data.serialize() << "\n";
            temp = temp->next;
        }
        outFile.close();
    }
    T* find(const string& username) {
        Node<T>* temp = head;
        while (temp) {
            if (temp->data.username == username) {
                return &temp->data;
            }
            temp = temp->next;
        }
        return nullptr;
    }

    // Load linked list data from a file
    void loadFromFile(const string& filename) {
        ifstream inFile(filename);
        if (!inFile) {
            cerr << "Error opening file for reading: " << filename << endl;
            return;
        }
        clear();
        string line;
        while (getline(inFile, line)) {
            T data;
            data.deserialize(line);
            add(data);
        }
        inFile.close();
    }

    // Clear the linked list
    void clear() {
        while (head) {
            Node<T>* temp = head;
            head = head->next;
            delete temp;
        }
    }

    // Destructor to clear memory
    ~LinkedList() {
        clear();
    }
};

#endif
