#include "Graph.h"

#include "Sorting.h"

#include <fstream>
#include <iostream>
#include <sstream>
#include <sys/stat.h>
#include <cerrno>
#include <climits>
#include <vector>

using namespace std;

CampusGraph::CampusGraph()
{
    locationCount = 0;
    nextId = 1;
}

int CampusGraph::findLocationIndex(int id) const
{
    int low = 0;
    int high = locationCount - 1;
    while (low <= high)
    {
        int middle = low + (high - low) / 2;
        int locationIndex = locationIndicesById[middle];
        int currentId = locations[locationIndex].id;
        if (currentId == id)
        {
            return locationIndex;
        }
        if (currentId < id)
        {
            low = middle + 1;
        }
        else
        {
            high = middle - 1;
        }
    }

    return -1;
}

void CampusGraph::clearGraph()
{
    for (int i = 0; i < MAX_LOCATIONS; i++)
    {
        adjacency[i].clear();
    }

    locationCount = 0;
    nextId = 1;
    nameIndex.clear();
}

bool CampusGraph::addLocation(string name)
{
    if (name.empty() || locationCount >= MAX_LOCATIONS)
    {
        return false;
    }

    return addLocationWithId(nextId, name);
}

bool CampusGraph::addLocationWithId(int id, string name)
{
    if (
        id <= 0 ||
        name.empty() ||
        locationCount >= MAX_LOCATIONS ||
        findLocationIndex(id) != -1
    )
    {
        return false;
    }

    int newLocationIndex = locationCount;
    locations[newLocationIndex] = Location(id, name);
    int insertAt = locationCount;
    while (insertAt > 0 &&
           locations[locationIndicesById[insertAt - 1]].id > id)
    {
        locationIndicesById[insertAt] = locationIndicesById[insertAt - 1];
        insertAt--;
    }
    locationIndicesById[insertAt] = newLocationIndex;
    nameIndex.insert(name, id);
    locationCount++;

    if (id >= nextId)
    {
        nextId = id + 1;
    }

    return true;
}

bool CampusGraph::removeLocation(int id)
{
    int index = findLocationIndex(id);

    if (index == -1)
    {
        return false;
    }

    nameIndex.remove(locations[index].name);

    for (int i = 0; i < locationCount; i++)
    {
        adjacency[i].remove(id);
    }
    adjacency[index].clear();

    for (int i = index; i < locationCount - 1; i++)
    {
        locations[i] = locations[i + 1];
        adjacency[i].moveFrom(adjacency[i + 1]);
    }

    int indexPosition = 0;
    while (locationIndicesById[indexPosition] != index)
    {
        indexPosition++;
    }
    for (int i = indexPosition; i < locationCount - 1; i++)
    {
        locationIndicesById[i] = locationIndicesById[i + 1];
    }
    for (int i = 0; i < locationCount - 1; i++)
    {
        if (locationIndicesById[i] > index)
        {
            locationIndicesById[i]--;
        }
    }

    adjacency[locationCount - 1].clear();
    locationCount--;
    return true;
}

bool CampusGraph::renameLocation(string currentName, string newName)
{
    if (newName.empty())
    {
        return false;
    }

    int id;
    if (!nameIndex.find(currentName, id))
    {
        return false;
    }

    int existingId;
    if (nameIndex.find(newName, existingId) && existingId != id)
    {
        return false;
    }

    int index = findLocationIndex(id);
    if (index == -1)
    {
        return false;
    }

    nameIndex.remove(locations[index].name);
    locations[index].name = newName;
    nameIndex.insert(newName, id);
    return true;
}

bool CampusGraph::addOrUpdatePath(
    int from,
    int to,
    CompassDirection direction,
    int distance)
{
    int fromIndex = findLocationIndex(from);
    int toIndex = findLocationIndex(to);

    if (
        from == to ||
        fromIndex == -1 ||
        toIndex == -1 ||
        distance < 0
    )
    {
        return false;
    }

    adjacency[fromIndex].insert(to, direction, distance);
    adjacency[toIndex].insert(from, oppositeDirection(direction), distance);
    return true;
}

bool CampusGraph::removePath(int from, int to)
{
    int fromIndex = findLocationIndex(from);
    int toIndex = findLocationIndex(to);

    if (fromIndex == -1 || toIndex == -1)
    {
        return false;
    }

    bool removedForward = adjacency[fromIndex].remove(to);
    bool removedReverse = adjacency[toIndex].remove(from);
    return removedForward || removedReverse;
}

