Smart Campus Navigation and Facility Analytics System
A C++ project for organizing campus departments and facilities using fundamental data structures. The current codebase implements a hierarchical department tree, a facility manager, and a custom hash table for facility ID lookups. It is designed as the foundation for a larger campus navigation and analytics application.
Project status: Work in progress. The data-structure classes and file loaders are implemented, but src/main.cpp is incomplete. The project does not yet provide a runnable command-line menu, map-based navigation, routing, or analytics dashboard.

Features implemented
- Department hierarchy: Store departments in a general (multi-child) tree; add departments, search by name, display parent/child/depth information, and print the hierarchy or preorder traversal.
- Facility records: Represent facilities by ID, name, building, and category; load records in file order, list them, and search by ID.
- Fast facility lookup: A custom hash table maps facility IDs to their positions in the facility list. It uses the DJB2 hash function and separate chaining for collisions.
- Input validation: Reject malformed records, duplicate department names or facility IDs, missing fields, unknown department parents, and facility references to unknown buildings.
- Loader diagnostics: Report accepted/rejected record counts and hash-table capacity, entries, and collisions.
- Case-insensitive matching: Normalize facility IDs and compare department names without case sensitivity.
Technologies and concepts
- Language: C++ (standard library; C++11 or later recommended)
- Data structures: General tree, vectors, linked lists, hash table
- Concepts: File I/O, parsing, validation, recursion, object-oriented design, dynamic memory management, copy constructors, and assignment operators
Project structure
SmartCampusNavigation-main/
├── include/
│   ├── DepartmentNode.h       # Individual department tree node
│   ├── DepartmentTree.h       # Department hierarchy and operations
│   ├── Facility.h             # Facility data model
│   ├── FacilityManager.h      # Facility loading, validation, and search
│   ├── HashTable.h            # Custom separate-chaining hash table
│   └── Utils.h                # Text and file-path utilities
├── src/
│   ├── DepartmentNode.cpp
│   ├── DepartmentTree.cpp
│   ├── Facility.cpp
│   ├── FacilityManager.cpp
│   ├── HashTable.cpp
│   ├── Utils.cpp
│   └── main.cpp               # Incomplete application entry point
├── data/                      # Placeholder; sample datasets not included
├── tempdata/                  # Placeholder
└── .gitignore
Getting started
Prerequisites
Install a C++ compiler such as GCC (g++) or Clang that supports C++11 or newer.
Compile the existing source modules
From the repository root:
g++ -std=c++11 -Wall -Wextra -Iinclude -c src/DepartmentNode.cpp src/DepartmentTree.cpp src/Facility.cpp src/FacilityManager.cpp src/HashTable.cpp src/Utils.cpp
This compiles the implemented components into object files. It does not create an executable because src/main.cpp does not yet define a complete main() function.
Build the application after completing main.cpp
Once an application entry point has been implemented, compile and link all source files:
g++ -std=c++11 -Wall -Wextra -Iinclude src/*.cpp -o campus
Run the finished application with the data-directory argument expected by the design:
./campus data
On Windows, the executable will typically be campus.exe. The command-line argument handling and interactive menu still need to be implemented.
Data file formats
The current loaders support pipe-delimited (|) text files. Blank lines and lines beginning with # are skipped.
departments.txt
Each record contains a department name and parent name:
# Department | Parent
University | none
College of Engineering | University
Computer Science | College of Engineering
Electrical Engineering | College of Engineering
The first accepted department becomes the root and uses none as its parent. Subsequent parent departments must already exist in the tree. Department names must be unique, ignoring case.
facilities.txt
Each record contains an ID, facility name, building, and category:
# ID | Name | Building | Category
F001 | Computer Lab | Discovery Park | Laboratory
F002 | Student Study Room | Willis Library | Study Space
F003 | Advising Office | Discovery Park | Office
Facility IDs must follow the pattern F plus exactly three digits (for example, F001). IDs are unique regardless of letter case. A facility's building must appear in the list of valid buildings passed to FacilityManager::LoadFromFile().

Note: The examples above illustrate the expected formats; the uploaded repository does not contain populated department, facility, or building datasets. A buildings.txt loader and the complete four-file startup workflow mentioned in the main.cpp design comment are not included in the current snapshot.
Architecture
Component	Responsibility
DepartmentNode	Holds a department's name, parent, children, and depth.
DepartmentTree	Owns the department nodes; loads, searches, adds, and traverses the hierarchy.
Facility	Encapsulates a facility's identifying information.
FacilityManager	Validates and stores facilities and coordinates ID-based lookups.
HashTable	Stores facility-ID-to-index mappings using DJB2 hashing and separate chaining.
Utils	Provides trimming, case conversion, pipe splitting, blank/comment detection, and path construction.


The department tree supports any number of children per node. Facility records are kept in a vector in insertion order, while the hash table enables average-case constant-time ID lookup, subject to collisions and load factor.
Development roadmap
- [x] Implement department tree and traversal operations
- [x] Implement facility records and validation
- [x] Implement a custom hash table with collision tracking
- [x] Implement department and facility text-file loaders
- [ ] Finish main.cpp and the interactive command-line menu
- [ ] Add building data loading and integrate all expected input files
- [ ] Supply example datasets and automated tests
- [ ] Implement actual campus navigation/routing features
- [ ] Add facility analytics and reporting features
Limitations
The current repository contains reusable C++ modules rather than a finished end-user application. In particular, the navigation and analytics functionality described by the project title has not yet been implemented. Changes made to the in-memory department tree are not written back to departments.txt.
License
No license file was included in the supplied project. Add a license if you intend to specify permissions for reuse or distribution.
