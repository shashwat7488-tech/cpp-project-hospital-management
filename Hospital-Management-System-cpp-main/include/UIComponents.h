#pragma once
#include <string>
#include <iostream>
#include <vector>

using namespace std;

namespace UI {
    const string RESET   = "\033[0m";
    const string BOLD    = "\033[1m";
    const string BLUE    = "\033[94m";
    const string CYAN    = "\033[96m";
    const string GREEN   = "\033[92m";
    const string RED     = "\033[91m";
    const string GRAY    = "\033[90m";

    inline void printLine(int width) {
        cout << GRAY << "  " << string(static_cast<size_t>(width), '-') << RESET << endl;
    }

    inline void showLogo() {
        cout << BLUE << BOLD << endl;
        cout << "   _   _  __  __  ____  " << endl;
        cout << "  | | | ||  \\/  |/ ___| " << endl;
        cout << "  | |_| || |\\/| |\\___ \\ " << endl;
        cout << "  |  _  || |  | | ___) |" << endl;
        cout << "  |_| |_||_|  |_||____/ " << endl;
        cout << "  Hospital Management System" << RESET << endl;
    }

    inline void showHeader(const string& title) {
        cout << endl;
        cout << BLUE << BOLD << "  " << title << RESET << endl;
        printLine(static_cast<int>(title.length()) + 2);
    }

    inline void showMenu(const string& title, const vector<string>& options) {
        showHeader(title);
        for (size_t i = 0; i < options.size(); ++i) {
            bool isExit = (i == options.size() - 1);
            cout << "   " << (isExit ? RED : BLUE) << "[" << (isExit ? 0 : i + 1) << "]" << RESET 
                 << "  " << options[i] << endl;
        }
        cout << endl << GRAY << "  Selection: " << RESET;
    }

    inline void showStatus(const string& type, const string& msg) {
        if (type == "success") cout << "  " << GREEN << "[OK] " << RESET << msg << endl;
        else if (type == "error") cout << "  " << RED << "[!!] " << RESET << msg << endl;
        else cout << "  " << CYAN << "[..] " << RESET << msg << endl;
    }

    inline void showData(const string& label, const string& val) {
        cout << "  " << GRAY << label << " : " << RESET << BOLD << val << RESET << endl;
    }
}