bool CampusGraph::getLocationById(int id, Location& location) const
{
    int index = findLocationIndex(id);

    if (index == -1)
    {
        return false;
    }

    location = locations[index];
    return true;
}

bool CampusGraph::findLocationByName(string name, int& id)
{

    return nameIndex.find(name, id);
}

void CampusGraph::displayConnections(bool showIds)
{
    if (locationCount == 0)
    {
        cout << "\nNo locations available.\n";
        return;
    }

    cout << "\n========== CONNECTIONS ==========\n";

    for (int i = 0; i < locationCount; i++)
    {
        cout << "\n" << locations[i].name;
        if (showIds)
        {
            cout << " (" << locations[i].id << ')';
        }
        cout << ":\n";

        EdgeNode* edge = adjacency[i].getHead();
        if (edge == nullptr)
        {
            cout << "  No connections\n";
            continue;
        }

        while (edge != nullptr)
        {
            int destinationIndex = findLocationIndex(edge->destId);
            if (destinationIndex != -1)
            {
                cout << "  <-> " << locations[destinationIndex].name;
                if (showIds)
                {
                    cout << " (" << edge->destId << ')';
                }
                if (edge->direction == CompassDirection::Unknown)
                {
                    cout << " [heading not configured]";
                }
                else
                {
                    cout << " [" << compassDirectionName(edge->direction)
                         << "]";
                }
                cout << " [" << edge->distance << " distance units]\n";
            }

            edge = edge->next;
        }
    }
}

void CampusGraph::displayLocations(bool showIds)
{
    if (locationCount == 0)
    {
        cout << "\nNo locations available.\n";
        return;
    }

    Location* sortedLocations = new Location[locationCount];
    for (int i = 0; i < locationCount; i++)
    {
        sortedLocations[i] = locations[i];
    }

    // Merge sort
    mergeSortByName(sortedLocations, 0, locationCount - 1);

    cout << "\n========== LOCATIONS ==========\n";
    for (int i = 0; i < locationCount; i++)
    {
        if (showIds)
        {
            cout << sortedLocations[i].id << " | ";
        }
        cout << sortedLocations[i].name << '\n';
    }

    delete[] sortedLocations;
}

bool CampusGraph::shortestPath(
    int source,
    int destination,
    string historyFile)
{
    int sourceIndex = findLocationIndex(source);
    int destinationIndex = findLocationIndex(destination);

    if (sourceIndex == -1 || destinationIndex == -1)
    {
        cout << "\nInvalid source or destination location.\n";
        return false;
    }

    int steps[MAX_LOCATIONS];
    int previous[MAX_LOCATIONS];
    if (!calculateShortestPaths(sourceIndex, steps, previous))
    {
        cout << "\nUnable to calculate the shortest path.\n";
        return false;
    }
    if (steps[destinationIndex] == -1)
    {
        cout << "\nNo route exists between these locations.\n";
        int endpoints[] = {sourceIndex, destinationIndex};
        if (!appendHistoryEntry(
                "SHORTEST",
                "NOT_CONNECTED",
                -1,
                endpoints,
                2,
                historyFile))
        {
            cout << "Unable to save route history to "
                 << historyFile << ".\n";
        }
        return false;
    }

    int routeIndices[MAX_LOCATIONS];
    int routeLength = 0;
    PathStack route;
    for (int current = destinationIndex; current != -1; current = previous[current])
    {
        if (routeLength == MAX_LOCATIONS)
        {
            cout << "\nUnable to reconstruct the route.\n";
            return false;
        }
        routeIndices[routeLength++] = current;
        route.push(locations[current].id);
    }

    for (int left = 0, right = routeLength - 1; left < right; left++, right--)
    {
        int temp = routeIndices[left];
        routeIndices[left] = routeIndices[right];
        routeIndices[right] = temp;
    }

    cout << "\n========== SHORTEST PATH ==========\n";
    cout << "Connection found.\n";
    displayRoute(route);
    displayNavigation(routeIndices, routeLength, steps[destinationIndex]);

    if (appendHistoryEntry(
            "SHORTEST",
            "CONNECTED",
            steps[destinationIndex],
            routeIndices,
            routeLength,
            historyFile))
    {
        cout << "Route added to history.\n";
    }
    else
    {
        cout << "Unable to save route history to "
             << historyFile << ".\n";
    }
    return true;
}

