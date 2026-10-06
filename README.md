# Campus Navigation System

A console-based campus navigation system written in C++ as a Data Structures and Algorithms (DSA) course project

The program lets an administrator build and maintain a map of the campus, and lets regular users search for places and find the shortest route between them.

## Features

### Admin Panel (password-protected)
- Add, update, and delete campus locations
- Add, update, and delete paths between locations (with distances)
- Save all changes to `campus_data.txt`

### User Panel
- Browse and search campus locations
- Find the shortest path between two locations
- Explore the campus map through graph traversal

## Data Structures and Algorithms Used

| Data structures | Array, Linked List, Stack, Queue, Binary Search Tree (BST), Graph |
| Traversal | Breadth-First Search (BFS), Depth-First Search (DFS) |
| Searching | Searching algorithms for locating places |
| Sorting | merge sort |
| Shortest path | Dijkstra's algorithm |

## Data File

All campus data is stored in a single, manually maintained text file: `campus_data.txt`.

- If `campus_data.txt` is missing, the program starts with an empty campus.
- No sample or predefined data is shipped with the project.
- The admin populates the data through the Admin Panel, and the program writes it back to the file.

## Getting Started

### Requirements
- A C++ compiler with C++11 support or later (e.g., `g++`)

### Build

```bash
g++ -std=c++11 -o campus_nav main.cpp
```

Adjust the source file names if the project is split across multiple files.

### Run

```bash
./campus_nav
```

## Usage

1. Launch the program and choose **Admin Panel** or **User Panel** from the main menu.
2. **First run:** log in as admin and add locations and paths, since the campus starts empty.
3. As a user, search for locations or request the shortest route between two places.

## Project Structure

```
.
├── main.cpp            # Entry point and menu handling
├── campus_data.txt     # Campus data (created/maintained by the admin)
└── README.md
```

Update this section to match your actual files.

## Author

Pronoy Gain
