// HashTable.h
//Design: Table designed to map a facility ID to its corresponding position in the loaded list.
// Utilises separate chaining; each bucket contains a linked list of keys that hash to it. New keys are appended to the end of the list.

// Hash function: DJB2 over the uppercased key, reduced with modulo capacity.
//hash = 5381; for each character c: hash = hash * 33 + c     |     index = hash % capacity

#ifndef HASH_TABLE_H
#define HASH_TABLE_H

#include <iostream>
#include <string>
#include <vector>
using namespace std;

// Initial number of buckets
const int DEFAULT_HASH_CAPACITY = 13;

// One value pair in a bucket's chain
class HashNode {
public:
    string key;
    int value;
    HashNode* next;

    HashNode(const string& keyText, int valueNumber) : key(keyText), value(valueNumber), next(nullptr) {}
};

class HashTable {
private:
    vector<HashNode*> buckets; // head of the chain
    int entries;               // number of keys stored
    int collisions;            // collisions counted during insertions

    // Private helper functions
    void clearBuckets();
    void copyFrom(const HashTable& other);

public:
    // Constructors and the copy-control functions
    HashTable(int initialCapacity = DEFAULT_HASH_CAPACITY);
    HashTable(const HashTable& other);
    ~HashTable();
    HashTable& operator =(const HashTable& other);

    // Maps a key to a bucket number from 0 to capacity - 1.
    int HashFunction(const string& key) const;

    // Insert returns false if key is already stored, ignoring case. Find returns true and fills value when key is stored.
    bool Insert(const string& key, int value);
    bool Find(const string& key, int& value) const;

    // Diagnostic info
    int GetCapacity() const;
    int GetEntries() const;
    int GetCollisions() const;
};

#endif // HASH_TABLE_H