bool CampusGraph::shortestPathByDistance(
    int source,
    int destination,
    string historyFile) const
{
    int sourceIndex = findLocationIndex(source);
    int destinationIndex = findLocationIndex(destination);
    if (sourceIndex == -1 || destinationIndex == -1)
    {
        cout << "\nInvalid source or destination location.\n";
        return false;
    }

    long long distances[MAX_LOCATIONS];
    int previous[MAX_LOCATIONS];
    if (!calculateShortestPathsByDistance(
            sourceIndex,
            distances,
            previous))
    {
        cout << "\nUnable to calculate the shortest path by distance.\n";
        return false;
    }
    if (distances[destinationIndex] == LLONG_MAX)
    {
        cout << "\nNo route exists between these locations.\n";
        int endpoints[] = {sourceIndex, destinationIndex};
        if (!appendHistoryEntry(
                "DIJKSTRA",
                "NOT_CONNECTED",
                -1,
                endpoints,
                2,
                historyFile))
        {
            cout << "Unable to save route history to "
                 << historyFile << ".\n";
        }
        return false;
    }

    int routeIndices[MAX_LOCATIONS];
    int routeLength = 0;
    PathStack route;
    for (int current = destinationIndex; current != -1; current = previous[current])
    {
        if (routeLength == MAX_LOCATIONS)
        {
            cout << "\nUnable to reconstruct the route.\n";
            return false;
        }
        routeIndices[routeLength++] = current;
        route.push(locations[current].id);
    }
    for (int left = 0, right = routeLength - 1; left < right; left++, right--)
    {
        int temp = routeIndices[left];
        routeIndices[left] = routeIndices[right];
        routeIndices[right] = temp;
    }

    cout << "\n========== SHORTEST PATH BY DISTANCE ==========\n";
    displayRoute(route);
    displayNavigation(routeIndices, routeLength, -1);
    cout << "Total distance: " << distances[destinationIndex]
         << " distance units.\n";
    if (!appendHistoryEntry(
            "DIJKSTRA",
            "CONNECTED",
            distances[destinationIndex],
            routeIndices,
            routeLength,
            historyFile))
    {
        cout << "Unable to save route history to "
             << historyFile << ".\n";
    }
    return true;
}

bool CampusGraph::appendHistoryEntry(
    string type,
    string status,
    long long metric,
    const int routeIndices[],
    int routeLength,
    string filename) const
{
    ofstream file(filename.c_str(), ios::app);
    if (!file.is_open())
    {
        return false;
    }

    PathStack route;
    for (int i = 0; i < routeLength; i++)
    {
        route.push(locations[routeIndices[i]].id);
    }

    file << "HISTORY_LIFO|" << type << '|' << status << '|' << metric << '|';
    bool firstId = true;
    while (!route.empty())
    {
        if (!firstId)
        {
            file << ' ';
        }
        file << route.pop();
        firstId = false;
    }
    file << '\n';
    return static_cast<bool>(file);
}

