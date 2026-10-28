#include "EmergencyManagement.h"
#include "Utility.h"
#include "Logger.h"
#include "BillingService.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <filesystem>
#include <vector>

using namespace std;

EmergencyManagement::EmergencyManagement() {
    loadState();
}

EmergencyManagement::~EmergencyManagement() {
    saveState();
}

void EmergencyManagement::registerEmergencyPatient(int patientID) {
    if (patientID <= 0) {
        cout << "Patient ID must be a positive number.\n";
        return;
    }
    auto activeStack = currentEmergencyStack;
    while (!activeStack.empty()) {
        if (activeStack.top()->id == patientID) {
            cout << "Patient with ID " << patientID << " is already in emergency care.\n";
            return;
        }
        activeStack.pop();
    }
    auto tempStack = oldEmergencyStack;
    while (!tempStack.empty()) {
        const auto& p = tempStack.top();
        if (p->id == patientID) {
            cout << "Patient with ID " << patientID << " is already in the old records.\n";
            return;
        }
        tempStack.pop();
    }

    auto patient = make_shared<Patient>();
    patient->id = patientID;
    patient->name = Utility::getRequiredLine("Enter patient name: ");
    patient->contact = Utility::getRequiredLine("Enter patient contact: ");
    do {
        patient->age = Utility::getSafeInt("Enter patient age: ");
        if (patient->age <= 0) cout << "Age must be a positive number.\n";
    } while (patient->age <= 0);
    patient->entryCondition = Utility::getRequiredLine("Enter entry condition: ");

    currentEmergencyStack.push(patient);
    Logger::getInstance().log("Registered Emergency Patient ID: " + to_string(patientID));
    cout << "Emergency Patient registered successfully.\n";
}

void EmergencyManagement::displayEmergencyPatients() const {
    if (currentEmergencyStack.empty()) { cout << "No current emergency patients.\n"; return; }
    auto tempStack = currentEmergencyStack;
    cout << "--- Current Emergency Patients ---\n";
    while (!tempStack.empty()) {
        const auto& p = tempStack.top();
        cout << "ID: " << p->id << " | Name: " << p->name << " | Condition: " << p->entryCondition << "\n";
        tempStack.pop();
    }
}

void EmergencyManagement::searchEmergencyPatient(int patientID) const {
    auto tempStack = currentEmergencyStack;
    while (!tempStack.empty()) {
        const auto& p = tempStack.top();
        if (p->id == patientID) { cout << "Found in Current Emergency: " << p->name << "\n"; return; }
        tempStack.pop();
    }
    tempStack = oldEmergencyStack;
    while (!tempStack.empty()) {
        const auto& p = tempStack.top();
        if (p->id == patientID) { cout << "Found in Old Emergency: " << p->name << "\n"; return; }
        tempStack.pop();
    }
    cout << "Patient ID " << patientID << " not found in emergency records.\n";
}

void EmergencyManagement::leaveEmergency(int patientID) {
    stack<shared_ptr<Patient>> tempStack;
    shared_ptr<Patient> target = nullptr;

    while (!currentEmergencyStack.empty()) {
        auto p = currentEmergencyStack.top();
        currentEmergencyStack.pop();
        if (p->id == patientID) { target = p; break; }
        tempStack.push(p);
    }

    if (!target) {
        cout << "Patient ID " << patientID << " not found in current emergency.\n";
        while (!tempStack.empty()) { currentEmergencyStack.push(tempStack.top()); tempStack.pop(); }
        return;
    }

    target->leavingCondition = Utility::getRequiredLine("Enter leaving health condition: ");
    
    double bill = BillingService::calculateBill(target->leavingCondition, "Emergency Care", 1);
    target->totalBill = bill;
    BillingService::printInvoice(target->id, target->name, bill);

    target->hasLeftEmergency = true;
    oldEmergencyStack.push(target);

    while (!tempStack.empty()) { currentEmergencyStack.push(tempStack.top()); tempStack.pop(); }
    Logger::getInstance().log("Emergency Patient ID: " + to_string(patientID) + " discharged.");
    cout << "Patient has successfully left the emergency department.\n";
}

void EmergencyManagement::displayOldEmergencyPatients() const {
    if (oldEmergencyStack.empty()) { cout << "No old emergency patients.\n"; return; }
    auto tempStack = oldEmergencyStack;
    cout << "--- Old Emergency Patients ---\n";
    while (!tempStack.empty()) {
        const auto& p = tempStack.top();
        cout << "ID: " << p->id << " | Name: " << p->name << " | Left Cond: " << p->leavingCondition << "\n";
        tempStack.pop();
    }
}

void EmergencyManagement::saveState() const {
    filesystem::create_directories("data");
    const auto saveStack = [](auto records, const string& path) {
        ofstream file(path);
        while (file.is_open() && !records.empty()) {
            const auto& p = records.top();
            file << p->id << "," << Utility::sanitize(p->name) << "," << p->age << ","
                 << Utility::sanitize(p->contact) << "," << Utility::sanitize(p->entryCondition) << ","
                 << Utility::sanitize(p->leavingCondition) << "\n";
            records.pop();
        }
    };
    saveStack(currentEmergencyStack, "data/emergency_current.csv");
    saveStack(oldEmergencyStack, "data/emergency_old.csv");
}

void EmergencyManagement::loadState() {
    const auto loadStack = [](const string& path, stack<shared_ptr<Patient>>& records, bool discharged) {
        ifstream file(path);
        string line;
        vector<shared_ptr<Patient>> loadedRecords;
        while (getline(file, line)) {
            stringstream ss(line);
            auto p = make_shared<Patient>();
            vector<string> fields;
            string field;
            while (getline(ss, field, ',')) fields.push_back(field);
            if (fields.size() < 5) continue;
            p->id = Utility::safeStoi(fields[0]);
            p->name = fields[1];
            p->age = Utility::safeStoi(fields[2]);
            if (fields.size() == 5 && discharged) { // Legacy discharged-record format.
                p->entryCondition = fields[3];
                p->leavingCondition = fields[4];
            } else {
                if (fields.size() < 5) continue;
                p->contact = fields[3];
                p->entryCondition = fields[4];
                if (fields.size() > 5) p->leavingCondition = fields[5];
            }
            if (p->id <= 0 || Utility::trim(p->name).empty()) continue;
            p->hasLeftEmergency = discharged;
            loadedRecords.push_back(p);
        }
        // Files are written from stack top to bottom. Push in reverse to restore that order.
        for (auto it = loadedRecords.rbegin(); it != loadedRecords.rend(); ++it) records.push(*it);
    };
    loadStack("data/emergency_current.csv", currentEmergencyStack, false);
    loadStack("data/emergency_old.csv", oldEmergencyStack, true);
}
