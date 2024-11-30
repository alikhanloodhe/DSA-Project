#include <iostream>
using namespace std;

// Node structure
struct QueueNode {
    int data;    // Data stored in the QueueNode
    QueueNode* next;  // Pointer to the next QueueNode

    // Constructor
    QueueNode(int value) : data(value), next(nullptr) {}
};

// Queue class using linked list
class Queue {
    // friend class User; user can access priv members as well
private:
    QueueNode* front; // Pointer to the front of the queue
    QueueNode* rear;  // Pointer to the rear of the queue
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
        QueueNode* newQueueNode = new QueueNode(value);

        if (isEmpty()) {
            front = rear = newQueueNode;
        } else {
            rear->next = newQueueNode;
            rear = newQueueNode;
        }
        size++;
        //cout << value << " enqueued to queue.\n";
    }


    QueueNode* getFront() const { // Getter Method for private member
    return front;
}
    // Dequeue: Remove an element from the front of the queue
    void dequeue() {
        if (isEmpty()) {
            cout << "Queue is empty. Cannot dequeue.\n";
            return;
        }

        QueueNode* temp = front;
        front = front->next;
        if (!front) { // If the queue is now empty
            rear = nullptr;
        }

       // cout << temp->data << " dequeued from queue.\n";
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

        QueueNode* current = front;
        cout << "Queue: ";
        while (current) {
            cout << current->data << " ";
            current = current->next;
        }
        cout << endl;
    }
};