bool CampusGraph::displayRouteHistory(string filename) const
{
    ifstream file(filename.c_str());
    if (!file.is_open())
    {
        struct stat fileInfo;
        if (stat(filename.c_str(), &fileInfo) != 0 && errno == ENOENT)
        {
            cout << "\nNo route history yet.\n";
            return true;
        }

        cout << "\nUnable to open route history file: "
             << filename << ".\n";
        return false;
    }

    vector<string> historyLines;
    string line;
    while (getline(file, line))
    {
        historyLines.push_back(line);
    }
    if (!file.eof())
    {
        cout << "Error while reading route history file.\n";
        return false;
    }

    cout << "\n========== ROUTE HISTORY ==========\n";
    int routeNumber = 0;
    for (size_t lineIndex = historyLines.size(); lineIndex > 0; lineIndex--)
    {
        const string& line = historyLines[lineIndex - 1];
        int lineNumber = static_cast<int>(lineIndex);
        if (line.compare(0, 8, "HISTORY|") == 0 ||
            line.compare(0, 13, "HISTORY_LIFO|") == 0)
        {
            istringstream fields(line);
            string marker;
            string type;
            string status;
            string metricText;
            string idsText;
            getline(fields, marker, '|');
            getline(fields, type, '|');
            getline(fields, status, '|');
            getline(fields, metricText, '|');
            getline(fields, idsText);

            bool lifoStored = marker == "HISTORY_LIFO";
            if (!lifoStored)
            {
                // Older history entries stored a timestamp before the route IDs.
                size_t legacySeparator = idsText.find('|');
                if (legacySeparator != string::npos)
                {
                    idsText = idsText.substr(legacySeparator + 1);
                }
            }

            istringstream metricInput(metricText);
            long long metric;
            if ((marker != "HISTORY" && !lifoStored) ||
                (type != "SHORTEST" &&
                 type != "DIJKSTRA" &&
                 type != "CONNECTIVITY") ||
                (status != "CONNECTED" && status != "NOT_CONNECTED") ||
                !(metricInput >> metric) || !metricInput.eof())
            {
                cout << "Skipping invalid history entry on line "
                     << lineNumber << ".\n";
                continue;
            }

            int routeIds[MAX_LOCATIONS];
            int routeLength = 0;
            istringstream idsInput(idsText);
            int id;
            while (idsInput >> id)
            {
                if (id <= 0 || routeLength == MAX_LOCATIONS)
                {
                    break;
                }
                routeIds[routeLength++] = id;
            }
            if (!idsInput.eof() ||
                (status == "CONNECTED" && routeLength == 0) ||
                (status == "NOT_CONNECTED" && routeLength != 2))
            {
                cout << "Skipping invalid history entry on line "
                     << lineNumber << ".\n";
                continue;
            }

            cout << ++routeNumber << ". ";
            if (type == "SHORTEST")
            {
                cout << "Fewest-connections route";
            }
            else if (type == "DIJKSTRA")
            {
                cout << "Dijkstra distance path";
            }
            else
            {
                cout << "Connectivity check";
            }
            cout << " | " << status;
            if (type == "DIJKSTRA" && status == "CONNECTED")
            {
                cout << " | Distance: " << metric;
            }
            else if (type == "SHORTEST" && status == "CONNECTED")
            {
                cout << " | Connections: " << metric;
            }
            if (routeLength > 0)
            {
                cout << (status == "CONNECTED" ? " | " : " | Endpoints: ");
                PathStack route;
                if (lifoStored)
                {
                    for (int i = 0; i < routeLength; i++)
                    {
                        route.push(routeIds[i]);
                    }
                }
                else
                {
                    for (int i = routeLength - 1; i >= 0; i--)
                    {
                        route.push(routeIds[i]);
                    }
                }
                bool firstLocation = true;
                while (!route.empty())
                {
                    if (!firstLocation)
                    {
                        cout << " -> ";
                    }
                    int locationIndex = findLocationIndex(route.pop());
                    if (locationIndex == -1)
                    {
                        cout << "[removed location]";
                    }
                    else
                    {
                        cout << locations[locationIndex].name;
                    }
                    firstLocation = false;
                }
            }
            cout << '\n';
            continue;
        }

        istringstream input(line);
        int routeIds[MAX_LOCATIONS];
        int routeLength = 0;
        int id;
        while (input >> id)
        {
            if (id <= 0 || routeLength == MAX_LOCATIONS)
            {
                break;
            }
            routeIds[routeLength++] = id;
        }

        if (routeLength == 0 || !input.eof())
        {
            cout << "Skipping invalid history entry on line "
                 << lineNumber << ".\n";
            continue;
        }

        PathStack route;
        for (int i = routeLength - 1; i >= 0; i--)
        {
            route.push(routeIds[i]);
        }

        cout << ++routeNumber << ". ";
        while (!route.empty())
        {
            int routeId = route.pop();
            int locationIndex = findLocationIndex(routeId);
            if (locationIndex == -1)
            {
                cout << "[removed location]";
            }
            else
            {
                cout << locations[locationIndex].name;
            }
            if (!route.empty())
            {
                cout << " -> ";
            }
        }
        cout << '\n';
    }

    if (routeNumber == 0)
    {
        cout << "No valid route history entries.\n";
    }
    return true;
}

bool CampusGraph::clearRouteHistory(string filename) const
{
    ofstream file(filename.c_str(), ios::trunc);
    if (!file.is_open())
    {
        return false;
    }
    file.close();
    return !file.fail();
}

