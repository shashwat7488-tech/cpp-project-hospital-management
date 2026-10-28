#include "HospitalDirectory.h"
#include "Utility.h"
#include "Logger.h"
#include <iostream>
#include <algorithm>
#include <fstream>
#include <sstream>
#include <filesystem>

using namespace std;

HospitalDirectory::HospitalDirectory() : root(nullptr) {}

HospitalDirectory::~HospitalDirectory() {
    saveRecords();
}

int HospitalDirectory::height(shared_ptr<Doctor> N) const {
    return N ? N->height : 0;
}

int HospitalDirectory::getBalance(shared_ptr<Doctor> N) const {
    return N ? height(N->left) - height(N->right) : 0;
}

shared_ptr<Doctor> HospitalDirectory::rightRotate(shared_ptr<Doctor> y) {
    auto x = y->left;
    auto T2 = x->right;
    x->right = y;
    y->left = T2;
    y->height = max(height(y->left), height(y->right)) + 1;
    x->height = max(height(x->left), height(x->right)) + 1;
    return x;
}

shared_ptr<Doctor> HospitalDirectory::leftRotate(shared_ptr<Doctor> x) {
    auto y = x->right;
    auto T2 = y->left;
    y->left = x;
    x->right = T2;
    x->height = max(height(x->left), height(x->right)) + 1;
    y->height = max(height(y->left), height(y->right)) + 1;
    return y;
}

shared_ptr<Doctor> HospitalDirectory::insertRecord(shared_ptr<Doctor> node, shared_ptr<Doctor> newDoctor) {
    if (!node) return newDoctor;
    if (newDoctor->id < node->id)
        node->left = insertRecord(node->left, newDoctor);
    else if (newDoctor->id > node->id)
        node->right = insertRecord(node->right, newDoctor);
    else
        return node;

    node->height = 1 + max(height(node->left), height(node->right));
    int balance = getBalance(node);

    if (balance > 1 && newDoctor->id < node->left->id) return rightRotate(node);
    if (balance < -1 && newDoctor->id > node->right->id) return leftRotate(node);
    if (balance > 1 && newDoctor->id > node->left->id) {
        node->left = leftRotate(node->left);
        return rightRotate(node);
    }
    if (balance < -1 && newDoctor->id < node->right->id) {
        node->right = rightRotate(node->right);
        return leftRotate(node);
    }
    return node;
}

void HospitalDirectory::insertRecord(shared_ptr<Doctor> newDoctor) {
    if (!newDoctor || newDoctor->id <= 0 || Utility::trim(newDoctor->name).empty() || Utility::trim(newDoctor->speciality).empty()) {
        cout << "Doctor record is incomplete or has an invalid ID.\n";
        return;
    }
    if (findDoctor(newDoctor->id)) {
        cout << "A doctor with this ID already exists.\n";
        return;
    }
    root = insertRecord(root, newDoctor);
    Logger::getInstance().log("Inserted Doctor ID: " + to_string(newDoctor->id));
}

shared_ptr<Doctor> HospitalDirectory::minValueNode(shared_ptr<Doctor> node) {
    auto current = node;
    while (current->left) current = current->left;
    return current;
}

shared_ptr<Doctor> HospitalDirectory::deleteNode(shared_ptr<Doctor> rootNode, int id) {
    if (!rootNode) return rootNode;
    if (id < rootNode->id)
        rootNode->left = deleteNode(rootNode->left, id);
    else if (id > rootNode->id)
        rootNode->right = deleteNode(rootNode->right, id);
    else {
        if (!rootNode->left || !rootNode->right) {
            auto temp = rootNode->left ? rootNode->left : rootNode->right;
            if (!temp) {
                temp = rootNode;
                rootNode = nullptr;
            } else
                *rootNode = *temp;
        } else {
            auto temp = minValueNode(rootNode->right);
            rootNode->id = temp->id;
            rootNode->name = temp->name;
            rootNode->right = deleteNode(rootNode->right, temp->id);
        }
    }

    if (!rootNode) return rootNode;
    rootNode->height = 1 + max(height(rootNode->left), height(rootNode->right));
    int balance = getBalance(rootNode);
    if (balance > 1 && getBalance(rootNode->left) >= 0) return rightRotate(rootNode);
    if (balance > 1 && getBalance(rootNode->left) < 0) {
        rootNode->left = leftRotate(rootNode->left);
        return rightRotate(rootNode);
    }
    if (balance < -1 && getBalance(rootNode->right) <= 0) return leftRotate(rootNode);
    if (balance < -1 && getBalance(rootNode->right) > 0) {
        rootNode->right = rightRotate(rootNode->right);
        return leftRotate(rootNode);
    }
    return rootNode;
}

void HospitalDirectory::deleteRecord(int id) {
    root = deleteNode(root, id);
    Logger::getInstance().log("Deleted Doctor ID: " + to_string(id));
}

