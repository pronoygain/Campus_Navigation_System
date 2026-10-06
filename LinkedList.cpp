#include "LinkedList.h"

#include <cctype>

bool parseCompassDirection(
    const std::string& value,
    CompassDirection& direction)
{
    std::string normalized = value;
    for (std::string::size_type i = 0; i < normalized.size(); i++)
    {
        normalized[i] = static_cast<char>(
            std::toupper(static_cast<unsigned char>(normalized[i])));
    }

    if (normalized == "N" || normalized == "NORTH")
    {
        direction = CompassDirection::North;
    }
    else if (normalized == "E" || normalized == "EAST")
    {
        direction = CompassDirection::East;
    }
    else if (normalized == "S" || normalized == "SOUTH")
    {
        direction = CompassDirection::South;
    }
    else if (normalized == "W" || normalized == "WEST")
    {
        direction = CompassDirection::West;
    }
    else if (normalized == "UNKNOWN")
    {
        direction = CompassDirection::Unknown;
    }
    else
    {
        return false;
    }

    return true;
}

const char* compassDirectionName(CompassDirection direction)
{
    switch (direction)
    {
    case CompassDirection::North:
        return "North";
    case CompassDirection::East:
        return "East";
    case CompassDirection::South:
        return "South";
    case CompassDirection::West:
        return "West";
    default:
        return "Unknown";
    }
}

CompassDirection oppositeDirection(CompassDirection direction)
{
    switch (direction)
    {
    case CompassDirection::North:
        return CompassDirection::South;
    case CompassDirection::East:
        return CompassDirection::West;
    case CompassDirection::South:
        return CompassDirection::North;
    case CompassDirection::West:
        return CompassDirection::East;
    default:
        return CompassDirection::Unknown;
    }
}

EdgeNode::EdgeNode(int destId, CompassDirection direction, int distance)
{
    this->destId = destId;
    this->direction = direction;
    this->distance = distance;
    this->next = nullptr;
}

AdjList::AdjList()
{
    head = nullptr;
}

AdjList::~AdjList()
{
    clear();
}

void AdjList::insert(int destId, CompassDirection direction, int distance)
{
    EdgeNode* existing = find(destId);
    if (existing != nullptr)
    {
        existing->direction = direction;
        existing->distance = distance;
        return;
    }

    EdgeNode* newNode = new EdgeNode(destId, direction, distance);

    if (head == nullptr)
    {
        head = newNode;
        return;
    }

    EdgeNode* temp = head;

    while (temp->next != nullptr)
    {
        temp = temp->next;
    }

    temp->next = newNode;
}

bool AdjList::remove(int destId)
{
    if (head == nullptr)
    {
        return false;
    }

    if (head->destId == destId)
    {
        EdgeNode* temp = head;
        head = head->next;

        delete temp;

        return true;
    }

    EdgeNode* current = head;

    while (current->next != nullptr)
    {
        if (current->next->destId == destId)
        {
            EdgeNode* temp = current->next;

            current->next = current->next->next;

            delete temp;

            return true;
        }

        current = current->next;
    }

    return false;
}

EdgeNode* AdjList::find(int destId)
{
    EdgeNode* current = head;

    while (current != nullptr)
    {
        if (current->destId == destId)
        {
            return current;
        }

        current = current->next;
    }

    return nullptr;
}

EdgeNode* AdjList::getHead()
{
    return head;
}

const EdgeNode* AdjList::getHead() const
{
    return head;
}

void AdjList::moveFrom(AdjList& other)
{
    if (this == &other)
    {
        return;
    }

    clear();
    head = other.head;
    other.head = nullptr;
}

void AdjList::clear()
{
    EdgeNode* current = head;

    while (current != nullptr)
    {
        EdgeNode* temp = current;

        current = current->next;

        delete temp;
    }

    head = nullptr;
}