bool CampusGraph::depthFirstConnectivity(
    int source,
    int destination,
    string historyFile) const
{
    int sourceIndex = findLocationIndex(source);
    int destinationIndex = findLocationIndex(destination);

    if (sourceIndex == -1 || destinationIndex == -1)
    {
        cout << "\nInvalid source or destination location.\n";
        return false;
    }

    bool visited[MAX_LOCATIONS] = {};
    int previous[MAX_LOCATIONS];
    for (int i = 0; i < locationCount; i++)
    {
        previous[i] = -1;
    }

    // DFS uses a stack (LIFO) to explore graph vertices.
    PathStack pending;
    pending.push(sourceIndex);
    visited[sourceIndex] = true;

    while (!pending.empty() && !visited[destinationIndex])
    {
        int currentIndex = pending.pop();
        const EdgeNode* edge = adjacency[currentIndex].getHead();

        while (edge != nullptr)
        {
            int neighborIndex = findLocationIndex(edge->destId);
            if (neighborIndex != -1 && !visited[neighborIndex])
            {
                visited[neighborIndex] = true;
                previous[neighborIndex] = currentIndex;
                pending.push(neighborIndex);
            }
            edge = edge->next;
        }
    }

    cout << "\n========== CONNECTIVITY CHECK ==========\n";
    if (!visited[destinationIndex])
    {
        cout << locations[sourceIndex].name << " and "
             << locations[destinationIndex].name
             << " are not connected.\n";
        int endpoints[] = {sourceIndex, destinationIndex};
        if (!appendHistoryEntry(
                "CONNECTIVITY",
                "NOT_CONNECTED",
                -1,
                endpoints,
                2,
                historyFile))
        {
            cout << "Unable to save route history to "
                 << historyFile << ".\n";
        }
        return false;
    }

    int routeIndices[MAX_LOCATIONS];
    int routeLength = 0;
    for (int current = destinationIndex; current != -1; current = previous[current])
    {
        if (routeLength == MAX_LOCATIONS)
        {
            cout << "\nUnable to reconstruct the DFS path.\n";
            return false;
        }
        routeIndices[routeLength++] = current;
    }
    for (int left = 0, right = routeLength - 1; left < right; left++, right--)
    {
        int temp = routeIndices[left];
        routeIndices[left] = routeIndices[right];
        routeIndices[right] = temp;
    }

    PathStack route;
    for (int i = routeLength - 1; i >= 0; i--)
    {
        route.push(locations[routeIndices[i]].id);
    }

    cout << locations[sourceIndex].name << " and "
         << locations[destinationIndex].name << " are connected.\n";
    displayRoute(route);
    displayNavigation(routeIndices, routeLength, -1);
    if (!appendHistoryEntry(
            "CONNECTIVITY",
            "CONNECTED",
            -1,
            routeIndices,
            routeLength,
            historyFile))
    {
        cout << "Unable to save route history to "
             << historyFile << ".\n";
    }
    return true;
}

bool CampusGraph::calculateShortestPaths(
    int sourceIndex,
    int steps[],
    int previous[]
) const
{
    if (sourceIndex < 0 || sourceIndex >= locationCount)
    {
        return false;
    }

    for (int i = 0; i < locationCount; i++)
    {
        steps[i] = -1;
        previous[i] = -1;
    }
    // BFS 
    IntQueue queue;
    if (!queue.enqueue(sourceIndex))
    {
        return false;
    }
    steps[sourceIndex] = 0;

    while (!queue.empty())
    {
        int currentIndex = queue.dequeue();
        const EdgeNode* edge = adjacency[currentIndex].getHead();
        while (edge != nullptr)
        {
            int neighborIndex = findLocationIndex(edge->destId);
            if (neighborIndex != -1 && steps[neighborIndex] == -1)
            {
                steps[neighborIndex] = steps[currentIndex] + 1;
                previous[neighborIndex] = currentIndex;
                if (!queue.enqueue(neighborIndex))
                {
                    return false;
                }
            }
            edge = edge->next;
        }
    }

    return true;
}

