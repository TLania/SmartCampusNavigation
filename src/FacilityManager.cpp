// FacilityManager.cpp
// Design: Implements loading and validation of facilities.txt.
#include "FacilityManager.h"
#include "Utils.h"
#include <fstream>
using namespace std;

// Reason for each outcome, used in rejection warnings
string GetFacilityResultMessage(FacilityResult result) {
    switch (result) {
        case FAC_RES_ACCEPTED:
            return "Accepted";
        case FAC_RES_BAD_FIELD_COUNT:
            return "Expected: FacilityID|FacilityName|BuildingName|Category";
        case FAC_RES_EMPTY_FIELD:
            return "Missing a required field";
        case FAC_RES_BAD_ID_FORMAT:
            return "Invalid facility ID";
        case FAC_RES_UNKNOWN_BUILDING:
            return "This building is not in buildings.txt";
        case FAC_RES_DUPLICATE_ID:
            return "Duplicate facility ID (the first record is kept)";
    }
    return "Unknown Error";
}

// An ID is valid when it is F followed by exactly three digits
// Caller passes the trimmed, uppercased ID
bool FacilityManager::isValidID(const string& id) const {
    if (id.size() != 4 || id.at(0) != 'F') {
        return false;
    }
    for (size_t i = 1; i < id.size(); i++) {
        if (id.at(i) < '0' || id.at(i) > '9') {
            return false;
        }
    }
    return true;
}

// Matches the name against the accepted buildings ignoring case
// Returns the buildings.txt spelling, or "" if none
string FacilityManager::findBuilding(const vector<string>& buildingNames, const string& name) const {
    string wanted = toUpperCase(name);
    for (size_t i = 0; i < buildingNames.size(); i++) {
        if (toUpperCase(buildingNames.at(i)) == wanted) {
            return buildingNames.at(i);
        }
    }
    return "";
}

// Returns the stored spelling of a category
// Or "" when no record has used it yet
string FacilityManager::findCategory(const string& name) const {
    string wanted = toUpperCase(name);
    for (size_t i = 0; i < categories.size(); i++) {
        if (toUpperCase(categories.at(i)) == wanted) {
            return categories.at(i);
        }
    }
    return "";
}

// Reads facilities.txt line by line.
// Order: field count, empty fields, ID format, unknown building, duplicate ID
//The first problem found is the reason printed for the rejected line. The duplicate check is the hash table insertion, which only succeeds for an ID not seen before
int FacilityManager::LoadFromFile(const string& directory, const string& filename, const vector<string>& buildingNames) {
    string path = buildPath(directory, filename);
    int accepted = 0;
    int rejected = 0;

    ifstream file(path.c_str());
    if (!file.is_open()) {
        cout << "Error: cannot open " << path << " - treating it as empty." << endl;
        cout << filename << ": 0 accepted, 0 rejected (hash capacity " << idTable.GetCapacity()
             << ", entries " << idTable.GetEntries() << ", collisions " << idTable.GetCollisions() << ")" << endl;
        return 0;
    }

    string line;
    int lineNumber = 0;
    while (getline(file, line)) {
        lineNumber++;
        if (isSkippableLine(line)) {
            continue;
        }

        vector<string> batch;
        FacilityResult result = FAC_RES_ACCEPTED;
        string id = "";
        string name = "";
        string building = "";
        string buildinginput = "";
        string category = "";

        if (splitBatch(line, batch) != 4) {
            result = FAC_RES_BAD_FIELD_COUNT;
        } else {
            id = toUpperCase(trim(batch.at(0)));
            name = trim(batch.at(1));
            building = trim(batch.at(2));
            category = trim(batch.at(3));

            if (id.empty() || name.empty() || building.empty() || category.empty()) {
                result = FAC_RES_EMPTY_FIELD;
            } else if (!isValidID(id)) {
                result = FAC_RES_BAD_ID_FORMAT;
            } else {
                buildinginput = building;
                building = findBuilding(buildingNames, building);
                if (building.empty()) {
                    result = FAC_RES_UNKNOWN_BUILDING;
                } else if (!idTable.Insert(id, (int)facilities.size())) {
                    result = FAC_RES_DUPLICATE_ID;
                }
            }
        }

        if (result != FAC_RES_ACCEPTED) {
            cout << "Warning: " << filename << " line " << lineNumber << ": "
                 << GetFacilityResultMessage(result);
            if (result == FAC_RES_UNKNOWN_BUILDING) {
                cout << ": " << buildinginput;
            }
            cout << endl;
            rejected++;
            continue;
        }

        // Record accepted, Category spelling now recorded
        string knownCategory = findCategory(category);
        if (knownCategory.empty()) {
            categories.push_back(category);
        } else {
            category = knownCategory;
        }

        facilities.push_back(Facility(id, name, building, category));
        accepted++;
    }
    file.close();

    cout << filename << ": " << accepted << " accepted, " << rejected << " rejected (hash capacity "
         << idTable.GetCapacity() << ", entries " << idTable.GetEntries() << ", collisions "
         << idTable.GetCollisions() << ")" << endl;
    return accepted;
}

// Number of accepted facilities
int FacilityManager::GetSize() const {
    return (int)facilities.size();
}

// Facility at a position in file order
Facility FacilityManager::GetFacilityAt(int index) const {
    return facilities.at(index);
}

// Hash table lookup. returns the list position of the ID, or -1 if not found
int FacilityManager::FindByID(const string& id) const {
    int index = -1;
    if (idTable.Find(id, index)) {
        return index;
    }
    return -1;
}

// Prints every facility in file order, then the total
void FacilityManager::DisplayAll() const {
    if (facilities.empty()) {
        cout << "No facilities are loaded." << endl;
        return;
    }

    cout << "Facilities (ID | Name | Building | Category):" << endl;
    for (size_t i = 0; i < facilities.size(); i++) {
        facilities.at(i).Display();
    }
    cout << "Total facilities: " << facilities.size() << endl;
}

// ID is trimmed and uppercased, then checked for  format, then looked up through the hash table
void FacilityManager::SearchByID(const string& id) const {
    string normalized = toUpperCase(trim(id));

    if (!isValidID(normalized)) {
        cout << "Invalid format: an ID is F followed by exactly three digits" << endl;
        return;
    }

    int index = FindByID(normalized);
    if (index < 0) {
        cout << "Not found: no facility with ID " << normalized << endl;
        return;
    }

    Facility found = facilities.at(index);
    cout << "Found: " << found.GetName() << " | " << found.GetBuilding() << " | "
         << found.GetCategory() << endl;
}
