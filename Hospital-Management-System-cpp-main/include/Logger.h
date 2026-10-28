#pragma once
#include <string>
#include <fstream>
#include <mutex>
#include <filesystem>

using namespace std;

/**
 * @class Logger
 * @brief Thread-safe logging mechanism for the hospital system.
 */
class Logger {
public:
    static Logger& getInstance();
    void log(const string& message);

private:
    Logger();
    ~Logger();
    Logger(const Logger&) = delete;
    Logger& operator=(const Logger&) = delete;

    ofstream logFile;
    mutex mtx;
};
