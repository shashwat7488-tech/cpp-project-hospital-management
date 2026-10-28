#include "HospitalApp.h"
#include <iostream>

using namespace std;

int main() {
    try {
        HospitalApp app;
        app.run();
    } catch (const exception& e) {
        cerr << "Error: " << e.what() << endl;
        return 1;
    }
    return 0;
}
