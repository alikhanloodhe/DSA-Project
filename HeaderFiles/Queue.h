#include <iostream>
using namespace std;

// Node structure
struct Node {
    int data;    // Data stored in the node
    Node* next;  // Pointer to the next node

    // Constructor
    Node(int value) : data(value), next(nullptr) {}
};

// Queue class using linked list
class Queue {
private:
    Node* front; // Pointer to the front of the queue
    Node* rear;  // Pointer to the rear of the queue
    int size;    // Current size of the queue

public:
    // Constructor
    Queue() : front(nullptr), rear(nullptr), size(0) {}

    // Destructor to free memory
    ~Queue() {
        while (!isEmpty()) {
            dequeue();
        }
    }

    // Enqueue: Add an element to the rear of the queue
    void enqueue(int value) {
        Node* newNode = new Node(value);

        if (isEmpty()) {
            front = rear = newNode;
        } else {
            rear->next = newNode;
            rear = newNode;
        }
        size++;
        cout << value << " enqueued to queue.\n";
    }

    // Dequeue: Remove an element from the front of the queue
    void dequeue() {
        if (isEmpty()) {
            cout << "Queue is empty. Cannot dequeue.\n";
            return;
        }

        Node* temp = front;
        front = front->next;
        if (!front) { // If the queue is now empty
            rear = nullptr;
        }

        cout << temp->data << " dequeued from queue.\n";
        delete temp;
        size--;
    }

    // Peek: Get the front element without removing it
    int peek() const {
        if (isEmpty()) {
            cout << "Queue is empty. Cannot peek.\n";
            return -1; // Return an invalid value
        }
        return front->data;
    }

    // Check if the queue is empty
    bool isEmpty() const {
        return front == nullptr;
    }

    // Get the size of the queue
    int getSize() const {
        return size;
    }

    // Display the queue contents
    void display() const {
        if (isEmpty()) {
            cout << "Queue is empty.\n";
            return;
        }

        Node* current = front;
        cout << "Queue: ";
        while (current) {
            cout << current->data << " ";
            current = current->next;
        }
        cout << endl;
    }
};
