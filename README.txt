Smart Campus Navigation and Facility Analytics System
CSCE 2110 Project 2 - Milestone 1

PURPOSE
````````````````````````````````````````````````````````````````````````
A program loads a small campus dataset from four text files, allowing users to explore it through a menu-driven interface.

  - Facilities are organized using a custom hash table with separate chaining, keyed by facility ID, which enables efficient ID-based searches.
  - Buildings and their interconnections are represented as an undirected graph using adjacency lists.
    Users can perform a breadth-first search (BFS) to list all reachable buildings from a chosen starting point, along with their respective distances.
  - Departments are structured in a general tree format, allowing any department to have multiple sub-departments.
    The tree can be displayed in a preorder traversal, and users can search for and add departments directly from the menu.

Every file is checked while loading. A rejected record prints a warning with
the file name, line number and reason, and each file prints a summary line.

BUILD AND RUN
````````````````````````````````````````````````````````````````````````
From the Project2 folder:

  Build:        g++ -Iinclude -o campus src/*.cpp
  Run:          ./campus
  Run with     ./campus testdata/case1_bad_records
  another
  data folder:

The data-directory argument is optional. If not provided, the program will look for four specific files in the data/ folder: buildings.txt, connections.txt, facilities.txt, and departments.txt.
 Any file that fails to open will be reported and treated as empty.

MENU Milestone 1
````````````````````````````````````````````````````````````````````````
   1. View all facilities
   2. Search facility by ID
   3. View campus graph
   4. Display graph counts and degrees
   5. Find reachable buildings (BFS)
   6. Add department
   7. Search department
   8. Display department hierarchy
   9. Run preorder traversal
  10. Exit            (0 also exits)

Notes on input:
  - IDs and names are matched without regard for capitalization, and leading and trailing spaces are disregarded. Spaces within a name are preserved.
  - A facility ID follows the format F followed by three digits (e.g., F004).
  - Names may contain spaces; however, a blank name results in an error and returns to the menu.
    Invalid menu input (including blank input, letters, or out-of-range values) triggers an error message, and the menu is displayed again.
  - Departments added through the menu exist only in memory; the file departments.txt remains unchanged. If the tree is empty, the new department will become the root without prompting for a parent.

DIRECTORY LAYOUT
````````````````````````````````````````````````````````````````````````
  include/             Header files
  src/                 Source files, including main.cpp
  data/                Four provided starter files
  testdata/            Our test files, one folder per case:
      case1_bad_records            Every kind of bad record
      case2_missing_files          Connections.txt and facilities.txt missing
      case3_collision              IDs that collide in the hash table and a duplicate ID
      case3b_collision_no_duplicate  Same without the duplicate
      case4_components             Several connected components
      case5_empty_tree            No departments
      case6_root_only              A tree with only the root
  README.txt           This file
  design_document.txt  Data structures and design decisions
  test_results.txt     Recorded tests
  CONTRIBUTIONS.txt    Which parts are who's and shared work
  .gitignore           Excludes executables, object files and editor files

SOURCE MODULES
````````````````````````````````````````````````````````````````````````
  main.cpp             Reads the data-directory argument, loads the files in order (buildings, connections, facilities, departments) and runs the menu.
  Utils                Shared text helpers: trim, upper-casing, splitting a line on '|', skipping blank and comment lines.
  Facility             Facility record.
  FacilityManager      Loads and validates facilities.txt, keeps the list of facilities, and searches by ID through the hash table.
  HashTable            Custom hash table, separate chaining, keyed by ID.
  CampusGraph          Loads buildings and connections, stores the graph, displays it, and runs BFS.
  DepartmentNode       One department in the tree.
  DepartmentTree       Loads departments.txt, adds and searches departments and displays the hierarchy and preorder traversal.

TESTING
````````````````````````````````````````````````````````````````````````
test_results.txt lists each test with its input, expected result, testing result and pass or fail. 
Each test can be repeated by running the program with the data folder shown for that test and typing the listed input.
