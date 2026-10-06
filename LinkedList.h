#ifndef LINKEDLIST_H
#define LINKEDLIST_H

#include <string>

enum class CompassDirection
{
    Unknown = -1,
    North = 0,
    East = 1,
    South = 2,
    West = 3
};

bool parseCompassDirection(
    const std::string& value,
    CompassDirection& direction);
const char* compassDirectionName(CompassDirection direction);
CompassDirection oppositeDirection(CompassDirection direction);

//linked list used as adjacency list of graph edges
struct EdgeNode
{
    int destId;
    CompassDirection direction;
    int distance;
    EdgeNode* next;

    EdgeNode(int destId, CompassDirection direction, int distance);
};

class AdjList
{
private:
    EdgeNode* head;

public:
    AdjList();
    ~AdjList();

    void insert(int destId, CompassDirection direction, int distance);
    bool remove(int destId);

    EdgeNode* find(int destId);

    EdgeNode* getHead();
    const EdgeNode* getHead() const;

    void moveFrom(AdjList& other);

    void clear();
};

#endif