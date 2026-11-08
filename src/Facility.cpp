// Facility.cpp
// Design: Implements the Facility record declared in Facility.h.
#include "Facility.h"
using namespace std;

// Constructors
Facility::Facility() : facilityID(""), facilityName(""), building(""), category("") {}

Facility::Facility(const string& id, const string& name, const string& buildingName, const string& categoryName)
    : facilityID(id), facilityName(name), building(buildingName), category(categoryName) {}

// Accessors (Getters)
string Facility::GetID() const { 
    return facilityID;
 }
string Facility::GetName() const { 
    return facilityName; 
}
string Facility::GetBuilding() const { 
    return building; 
}
string Facility::GetCategory() const { 
    return category;
 }

// Mutators (Setters)
void Facility::SetID(const string& id) {
    facilityID = id; 
}
void Facility::SetName(const string& name) { 
    facilityName = name;
 }
void Facility::SetBuilding(const string& buildingName) {
     building = buildingName; 
    }
void Facility::SetCategory(const string& categoryName) {
     category = categoryName; 
    }

// Prints: ID | Name | Building | Category
void Facility::Display() const {
    cout << facilityID << " | " << facilityName << " | " << building << " | " << category << endl;
}
