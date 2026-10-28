#include "BillingService.h"
#include "UIComponents.h"
#include <iostream>
#include <iomanip>
#include <algorithm>
#include <sstream>

using namespace std;

double BillingService::calculateBill(const string& medicine, const string& tests, int daysAdmitted) {
    double total = daysAdmitted * 150.00; // Base room rate
    if (!medicine.empty() && medicine != "--") total += (count(medicine.begin(), medicine.end(), ',') + 1) * 25.50;
    if (!tests.empty() && tests != "--") total += (count(tests.begin(), tests.end(), ',') + 1) * 120.00;
    return total;
}

void BillingService::printInvoice(int patientID, const string& name, double totalAmount) {
    UI::showHeader("OFFICIAL INVOICE");
    UI::showData("Patient ID", to_string(patientID));
    UI::showData("Patient Name", name);
    UI::printLine(30);
    ostringstream formattedTotal;
    formattedTotal << fixed << setprecision(2) << totalAmount;
    UI::showData("TOTAL DUE", "$" + formattedTotal.str());
    UI::printLine(30);
    UI::showStatus("success", "Payment processed via HMS Secure Pay.");
}
