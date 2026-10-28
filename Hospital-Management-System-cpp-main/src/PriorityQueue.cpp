#include "PriorityQueue.h"
#include "Utility.h"
#include "Logger.h"
#include "BillingService.h"
#include <iostream>
#include <climits>
#include <algorithm>
#include <fstream>
#include <sstream>
#include <filesystem>

using namespace std;
namespace fs = filesystem;

PriorityQueue::PriorityQueue() {
    loadState();
}

PriorityQueue::~PriorityQueue() {
    saveState();
}

shared_ptr<Patient> PriorityQueue::parsePatient(const string& line) const {
    auto p = make_shared<Patient>();
    stringstream ss(line);
    string item;
    try {
        if (!getline(ss, item, ',')) return nullptr;
        p->id = Utility::safeStoi(item);
        getline(ss, p->name, ',');
        getline(ss, p->contact, ',');
        getline(ss, item, ','); p->age = Utility::safeStoi(item);
        getline(ss, item, ','); p->priority = Utility::safeStoi(item);
        getline(ss, p->doctorName, ',');
        getline(ss, p->medicine, ',');
        getline(ss, p->tests, ',');
        getline(ss, p->diagnosis, ',');
        getline(ss, p->notesForPatient, ',');
        getline(ss, item, ','); p->admitted = (item == "1");
        if (p->id <= 0 || Utility::trim(p->name).empty()) return nullptr;
    } catch (...) { return nullptr; }
    return p;
}

void PriorityQueue::saveState() const {
    if (!fs::exists("data")) fs::create_directory("data");
    ofstream appFile("data/appointments.csv");
    for (const auto& p : heap) {
        appFile << p->id << "," << Utility::sanitize(p->name) << "," << Utility::sanitize(p->contact) << "," 
                << p->age << "," << p->priority << "," << Utility::sanitize(p->doctorName) << "," 
                << Utility::sanitize(p->medicine) << "," << Utility::sanitize(p->tests) << "," 
                << Utility::sanitize(p->diagnosis) << "," << Utility::sanitize(p->notesForPatient) << "," 
                << (p->admitted ? "1" : "0") << "\n";
    }
    appFile.close();
    ofstream admFile("data/admitted.csv");
    for (const auto& [id, p] : admittedPatientsMap) {
        admFile << p->id << "," << Utility::sanitize(p->name) << "," << Utility::sanitize(p->contact) << "," 
                << p->age << "," << p->priority << "," << Utility::sanitize(p->doctorName) << "," 
                << Utility::sanitize(p->medicine) << "," << Utility::sanitize(p->tests) << "," 
                << Utility::sanitize(p->diagnosis) << "," << Utility::sanitize(p->notesForPatient) << "," 
                << (p->admitted ? "1" : "0") << "\n";
    }
    admFile.close();
    ofstream compFile("data/complaints.csv");
    auto tempQueue = complaintsQueue;
    while (!tempQueue.empty()) {
        compFile << Utility::sanitize(tempQueue.front()) << "\n";
        tempQueue.pop();
    }
    compFile.close();
}

void PriorityQueue::loadState() {
    if (ifstream appFile("data/appointments.csv"); appFile.is_open()) {
        string line;
        while (getline(appFile, line)) if (auto p = parsePatient(line); p) heap.push_back(p);
        for (int i = static_cast<int>(heap.size()) / 2 - 1; i >= 0; i--) heapifyDown(i);
        appFile.close();
    }
    if (ifstream admFile("data/admitted.csv"); admFile.is_open()) {
        string line;
        while (getline(admFile, line)) if (auto p = parsePatient(line); p) admittedPatientsMap[p->id] = p;
        admFile.close();
    }
    if (ifstream compFile("data/complaints.csv"); compFile.is_open()) {
        string line;
        while (getline(compFile, line)) if (!line.empty()) complaintsQueue.push(line);
        compFile.close();
    }
}

void PriorityQueue::heapifyUp(int index) {
    if (index && heap[static_cast<size_t>(parent(index))]->priority < heap[static_cast<size_t>(index)]->priority) {
        swap(heap[static_cast<size_t>(index)], heap[static_cast<size_t>(parent(index))]);
        heapifyUp(parent(index));
    }
}

void PriorityQueue::heapifyDown(int index) {
    int left = leftChild(index), right = rightChild(index), largest = index;
    if (left < static_cast<int>(heap.size()) && heap[static_cast<size_t>(left)]->priority > heap[static_cast<size_t>(largest)]->priority) largest = left;
    if (right < static_cast<int>(heap.size()) && heap[static_cast<size_t>(right)]->priority > heap[static_cast<size_t>(largest)]->priority) largest = right;
    if (largest != index) { 
        swap(heap[static_cast<size_t>(index)], heap[static_cast<size_t>(largest)]); 
        heapifyDown(largest); 
    }
}

