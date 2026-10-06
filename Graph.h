#ifndef GRAPH_H
#define GRAPH_H

#include <string>

#include "location.h"
#include "LinkedList.h"
#include "BST.h"
#include "Queue.h"
#include "Stack.h"

using namespace std;

class CampusGraph
{
private:
    static const int MAX_LOCATIONS = 1000;

    //used array and linked list 
    Location locations[MAX_LOCATIONS];
    AdjList adjacency[MAX_LOCATIONS];
    int locationIndicesById[MAX_LOCATIONS];
    int locationCount;
    LocationBST nameIndex;
    int nextId;

    // Binary search ID index
    int findLocationIndex(int id) const;
    bool calculateShortestPaths(
        int sourceIndex,
        int steps[],
        int previous[]
    ) const;
    bool calculateShortestPathsByDistance(
        int sourceIndex,
        long long distances[],
        int previous[]
    ) const;
    bool findCycleFrom(
        int currentIndex,
        int startIndex,
        int maxEdges,
        int pathLength,
        bool visited[],
        int path[]
    ) const;
    void displayRoute(PathStack& route) const;
    void displayNavigation(
        const int routeIndices[],
        int routeLength,
        int shortestDistance) const;
    bool appendHistoryEntry(
        string type,
        string status,
        long long metric,
        const int routeIndices[],
        int routeLength,
        string filename) const;
    void clearGraph();

public:
    CampusGraph();

    bool addLocation(string name);
    bool addLocationWithId(int id, string name);
    bool removeLocation(int id);
    bool renameLocation(string currentName, string newName);

    bool addOrUpdatePath(
        int from,
        int to,
        CompassDirection direction = CompassDirection::Unknown,
        int distance = 1);
    bool removePath(int from, int to);

    bool getLocationById(int id, Location& location) const;
    bool findLocationByName(string name, int& id);

    void displayLocations(bool showIds = true);
    void displayConnections(bool showIds = true);

    bool shortestPath(
        int source,
        int destination,
        string historyFile = "route_history.txt");
    bool shortestPathByDistance(
        int source,
        int destination,
        string historyFile = "route_history.txt") const;
    bool displayRouteHistory(string filename) const;
    bool clearRouteHistory(string filename) const;
    bool depthFirstConnectivity(
        int source,
        int destination,
        string historyFile = "route_history.txt") const;
    bool displayCyclicPath(int startId) const;

    bool loadFromFile(string filename);
    bool saveToFile(string filename);

    int getNextId() const;
    int getLocationCount() const;
};

#endif
