// HashTable.cpp
// Design: Implements the separate-chaining hash table declared in HashTable.h.
#include "HashTable.h"
#include "Utils.h"
using namespace std;

// Creates the empty buckets. A capacity below 1 falls back to default.
HashTable::HashTable(int initialCapacity) : buckets(), entries(0), collisions(0) {
    if (initialCapacity < 1) {
        initialCapacity = DEFAULT_HASH_CAPACITY;
    }
    buckets.assign(initialCapacity, nullptr);
}

// Copy constructor
HashTable::HashTable(const HashTable& other) : buckets(), entries(0), collisions(0) {
    copyFrom(other);
}

// Destructor
HashTable::~HashTable() {
    clearBuckets();
}

// Assignment operator: frees the old chains, then copies the other table
HashTable& HashTable::operator =(const HashTable& other) {
    if (this != &other) {
        clearBuckets();
        copyFrom(other);
    }
    return *this;
}

// Deletes all node and leaves all buckets empty
void HashTable::clearBuckets() {
    for (size_t i = 0; i < buckets.size(); i++) {
        HashNode* current = buckets.at(i);
        while (current != nullptr) {
            HashNode* doomed = current;
            current = current->next;
            delete doomed;
        }
        buckets.at(i) = nullptr;
    }
    entries = 0;
    collisions = 0;
}

// Copies capacity, counters and every chain from another table
void HashTable::copyFrom(const HashTable& other) {
    buckets.assign(other.buckets.size(), nullptr);
    for (size_t i = 0; i < other.buckets.size(); i++) {
        HashNode* source = other.buckets.at(i);
        HashNode* last = nullptr;
        while (source != nullptr) {
            HashNode* created = new HashNode(source->key, source->value);
            if (last == nullptr) {
                buckets.at(i) = created;
            } else {
                last->next = created;
            }
            last = created;
            source = source->next;
        }
    }
    entries = other.entries;
    collisions = other.collisions;
}

// DJB2: start at 5381. Key is uppercased, so "f004" and "F004" share a bucket
int HashTable::HashFunction(const string& key) const {
    string normalized = toUpperCase(key);
    unsigned long long hash = 5381;
    for (size_t i = 0; i < normalized.size(); i++) {
        hash = hash * 33 + (unsigned char)normalized.at(i);
    }
    return (int)(hash % (unsigned long long)buckets.size());
}

// Traverse the chain linked to the key hashes. If the key already exists, it indicates a duplicate, and no changes are made 
//If the key is unique, the new node is added to the end of the chain. If the chain previously contained nodes, this addition is counted as one collision
bool HashTable::Insert(const string& key, int value) {
    string normalized = toUpperCase(key);
    int index = HashFunction(normalized);

    HashNode* current = buckets.at(index);
    HashNode* last = nullptr;
    while (current != nullptr) {
        if (current->key == normalized) {
            return false;
        }
        last = current;
        current = current->next;
    }

    HashNode* created = new HashNode(normalized, value);
    if (last == nullptr) {
        buckets.at(index) = created;
    } else {
        last->next = created;
        collisions++;
    }
    entries++;
    return true;
}

// Looks only in the key's own bucket
bool HashTable::Find(const string& key, int& value) const {
    string normalized = toUpperCase(key);
    HashNode* current = buckets.at(HashFunction(normalized));
    while (current != nullptr) {
        if (current->key == normalized) {
            value = current->value;
            return true;
        }
        current = current->next;
    }
    return false;
}

// Number of buckets
int HashTable::GetCapacity() const {
    return (int)buckets.size();
}

// Number of keys stored
int HashTable::GetEntries() const {
    return entries;
}

// Collisions counted
int HashTable::GetCollisions() const {
    return collisions;
}
