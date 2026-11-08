// Facility.h
// Design: Campus facility record. Takes ID, name, building, category. The ID is stored in uppercase form, EX: F004.
#ifndef FACILITY_H
#define FACILITY_H

#include <iostream>
#include <string>
using namespace std;

class Facility {
private:
    string facilityID;   // F plus exactly three digits, uppercase
    string facilityName; // With unique ID, name can repeat
    string building;     // refers to buildings.txt
    string category;     // First accepted becomes set spelling

public:
    // Constructors
    Facility();
    Facility(const string& id, const string& name, const string& buildingName, const string& categoryName);

    // Accessors (Getters)
    string GetID() const;
    string GetName() const;
    string GetBuilding() const;
    string GetCategory() const;

    // Mutators (Setters)
    void SetID(const string& id);
    void SetName(const string& name);
    void SetBuilding(const string& buildingName);
    void SetCategory(const string& categoryName);

    // Display method
    void Display() const;
};

#endif // FACILITY_H
