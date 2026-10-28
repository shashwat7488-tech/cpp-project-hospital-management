#pragma once

#include <iostream>
#include <string>
#include <limits>
#include <thread>
#include <chrono>
#include <algorithm>
#include <cctype>

using namespace std;

namespace Utility {

    /**
     * @brief Clears the console screen using ANSI escape sequences (Cross-platform).
     */
    inline void clearScreen() {
        cout << "\033[2J\033[1;1H";
    }

    /**
     * @brief Pauses the thread execution for a given number of milliseconds.
     */
    inline void sleep(int milliseconds) {
        this_thread::sleep_for(chrono::milliseconds(milliseconds));
    }

    /**
     * @brief Displays a loading animation.
     */
    inline void showLoading(const string& message = "Loading", int dots = 5, int delayMs = 300) {
        cout << "\n" << message;
        for (int i = 0; i < dots; ++i) {
            cout << ". ";
            cout.flush();
            sleep(delayMs);
        }
        cout << "\n";
    }

    /**
     * @brief Sanitizes a string for CSV storage by replacing commas with spaces.
     * @note This prevents CSV parsing errors.
     */
    inline string sanitize(string str) {
        replace(str.begin(), str.end(), ',', ' ');
        return str;
    }

    inline string trim(string str) {
        const auto first = find_if_not(str.begin(), str.end(), [](unsigned char c) { return isspace(c); });
        const auto last = find_if_not(str.rbegin(), str.rend(), [](unsigned char c) { return isspace(c); }).base();
        return first < last ? string(first, last) : string();
    }

    inline string getRequiredLine(const string& prompt) {
        string value;
        do {
            cout << prompt;
            getline(cin, value);
            value = trim(value);
            if (value.empty()) cout << "This field cannot be empty. Please try again.\n";
        } while (value.empty());
        return value;
    }

    /**
     * @brief Safely converts string to int, returning 0 on failure.
     */
    inline int safeStoi(const string& str) {
        try {
            return stoi(str);
        } catch (...) {
            return 0;
        }
    }

    /**
     * @brief Gets a validated integer input from the user.
     */
    inline int getSafeInt(const string& prompt) {
        int value;
        while (true) {
            cout << prompt;
            if (cin >> value) {
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                return value;
            } else {
                cout << "Invalid input. Please enter a valid integer.\n";
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
            }
        }
    }

    /**
     * @brief Prompts for a yes/no answer and validates it.
     */
    inline bool getYesNo(const string& prompt) {
        string input;
        while (true) {
            cout << prompt << " (yes/no): ";
            getline(cin, input);
            for (char& c : input) c = tolower(c);
            if (input == "yes" || input == "y") return true;
            if (input == "no" || input == "n") return false;
            cout << "Invalid input. Please enter 'yes' or 'no'.\n";
        }
    }
}
