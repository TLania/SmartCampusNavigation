// FacilityManager.h
// Design:The system loads facilities from `facilities.txt`, validates each record, and maintains them in file order. 
//It references accepted building names from `buildings.txt` to ensure that each facility only refers to a valid building.
//Additionally, it stores every approved facility ID in a HashTable for quick access and to prevent duplicates.
#ifndef FACILITY_MANAGER_H
#define FACILITY_MANAGER_H

#include <iostream>
#include <string>
#include <vector>
#include "Facility.h"
#include "HashTable.h"
using namespace std;

// Names for the outcome of checking facilities.txt record
enum FacilityResult {
    FAC_RES_ACCEPTED,
    FAC_RES_BAD_FIELD_COUNT,
    FAC_RES_EMPTY_FIELD,
    FAC_RES_BAD_ID_FORMAT,
    FAC_RES_UNKNOWN_BUILDING,
    FAC_RES_DUPLICATE_ID
};

// Takes FacilityResult and turns into a readable reason for a warning
string GetFacilityResultMessage(FacilityResult result);

class FacilityManager {
private:
    vector<Facility> facilities; // Facilities In file order
    vector<string> categories;   // First accepted becomes set spelling
    HashTable idTable;           // facility ID->position in the facilities list

    // Private helper functions
    bool isValidID(const string& id) const;
    string findBuilding(const vector<string>& buildingNames, const string& name) const;
    string findCategory(const string& name) const;

public:
    // File input. Prints a warning for rejected record and summary line, then returns the number of accepted facilities
    int LoadFromFile(const string& directory, const string& filename, const vector<string>& buildingNames);

    // Accessors
    int GetSize() const;
    Facility GetFacilityAt(int index) const;

    // Looks the ID up in the hash table and returns the position in the list, or -1 when no facility has matching ID
    int FindByID(const string& id) const;

    // Display every facility in file order
    void DisplayAll() const;

    // Prints one of three outcomes - invalid format, valid but not found, or found.
    void SearchByID(const string& id) const;
};

#endif // FACILITY_MANAGER_H