bool CampusGraph::calculateShortestPathsByDistance(
    int sourceIndex,
    long long distances[],
    int previous[]
) const
{
    if (sourceIndex < 0 || sourceIndex >= locationCount)
    {
        return false;
    }

    bool visited[MAX_LOCATIONS] = {};
    for (int i = 0; i < locationCount; i++)
    {
        distances[i] = LLONG_MAX;
        previous[i] = -1;
    }
    distances[sourceIndex] = 0;

    for (int iteration = 0; iteration < locationCount; iteration++)
    {
        int currentIndex = -1;
        for (int i = 0; i < locationCount; i++)
        {
            if (!visited[i] &&
                (currentIndex == -1 ||
                 distances[i] < distances[currentIndex]))
            {
                currentIndex = i;
            }
        }

        if (currentIndex == -1 || distances[currentIndex] == LLONG_MAX)
        {
            break;
        }
        visited[currentIndex] = true;

        const EdgeNode* edge = adjacency[currentIndex].getHead();
        while (edge != nullptr)
        {
            int neighborIndex = findLocationIndex(edge->destId);
            if (neighborIndex != -1 && !visited[neighborIndex] &&
                distances[currentIndex] <= LLONG_MAX - edge->distance)
            {
                long long candidate =
                    distances[currentIndex] + edge->distance;
                if (candidate < distances[neighborIndex])
                {
                    distances[neighborIndex] = candidate;
                    previous[neighborIndex] = currentIndex;
                }
            }
            edge = edge->next;
        }
    }
    return true;
}

void CampusGraph::displayRoute(PathStack& route) const
{
    cout << "Route: ";
    while (!route.empty())
    {
        int id = route.pop();
        int index = findLocationIndex(id);
        if (index != -1)
        {
            cout << locations[index].name;
        }
        if (!route.empty())
        {
            cout << " -> ";
        }
    }

    cout << '\n';
}

void CampusGraph::displayNavigation(
    const int routeIndices[],
    int routeLength,
    int shortestDistance) const
{
    if (routeLength <= 0)
    {
        cout << "No route steps to display.\n";
        return;
    }

    cout << "Navigation directions:\n";
    cout << "Start at " << locations[routeIndices[0]].name << ".\n";

    for (int i = 1; i < routeLength; i++)
    {
        CompassDirection previousDirection = CompassDirection::Unknown;
        CompassDirection nextDirection = CompassDirection::Unknown;

        const EdgeNode* edge =
            adjacency[routeIndices[i - 1]].getHead();
        while (edge != nullptr)
        {
            if (edge->destId == locations[routeIndices[i]].id)
            {
                nextDirection = edge->direction;
                break;
            }
            edge = edge->next;
        }

        if (i > 1)
        {
            edge = adjacency[routeIndices[i - 2]].getHead();
            while (edge != nullptr)
            {
                if (edge->destId == locations[routeIndices[i - 1]].id)
                {
                    previousDirection = edge->direction;
                    break;
                }
                edge = edge->next;
            }
        }

        cout << "  " << i << ". ";
        if (nextDirection == CompassDirection::Unknown ||
            (i > 1 && previousDirection == CompassDirection::Unknown))
        {
            cout << "Continue to " << locations[routeIndices[i]].name
                 << " (heading not configured).\n";
            continue;
        }

        if (i == 1)
        {
            cout << "Go straight to " << locations[routeIndices[i]].name
                 << ".\n";
            continue;
        }

        int turn = (
            static_cast<int>(nextDirection) -
            static_cast<int>(previousDirection) + 4
        ) % 4;
        if (turn == 0)
        {
            cout << "Go straight to ";
        }
        else if (turn == 1)
        {
            cout << "Turn right to ";
        }
        else if (turn == 3)
        {
            cout << "Turn left to ";
        }
        else
        {
            cout << "Turn around to ";
        }
        cout << locations[routeIndices[i]].name << ".\n";
    }

    cout << "Destination: "
         << locations[routeIndices[routeLength - 1]].name << '\n';
}

bool CampusGraph::findCycleFrom(
    int currentIndex,
    int startIndex,
    int maxEdges,
    int pathLength,
    bool visited[],
    int path[]
) const
{
    const EdgeNode* edge = adjacency[currentIndex].getHead();
    while (edge != nullptr)
    {
        int neighborIndex = findLocationIndex(edge->destId);
        if (neighborIndex == startIndex && pathLength >= 3)
        {
            return true;
        }

        if (neighborIndex != -1 &&
            !visited[neighborIndex] &&
            pathLength < maxEdges)
        {
            visited[neighborIndex] = true;
            path[pathLength] = neighborIndex;
            if (findCycleFrom(
                    neighborIndex,
                    startIndex,
                    maxEdges,
                    pathLength + 1,
                    visited,
                    path))
            {
                return true;
            }
            visited[neighborIndex] = false;
        }
        edge = edge->next;
    }

    return false;
}

