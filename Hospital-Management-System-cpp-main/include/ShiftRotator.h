#pragma once
#include <string>
#include <vector>
#include <iostream>
#include <memory>

using namespace std;

/**
 * @struct ShiftNode
 * @brief Node for a Circular Linked List.
 */
struct ShiftNode {
    string doctorName;
    shared_ptr<ShiftNode> next;

    ShiftNode(string name) : doctorName(name), next(nullptr) {}
};

/**
 * @class ShiftRotator
 * @brief Demonstrates a Circular Linked List for managing Doctor shifts.
 */
class ShiftRotator {
private:
    shared_ptr<ShiftNode> currentShift;

public:
    ShiftRotator() : currentShift(nullptr) {}

    /** @brief O(N) Time to build | O(1) Space */
    void addDoctor(string name) {
        auto newNode = make_shared<ShiftNode>(name);
        if (!currentShift) {
            currentShift = newNode;
            currentShift->next = currentShift; // Points to itself (Circular)
        } else {
            auto temp = currentShift;
            while (temp->next != currentShift) temp = temp->next;
            temp->next = newNode;
            newNode->next = currentShift;
        }
    }

    /** @brief O(1) Time - Rotate to the next doctor in the circle */
    void nextShift() {
        if (currentShift) {
            currentShift = currentShift->next;
            cout << "New Doctor On-Call: " << currentShift->doctorName << endl;
        }
    }

    void showOnCall() {
        if (currentShift) cout << "Current Doctor On-Call: " << currentShift->doctorName << endl;
    }
};
