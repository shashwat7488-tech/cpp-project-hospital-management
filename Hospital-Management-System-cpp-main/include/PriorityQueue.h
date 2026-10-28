#pragma once
#include "Patient.h"
#include <vector>
#include <unordered_map>
#include <memory>
#include <stack>
#include <queue>
#include <string>

using namespace std;

/**
 * @class PriorityQueue
 * @brief Manages patient appointments using a Max-Heap and handles admitted patients via Hash Map.
 */
class PriorityQueue {
private:
    vector<shared_ptr<Patient>> heap;
    unordered_map<int, shared_ptr<Patient>> admittedPatientsMap; 
    stack<shared_ptr<Patient>> oldPatientsStack;
    queue<string> complaintsQueue;
    
    void heapifyUp(int index);
    void heapifyDown(int index);
    int parent(int i) const { return (i - 1) / 2; }
    int leftChild(int i) const { return 2 * i + 1; }
    int rightChild(int i) const { return 2 * i + 2; }

    shared_ptr<Patient> parsePatient(const string& line) const;

public:
    PriorityQueue();
    ~PriorityQueue();

    void bookAppointment();
    void cancelAppointment(int patientID);
    void searchAppointment(int patientID) const;
    void displayAllAppointments() const;
    shared_ptr<Patient> checkUpAppointment();

    void admitPatient(shared_ptr<Patient> patient);
    void displayAdmittedPatients() const;
    void searchAdmittedPatient(int patientID) const;
    void modifyAdmittedPatient(int patientID);

    void displayOldPatients() const;
    void searchOldPatient(int patientID) const;

    void registerComplaint(const string& complaint);
    void displayComplaints() const;

    void loadState();
    void saveState() const;
};
