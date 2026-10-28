#pragma once
#include <string>
#include <vector>

using namespace std;

/**
 * @class BillingService
 * @brief Handles the financial aspect of the hospital system.
 */
class BillingService {
public:
    /** @brief O(N) Time where N is string length | O(1) Space */
    static double calculateBill(const string& medicine, const string& tests, int daysAdmitted = 1);
    
    /** @brief O(1) Time | O(1) Space */
    static void printInvoice(int patientID, const string& name, double totalAmount);
};
