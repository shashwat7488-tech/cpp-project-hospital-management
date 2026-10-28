#pragma once
#include "Patient.h"
#include <memory>
#include <iostream>

using namespace std;

/**
 * @struct HistoryNode
 * @brief Node for a Doubly Linked List.
 */
struct HistoryNode {
    shared_ptr<Patient> data;
    shared_ptr<HistoryNode> next;
    shared_ptr<HistoryNode> prev;

    HistoryNode(shared_ptr<Patient> p) : data(p), next(nullptr), prev(nullptr) {}
};

/**
 * @class PatientHistory
 * @brief Demonstrates a Doubly Linked List for navigating recent patient records.
 */
class PatientHistory {
private:
    shared_ptr<HistoryNode> head;
    shared_ptr<HistoryNode> tail;
    shared_ptr<HistoryNode> current; // For navigation

public:
    PatientHistory() : head(nullptr), tail(nullptr), current(nullptr) {}

    /** @brief O(1) Time | O(1) Space */
    void addRecord(shared_ptr<Patient> p) {
        auto newNode = make_shared<HistoryNode>(p);
        if (!head) {
            head = tail = current = newNode;
        } else {
            tail->next = newNode;
            newNode->prev = tail;
            tail = newNode;
            current = tail; // Point to the most recent
        }
    }

    /** @brief O(1) Time - Navigate Backward */
    void undoView() {
        if (current && current->prev) {
            current = current->prev;
            cout << "Navigated Back to: " << current->data->name << endl;
        } else cout << "No previous records.\n";
    }

    /** @brief O(1) Time - Navigate Forward */
    void redoView() {
        if (current && current->next) {
            current = current->next;
            cout << "Navigated Forward to: " << current->data->name << endl;
        } else cout << "Already at most recent record.\n";
    }

    void displayCurrent() {
        if (current) cout << "Currently Viewing: " << current->data->name << " (ID: " << current->data->id << ")\n";
    }
};
