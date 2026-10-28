#pragma once
#include <string>

using namespace std;

/**
 * @struct Patient
 * @brief Unified structure to hold all patient data (Appointments, Admitted, Emergency).
 */
struct Patient {
    int id = 0;
    string name;
    string contact;
    int age = 0;
    
    // Appointment details
    int priority = 0;
    string doctorName;
    string medicine;
    string tests;
    string diagnosis;
    string notesForPatient;
    
    // Status flags
    bool admitted = false;
    double totalBill = 0.0;
    
    // Emergency details
    string entryCondition;
    string leavingCondition;
    bool hasLeftEmergency = false;
};
