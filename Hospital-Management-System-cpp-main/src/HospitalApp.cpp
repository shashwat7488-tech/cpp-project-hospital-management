#include "HospitalApp.h"
#include "Utility.h"
#include "Logger.h"
#include "UIComponents.h"
#include "BillingService.h"
#include <iostream>
#include <fstream>
#include <filesystem>

using namespace std;
namespace fs = filesystem;

HospitalApp::HospitalApp() : currentPassword("admin123"), loggedIn(false) {
    loadPassword();
    populateSearchEngine();
    shifts.addDoctor("Dr. Smith");
    shifts.addDoctor("Dr. Strange");
    shifts.addDoctor("Dr. Watson");
    if (fs::exists("data/hospital_records.csv")) directory.loadRecords("data/hospital_records.csv");
}

void HospitalApp::populateSearchEngine() {
    searchEngine.insert("Cardiology"); searchEngine.insert("Neurology");
    searchEngine.insert("Orthopedics"); searchEngine.insert("Pediatrics");
    searchEngine.insert("Dr. Smith"); searchEngine.insert("Dr. Strange");
}

void HospitalApp::loadPassword() {
    if (ifstream passFile("data/password.txt"); passFile.is_open()) {
        getline(passFile, currentPassword);
        passFile.close();
    }
}

void HospitalApp::savePassword() {
    if (!fs::exists("data")) fs::create_directory("data");
    if (ofstream passFile("data/password.txt"); passFile.is_open()) {
        passFile << currentPassword;
        passFile.close();
    }
}

void HospitalApp::changePassword() {
    Utility::clearScreen();
    UI::showLogo();
    UI::showHeader("ACCOUNT SECURITY");
    string oldPass, newPass;
    cout << "  Current Password: ";
    getline(cin, oldPass);
    if (oldPass == currentPassword) {
        newPass = Utility::getRequiredLine("  New Password    : ");
        currentPassword = newPass;
        savePassword();
        UI::showStatus("success", "Password updated successfully.");
    } else {
        UI::showStatus("error", "Incorrect credentials. Verification failed.");
    }
}

void HospitalApp::handleLogin() {
    while (!loggedIn) {
        Utility::clearScreen();
        UI::showLogo();
        UI::showHeader("SECURE GATEWAY");
        cout << "  [1] Access System\n  [0] Exit Application\n\n  Selection: ";
        if (int choice = Utility::getSafeInt(""); choice == 1) {
            cout << "  Enter Key: ";
            string pass; getline(cin, pass);
            if (pass == currentPassword) {
                loggedIn = true;
                Utility::showLoading("  Verifying Credentials", 3, 100);
            } else {
                UI::showStatus("error", "Access Denied. Invalid Key.");
                Utility::sleep(800);
            }
        } else if (choice == 0) exit(0);
    }
}

void HospitalApp::run() {
    handleLogin();
    bool running = true;
    while (running) {
        Utility::clearScreen();
        UI::showLogo();
        vector<string> mainItems = {
            "Medical Staff Directory", "Nursing & Wards", "Patient Care Portal", 
            "Emergency Unit", "Administrator Hub", "Hospital Navigation", 
            "Unified Search", "Viewing History", "Change Password", "Exit System"
        };
        UI::showMenu("CENTRAL DASHBOARD", mainItems);
        
        int choice = Utility::getSafeInt("");
        switch (choice) {
            case 1: doctorMenu(); break;
            case 2: nurseMenu(); break;
            case 3: patientMenu(); break;
            case 4: emergencyMenu(); break;
            case 5: adminMenu(); break;
            case 6: navigationMenu(); break;
            case 7: searchEngineMenu(); break;
            case 8: {
                Utility::clearScreen();
                UI::showLogo();
                UI::showHeader("SYSTEM HISTORY");
                cout << "  [1] Back  [2] Next\n\n  Navigate: ";
                const int historyChoice = Utility::getSafeInt("");
                if (historyChoice == 1) history.undoView();
                else if (historyChoice == 2) history.redoView();
                else UI::showStatus("error", "Please choose 1 or 2.");
                history.displayCurrent();
                cout << "\n  Press Enter to return..."; 
                string d; getline(cin, d);
                break;
            }
            case 9: changePassword(); break;
            case 0: running = false; break;
            default: UI::showStatus("error", "Unknown command."); Utility::sleep(500); break;
        }
    }
    UI::showStatus("info", "Synchronizing local data...");
    Utility::sleep(500);
}

