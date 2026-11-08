// Utils.h
// Design: text helpers used by loaders and menu. Trimming, case conversion, splitting a '|',detecting blank and comment lines, and building a file path.

#ifndef UTILS_H
#define UTILS_H

#include <iostream>
#include <string>
#include <vector>
using namespace std;

// Removes spaces, tabs, '\r' and '\n'
string trim(string text);

// Copy of the text in upper / lower case
string toUpperCase(string text);
string toLowerCase(string text);

// Splits a line on '|' into sections
// Returns the number of sections found
int splitBatch(const string& line, vector<string>& batch);

// True when the line is blank, or first character is '#'
bool isSkippableLine(const string& line);

// Connect a data directory and a file name
// buildPath("data/", "buildings.txt") and buildPath("data", "buildings.txt")
// both give "data/buildings.txt".
string buildPath(const string& directory, const string& filename);

#endif // UTILS_H
