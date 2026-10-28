#include "Logger.h"
#include <iostream>
#include <chrono>
#include <ctime>
#include <iomanip>
#include <sstream>

using namespace std;
namespace fs = filesystem;

Logger& Logger::getInstance() {
    static Logger instance;
    return instance;
}

Logger::Logger() {
    // C++17: Ensure directory exists
    if (!fs::exists("data")) {
        fs::create_directory("data");
    }
    
    logFile.open("data/hospital.log", ios::app);
    if (!logFile.is_open()) {
        cerr << "Warning: Could not open log file!" << endl;
    }
}

Logger::~Logger() {
    if (logFile.is_open()) {
        logFile.close();
    }
}

void Logger::log(const string& message) {
    lock_guard<mutex> lock(mtx);
    if (logFile.is_open()) {
        auto now = chrono::system_clock::now();
        time_t now_c = chrono::system_clock::to_time_t(now);
        
        struct tm timeinfo;
#if defined(_WIN32) || defined(_WIN64)
        localtime_s(&timeinfo, &now_c);
#else
        localtime_r(&now_c, &timeinfo);
#endif

        char timeBuffer[80];
        strftime(timeBuffer, sizeof(timeBuffer), "%Y-%m-%d %H:%M:%S", &timeinfo);
        
        logFile << "[" << timeBuffer << "] " << message << endl;
        logFile.flush();
    }
}
