// Utils.cpp
// Design: Implements the shared text helpers declared in Utils.h.
#include "Utils.h"
#include <sstream>
using namespace std;

// Removes whitespace from space, tab, '\r', '\n', from both ends only
string trim(string text) {
    const string whitespace = " \t\r\n";
    size_t first = text.find_first_not_of(whitespace);
    if (first == string::npos) {
        return ""; 
    }
    size_t last = text.find_last_not_of(whitespace);
    return text.substr(first, last - first + 1);
}

// upper-casing
string toUpperCase(string text) {
    for (size_t i = 0; i < text.size(); i++) {
        if (text.at(i) >= 'a' && text.at(i) <= 'z') {
            text.at(i) = text.at(i) - 'a' + 'A';
        }
    }
    return text;
}

// lower-casing
string toLowerCase(string text) {
    for (size_t i = 0; i < text.size(); i++) {
        if (text.at(i) >= 'A' && text.at(i) <= 'Z') {
            text.at(i) = text.at(i) - 'A' + 'a';
        }
    }
    return text;
}

// Splits on '|' with stringstream + getline. getline drops an empty section, so a line ending in '|' gets that empty section added back
int splitBatch(const string& line, vector<string>& batch) {
    batch.clear();
    stringstream stream(line);
    string piece;
    while (getline(stream, piece, '|')) {
        batch.push_back(piece);
    }
    if (!line.empty() && line.at(line.size() - 1) == '|') {
        batch.push_back("");
    }
    return (int)batch.size();
}

// Blank lines and lines whose first character is '#'
bool isSkippableLine(const string& line) {
    string cleaned = trim(line);
    if (cleaned.empty()) {
        return true;
    }
    return cleaned.at(0) == '#';
}

// Adds a '/' between directory and file name when needed
string buildPath(const string& directory, const string& filename) {
    if (directory.empty()) {
        return filename;
    }
    if (directory.at(directory.size() - 1) == '/') {
        return directory + filename;
    }
    return directory + "/" + filename;
}