void PriorityQueue::bookAppointment() {
    auto patient = make_shared<Patient>();
    do {
        patient->id = Utility::getSafeInt("Enter Patient ID: ");
        if (patient->id <= 0) cout << "Patient ID must be a positive number.\n";
    } while (patient->id <= 0);
    if (any_of(heap.begin(), heap.end(), [&](const shared_ptr<Patient>& p) { return p->id == patient->id; }) ||
        admittedPatientsMap.find(patient->id) != admittedPatientsMap.end()) {
        cout << "A patient with this ID already has an active record.\n";
        return;
    }
    patient->name = Utility::getRequiredLine("Enter Name: ");
    patient->contact = Utility::getRequiredLine("Enter Contact: ");
    patient->doctorName = Utility::getRequiredLine("Enter Doctor: ");
    patient->priority = Utility::getYesNo("High Priority?") ? INT_MAX : 0;
    heap.push_back(patient);
    heapifyUp(static_cast<int>(heap.size()) - 1);
}

void PriorityQueue::cancelAppointment(int patientID) {
    auto it = find_if(heap.begin(), heap.end(), [patientID](const shared_ptr<Patient>& p) { return p->id == patientID; });
    if (it != heap.end()) {
        int index = static_cast<int>(distance(heap.begin(), it));
        heap[static_cast<size_t>(index)] = heap.back();
        heap.pop_back();
        if (index < static_cast<int>(heap.size())) { heapifyDown(index); heapifyUp(index); }
        cout << "Canceled successfully.\n";
    } else cout << "ID not found.\n";
}

void PriorityQueue::searchAppointment(int patientID) const {
    auto it = find_if(heap.begin(), heap.end(), [patientID](const shared_ptr<Patient>& p) { return p->id == patientID; });
    if (it != heap.end()) cout << "Found: " << (*it)->name << " with " << (*it)->doctorName << "\n";
    else cout << "Not found.\n";
}

void PriorityQueue::displayAllAppointments() const {
    if (heap.empty()) { cout << "No appointments waiting.\n"; return; }
    for (const auto& p : heap) cout << "ID: " << p->id << " | Name: " << p->name << " | Priority: " << (p->priority > 0 ? "High" : "Normal") << "\n";
}

shared_ptr<Patient> PriorityQueue::checkUpAppointment() {
    if (heap.empty()) { cout << "No patients waiting.\n"; return nullptr; }
    auto p = heap.front();
    cout << "--- Check-up: " << p->name << " ---\n";
    p->diagnosis = Utility::getRequiredLine("Diagnosis: ");
    p->medicine = Utility::getRequiredLine("Medicine: ");
    double bill = BillingService::calculateBill(p->medicine, "Checkup/Consultation", 1);
    BillingService::printInvoice(p->id, p->name, bill);
    if (p->admitted = Utility::getYesNo("Admit Patient for further treatment?"); p->admitted) admitPatient(p);
    else oldPatientsStack.push(p);
    heap[0] = heap.back(); heap.pop_back();
    if (!heap.empty()) heapifyDown(0);
    return p;
}

void PriorityQueue::admitPatient(shared_ptr<Patient> patient) {
    admittedPatientsMap[patient->id] = patient;
}

void PriorityQueue::displayAdmittedPatients() const {
    if (admittedPatientsMap.empty()) { cout << "None admitted.\n"; return; }
    for (const auto& [id, p] : admittedPatientsMap) cout << "ID: " << id << " | Name: " << p->name << "\n";
}

void PriorityQueue::searchAdmittedPatient(int patientID) const {
    if (auto it = admittedPatientsMap.find(patientID); it != admittedPatientsMap.end())
        cout << "Found Admitted: " << it->second->name << " | Diagnosis: " << it->second->diagnosis << "\n";
    else cout << "Not found.\n";
}

void PriorityQueue::modifyAdmittedPatient(int patientID) {
    if (auto it = admittedPatientsMap.find(patientID); it != admittedPatientsMap.end()) {
        cout << "Updating " << it->second->name << "\n";
        it->second->notesForPatient = Utility::getRequiredLine("Notes: ");
    } else cout << "Not found.\n";
}

void PriorityQueue::displayOldPatients() const {
    if (oldPatientsStack.empty()) { cout << "No history.\n"; return; }
    auto temp = oldPatientsStack;
    while (!temp.empty()) { 
        const auto& p = temp.top();
        cout << "ID: " << p->id << " | Name: " << p->name << "\n"; temp.pop(); 
    }
}

void PriorityQueue::searchOldPatient(int patientID) const {
    auto temp = oldPatientsStack;
    while (!temp.empty()) {
        const auto& p = temp.top();
        if (p->id == patientID) { cout << "Found: " << p->name << "\n"; return; }
        temp.pop();
    }
    cout << "Not found.\n";
}

void PriorityQueue::registerComplaint(const string& complaint) {
    complaintsQueue.push(complaint);
}

void PriorityQueue::displayComplaints() const {
    if (complaintsQueue.empty()) { cout << "No complaints.\n"; return; }
    auto temp = complaintsQueue;
    while (!temp.empty()) { cout << "- " << temp.front() << "\n"; temp.pop(); }
}
