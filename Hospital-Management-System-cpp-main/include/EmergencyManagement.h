#pragma once
#include "Patient.h"
#include <stack>
#include <memory>

using namespace std;

/**
 * @class EmergencyManagement
 * @brief Handles emergency patients using Stacks and persists data to fix the "Leave Emergency" bug.
 */
class EmergencyManagement {
private:
    stack<shared_ptr<Patient>> currentEmergencyStack;
    stack<shared_ptr<Patient>> oldEmergencyStack;

public:
    EmergencyManagement();
    ~EmergencyManagement();

    void registerEmergencyPatient(int patientID);
    void displayEmergencyPatients() const;
    void searchEmergencyPatient(int patientID) const;
    void leaveEmergency(int patientID);
    void displayOldEmergencyPatients() const;

    void loadState();
    void saveState() const;
};