void HospitalApp::doctorMenu() {
    bool inLoop = true;
    while (inLoop) {
        Utility::clearScreen();
        UI::showLogo();
        UI::showMenu("DOCTOR PORTAL", {"Appointments", "Next Check-up", "History", "Back to Dashboard"});
        int choice = Utility::getSafeInt("");
        if (choice == 1) { 
            Utility::clearScreen(); UI::showLogo(); UI::showHeader("APPOINTMENTS"); 
            priorityQueue.displayAllAppointments(); 
        }
        else if (choice == 2) { 
            Utility::clearScreen(); UI::showLogo(); UI::showHeader("CHECK-UP"); 
            if (auto patient = priorityQueue.checkUpAppointment()) history.addRecord(patient);
        }
        else if (choice == 3) { 
            Utility::clearScreen(); UI::showLogo(); UI::showHeader("PATIENT HISTORY"); 
            priorityQueue.displayOldPatients(); 
        }
        else if (choice == 0) { inLoop = false; continue; }
        else { UI::showStatus("error", "Unknown command."); Utility::sleep(500); continue; }
        
        if (choice != 0) {
            cout << endl;
            if (!Utility::getYesNo("  Perform another task?")) inLoop = false;
        }
    }
}

void HospitalApp::nurseMenu() {
    bool inLoop = true;
    while (inLoop) {
        Utility::clearScreen();
        UI::showLogo();
        UI::showMenu("NURSE STATION", {"View Admitted", "Update Patient Data", "Back to Dashboard"});
        int choice = Utility::getSafeInt("");
        if (choice == 1) { 
            Utility::clearScreen(); UI::showLogo(); UI::showHeader("ADMITTED PATIENTS"); 
            priorityQueue.displayAdmittedPatients(); 
        }
        else if (choice == 2) { 
            Utility::clearScreen(); UI::showLogo(); UI::showHeader("UPDATE DATA"); 
            priorityQueue.modifyAdmittedPatient(Utility::getSafeInt("  Patient ID: ")); 
        }
        else if (choice == 0) { inLoop = false; continue; }
        else { UI::showStatus("error", "Unknown command."); Utility::sleep(500); continue; }
        
        if (choice != 0) {
            cout << endl;
            if (!Utility::getYesNo("  Perform another task?")) inLoop = false;
        }
    }
}

void HospitalApp::patientMenu() {
    bool inLoop = true;
    while (inLoop) {
        Utility::clearScreen();
        UI::showLogo();
        UI::showMenu("PATIENT SERVICES", {"Schedule Appointment", "Cancel Request", "File Feedback", "Back to Dashboard"});
        int choice = Utility::getSafeInt("");
        if (choice == 1) { 
            Utility::clearScreen(); UI::showLogo(); UI::showHeader("SCHEDULING"); 
            priorityQueue.bookAppointment(); 
        }
        else if (choice == 2) { 
            Utility::clearScreen(); UI::showLogo(); UI::showHeader("CANCELLATION"); 
            priorityQueue.cancelAppointment(Utility::getSafeInt("  Patient ID: ")); 
        }
        else if (choice == 3) { 
            Utility::clearScreen(); UI::showLogo(); UI::showHeader("FEEDBACK");
            string c = Utility::getRequiredLine("  Describe your complaint: ");
            priorityQueue.registerComplaint(c);
            UI::showStatus("success", "Feedback submitted.");
        }
        else if (choice == 0) { inLoop = false; continue; }
        else { UI::showStatus("error", "Unknown command."); Utility::sleep(500); continue; }
        
        if (choice != 0) {
            cout << endl;
            if (!Utility::getYesNo("  Continue in Portal?")) inLoop = false;
        }
    }
}

