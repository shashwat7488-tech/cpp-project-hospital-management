#pragma once
#include <string>
#include <memory>
#include <vector>
#include <optional>

using namespace std;

/**
 * @struct Doctor
 * @brief Represents a Doctor node in the AVL Tree.
 */
struct Doctor {
    string name;
    int id = 0;
    string speciality;
    bool isSurgeon = false;
    string cnic;
    string dob;
    string contactNumber;
    string homeAddress;
    string officeRoomNo;
    string buildingNo;

    int height = 1; 
    shared_ptr<Doctor> left = nullptr;
    shared_ptr<Doctor> right = nullptr;
};

/**
 * @class HospitalDirectory
 * @brief Manages doctor records using an AVL Tree.
 */
class HospitalDirectory {
private:
    shared_ptr<Doctor> root;

    int height(shared_ptr<Doctor> N) const;
    int getBalance(shared_ptr<Doctor> N) const;
    shared_ptr<Doctor> rightRotate(shared_ptr<Doctor> y);
    shared_ptr<Doctor> leftRotate(shared_ptr<Doctor> x);
    shared_ptr<Doctor> insertRecord(shared_ptr<Doctor> node, shared_ptr<Doctor> newDoctor);
    shared_ptr<Doctor> minValueNode(shared_ptr<Doctor> node);
    shared_ptr<Doctor> deleteNode(shared_ptr<Doctor> root, int id);
    
    void displayAllRecords(shared_ptr<Doctor> root) const;
    void inorderCollect(shared_ptr<Doctor> root, vector<shared_ptr<Doctor>>& list) const;
    shared_ptr<Doctor> searchNode(shared_ptr<Doctor> root, int id) const;
    void saveRecordsToFile(shared_ptr<Doctor> root, ofstream& file) const;
    shared_ptr<Doctor> parseRecord(const string& line);

public:
    HospitalDirectory();
    ~HospitalDirectory();

    shared_ptr<Doctor> createDoctorFromInput();

    /** @brief O(log N) Time | O(log N) Space */
    void insertRecord(shared_ptr<Doctor> newDoctor);

    /** @brief O(log N) Time | O(log N) Space */
    void deleteRecord(int id);

    /** @brief O(log N) Time | O(log N) Space */
    void updateRecord(int id);
    
    /** @brief O(log N) Time | O(1) Space */
    optional<shared_ptr<Doctor>> findDoctor(int id) const;
    
    void searchRecord(int id) const;

    /** @brief O(N) Time | O(N) Space (Recursion stack) */
    void displayAllRecords() const;

    /** @brief O(N log N) Time due to sorting | O(N) Space */
    void sortRecordsAlphabetical() const;
    
    void loadRecords(const string& fileName);
    void saveRecords(const string& fileName = "data/hospital_records.csv") const;
};
