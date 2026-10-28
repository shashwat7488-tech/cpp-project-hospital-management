#pragma once
#include "HospitalDirectory.h"
#include "PriorityQueue.h"
#include "EmergencyManagement.h"
#include "HospitalGraph.h"
#include "Trie.h"
#include "PatientHistory.h"
#include "ShiftRotator.h"
#include <string>

using namespace std;

/**
 * @class HospitalApp
 * @brief Controller class that runs the main application loop and sub-menus.
 */
class HospitalApp {
private:
    HospitalDirectory directory;
    PriorityQueue priorityQueue;
    EmergencyManagement emergency;
    HospitalGraph navigator;
    Trie searchEngine;
    PatientHistory history;
    ShiftRotator shifts;

    string currentPassword;
    bool loggedIn;

    void loadPassword();
    void savePassword();
    void changePassword();
    void populateSearchEngine();
    void handleLogin();
    void doctorMenu();
    void nurseMenu();
    void patientMenu();
    void emergencyMenu();
    void adminMenu();
    void navigationMenu();
    void searchEngineMenu();

public:
    HospitalApp();
    void run();
};