bool CampusGraph::displayCyclicPath(int startId) const
{
    int startIndex = findLocationIndex(startId);
    if (startIndex == -1)
    {
        cout << "\nStarting location not found.\n";
        return false;
    }

    int path[MAX_LOCATIONS];
    for (int maxEdges = 3; maxEdges <= locationCount; maxEdges++)
    {
        bool visited[MAX_LOCATIONS] = {};
        visited[startIndex] = true;
        path[0] = startIndex;
        if (findCycleFrom(
                startIndex,
                startIndex,
                maxEdges,
                1,
                visited,
                path))
        {
            cout << "\n========== CYCLIC PATH ==========\n";
            for (int i = 0; i < maxEdges; i++)
            {
                cout << locations[path[i]].name << " -> ";
            }
            cout << locations[startIndex].name << '\n';
            return true;
        }
    }

    cout << "\nNo cyclic path starting at "
         << locations[startIndex].name << " exists.\n";
    return false;
}

bool CampusGraph::loadFromFile(string filename)
{
    ifstream file(filename.c_str());
    if (!file.is_open())
    {
        return false;
    }

    clearGraph();
    string line;
    string section;

    while (getline(file, line))
    {
        if (line.empty())
        {
            continue;
        }

        if (line == "LOCATIONS")
        {
            section = "LOCATIONS";
            continue;
        }
        if (line == "EDGES")
        {
            section = "EDGES";
            continue;
        }
        if (line.compare(0, 7, "NEXTID|") == 0)
        {
            try
            {
                int savedNextId = stoi(line.substr(7));
                if (savedNextId > nextId)
                {
                    nextId = savedNextId;
                }
            }
            catch (const exception&)
            {
            }
            continue;
        }

        stringstream input(line);
        string first;
        string second;
        try
        {
            if (section == "LOCATIONS")
            {
                if (getline(input, first, '|') && getline(input, second))
                {
                    addLocationWithId(stoi(first), second);
                }
            }
            else if (section == "EDGES")
            {
                if (getline(input, first, '|') && getline(input, second, '|'))
                {
                    string directionText;
                    CompassDirection direction = CompassDirection::Unknown;
                    int distance = 1;
                    if (!getline(input, directionText, '|'))
                    {
                        directionText = second;
                        second.clear();
                    }
                    else
                    {
                        if (!parseCompassDirection(directionText, direction))
                        {
                            cout << "Ignoring invalid heading on edge "
                                 << first << "|" << second << ".\n";
                            continue;
                        }
                        string distanceText;
                        if (getline(input, distanceText, '|'))
                        {
                            distance = stoi(distanceText);
                        }
                    }
                    int from = stoi(first);
                    int to = second.empty() ? stoi(directionText) : stoi(second);
                    if (distance < 0 ||
                        !addOrUpdatePath(from, to, direction, distance))
                    {
                        cout << "Ignoring invalid edge "
                             << first << "|" << (second.empty() ? directionText : second)
                             << ".\n";
                    }
                }
            }
        }
        catch (const exception&)
        {
        }
    }

    return true;
}

bool CampusGraph::saveToFile(string filename)
{
    ofstream file(filename.c_str());
    if (!file.is_open())
    {
        return false;
    }

    file << "LOCATIONS\n";
    for (int i = 0; i < locationCount; i++)
    {
        file << locations[i].id << "|" << locations[i].name << '\n';
    }

    file << "\nEDGES\n";
    for (int i = 0; i < locationCount; i++)
    {
        EdgeNode* edge = adjacency[i].getHead();
        while (edge != nullptr)
        {
            if (locations[i].id < edge->destId)
            {
                file << locations[i].id << "|" << edge->destId
                     << "|" << compassDirectionName(edge->direction)
                     << "|" << edge->distance
                     << '\n';
            }
            edge = edge->next;
        }
    }

    file << "\nNEXTID|" << nextId << '\n';
    return file.good();
}

int CampusGraph::getNextId() const
{
    return nextId;
}

int CampusGraph::getLocationCount() const
{
    return locationCount;
}