void HospitalApp::emergencyMenu() {
    bool inLoop = true;
    while (inLoop) {
        Utility::clearScreen();
        UI::showLogo();
        UI::showMenu("ER RESPONSE", {"Register Critical Case", "Triage Stack", "Discharge Patient", "Back to Dashboard"});
        int choice = Utility::getSafeInt("");
        if (choice == 1) { 
            Utility::clearScreen(); UI::showLogo(); UI::showHeader("ER REGISTRATION"); 
            emergency.registerEmergencyPatient(Utility::getSafeInt("  ID: ")); 
        }
        else if (choice == 2) { 
            Utility::clearScreen(); UI::showLogo(); UI::showHeader("ER TRIAGE"); 
            emergency.displayEmergencyPatients(); 
        }
        else if (choice == 3) { 
            Utility::clearScreen(); UI::showLogo(); UI::showHeader("ER DISCHARGE"); 
            emergency.leaveEmergency(Utility::getSafeInt("  ID: ")); 
        }
        else if (choice == 0) { inLoop = false; continue; }
        else { UI::showStatus("error", "Unknown command."); Utility::sleep(500); continue; }
        
        if (choice != 0) {
            cout << endl;
            if (!Utility::getYesNo("  Continue in ER?")) inLoop = false;
        }
    }
}

void HospitalApp::adminMenu() {
    bool inLoop = true;
    while (inLoop) {
        Utility::clearScreen();
        UI::showLogo();
        UI::showMenu("ADMIN CONTROL", {"Add Professional", "Remove Staff", "Inventory of Staff", "Shift Rotation", "Security Settings", "Back to Dashboard"});
        int choice = Utility::getSafeInt("");
        if (choice == 1) { 
            Utility::clearScreen(); UI::showLogo(); UI::showHeader("RECRUITMENT"); 
            directory.insertRecord(directory.createDoctorFromInput()); 
        }
        else if (choice == 2) { 
            Utility::clearScreen(); UI::showLogo(); UI::showHeader("OFFBOARDING"); 
            directory.deleteRecord(Utility::getSafeInt("  Staff ID: ")); 
        }
        else if (choice == 3) { 
            Utility::clearScreen(); UI::showLogo(); UI::showHeader("STAFF DIRECTORY"); 
            directory.displayAllRecords(); 
        }
        else if (choice == 4) { 
            Utility::clearScreen(); UI::showLogo(); UI::showHeader("SHIFT MANAGEMENT"); 
            shifts.showOnCall(); shifts.nextShift(); 
        }
        else if (choice == 5) { changePassword(); }
        else if (choice == 0) { inLoop = false; continue; }
        else { UI::showStatus("error", "Unknown command."); Utility::sleep(500); continue; }
        
        if (choice != 0) {
            cout << endl;
            if (!Utility::getYesNo("  Stay in Admin Hub?")) inLoop = false;
        }
    }
}

void HospitalApp::navigationMenu() {
    Utility::clearScreen();
    UI::showLogo();
    UI::showHeader("HOSPITAL NAVIGATOR");
    navigator.displayLocations();
    cout << "\n  Origin      : "; string s; getline(cin, s);
    cout << "  Destination : "; string e; getline(cin, e);
    navigator.findShortestPath(s, e);
    cout << "\n  Press Enter to return..."; string dummy; getline(cin, dummy);
}

void HospitalApp::searchEngineMenu() {
    Utility::clearScreen();
    UI::showLogo();
    UI::showHeader("GLOBAL SEARCH");
    cout << "  Search Query: "; string p; getline(cin, p);
    auto results = searchEngine.searchPrefix(p);
    if(results.empty()) UI::showStatus("error", "No records match your query.");
    else {
        UI::showStatus("info", "Results found:");
        for(const auto& res : results) cout << "  > " << res << endl;
    }
    cout << "\n  Press Enter to return..."; string dummy; getline(cin, dummy);
}