shared_ptr<Doctor> HospitalDirectory::searchNode(shared_ptr<Doctor> node, int id) const {
    if (!node || node->id == id) return node;
    return (id < node->id) ? searchNode(node->left, id) : searchNode(node->right, id);
}

optional<shared_ptr<Doctor>> HospitalDirectory::findDoctor(int id) const {
    if (auto node = searchNode(root, id); node) return node;
    return nullopt;
}

void HospitalDirectory::searchRecord(int id) const {
    if (auto res = findDoctor(id); res) {
        const auto& doc = *res;
        cout << "Doctor Found!\nName: " << doc->name << "\nID: " << doc->id << "\nSpeciality: " << doc->speciality << endl;
    } else {
        cout << "Doctor with ID " << id << " not found." << endl;
    }
}

shared_ptr<Doctor> HospitalDirectory::createDoctorFromInput() {
    auto newDoctor = make_shared<Doctor>();
    newDoctor->name = Utility::getRequiredLine("Enter Doctor Name: ");
    do {
        newDoctor->id = Utility::getSafeInt("Enter Doctor ID: ");
        if (newDoctor->id <= 0) cout << "Doctor ID must be a positive number.\n";
    } while (newDoctor->id <= 0);
    newDoctor->speciality = Utility::getRequiredLine("Enter Doctor Speciality: ");
    newDoctor->isSurgeon = Utility::getYesNo("Is the doctor a Surgeon?");
    newDoctor->contactNumber = Utility::getRequiredLine("Enter Contact Number: ");
    return newDoctor;
}

void HospitalDirectory::updateRecord(int id) {
    if (auto res = findDoctor(id); res) {
        auto& doc = *res;
        cout << "Updating Doctor: " << doc->name << endl;
        cout << "Enter New Speciality: ";
        getline(cin, doc->speciality);
        doc->isSurgeon = Utility::getYesNo("Is Surgeon?");
        Logger::getInstance().log("Updated Doctor ID: " + to_string(id));
        cout << "Doctor updated successfully.\n";
    } else {
        cout << "Doctor not found!" << endl;
    }
}

void HospitalDirectory::displayAllRecords(shared_ptr<Doctor> node) const {
    if (node) {
        displayAllRecords(node->left);
        cout << "ID: " << node->id << " | Name: " << node->name << " | Speciality: " << node->speciality << endl;
        displayAllRecords(node->right);
    }
}

void HospitalDirectory::displayAllRecords() const {
    cout << "--- All Doctor Records ---" << endl;
    displayAllRecords(root);
}

void HospitalDirectory::inorderCollect(shared_ptr<Doctor> node, vector<shared_ptr<Doctor>>& list) const {
    if (node) {
        inorderCollect(node->left, list);
        list.push_back(node);
        inorderCollect(node->right, list);
    }
}

void HospitalDirectory::sortRecordsAlphabetical() const {
    vector<shared_ptr<Doctor>> docs;
    inorderCollect(root, docs);
    sort(docs.begin(), docs.end(), [](const shared_ptr<Doctor>& a, const shared_ptr<Doctor>& b) {
        return a->name < b->name;
    });
    cout << "--- Doctors Alphabetically ---" << endl;
    for (const auto& doc : docs) cout << "Name: " << doc->name << " | ID: " << doc->id << endl;
}

void HospitalDirectory::saveRecordsToFile(shared_ptr<Doctor> node, ofstream& file) const {
    if (node) {
        saveRecordsToFile(node->left, file);
        file << node->id << "," << node->name << "," << node->speciality << "," << (node->isSurgeon ? "1" : "0") << "\n";
        saveRecordsToFile(node->right, file);
    }
}

void HospitalDirectory::saveRecords(const string& fileName) const {
    const auto parent = filesystem::path(fileName).parent_path();
    if (!parent.empty()) filesystem::create_directories(parent);
    ofstream file(fileName);
    if (file.is_open()) {
        saveRecordsToFile(root, file);
        file.close();
    }
}

shared_ptr<Doctor> HospitalDirectory::parseRecord(const string& line) {
    auto doc = make_shared<Doctor>();
    stringstream ss(line);
    string item;
    if (!getline(ss, item, ',')) return nullptr;
    doc->id = Utility::safeStoi(item);
    getline(ss, doc->name, ',');
    getline(ss, doc->speciality, ',');
    getline(ss, item, ','); doc->isSurgeon = (item == "1");
    if (doc->id <= 0 || Utility::trim(doc->name).empty() || Utility::trim(doc->speciality).empty()) return nullptr;
    return doc;
}

void HospitalDirectory::loadRecords(const string& fileName) {
    ifstream file(fileName);
    if (!file.is_open()) return;
    string line;
    while (getline(file, line)) {
        if (!line.empty()) {
            if (auto doc = parseRecord(line); doc) insertRecord(doc);
        }
    }
    file.close();
}
