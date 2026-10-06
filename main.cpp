#include <iostream>
#include <string>
#include <sstream>

#include "Graph.h"

using namespace std;

const string DATA_FILE = "campus.txt";
const string ROUTE_HISTORY_FILE = "route_history.txt";
const string ADMIN_PASSWORD = "admin123";

void clearScreen()
{
    cout << "\033[2J\033[H";
}

void pauseScreen()
{
    cout << "\nPress Enter to continue...";

    string line;
    getline(cin, line);
}

int getInteger(string message)
{
    while (true)
    {
        cout << message;

        string line;
        getline(cin, line);
        istringstream input(line);
        int value;

        if (input >> value)
        {
            input >> ws;

            if (input.eof())
            {
                return value;
            }
        }

        cout << "Invalid input. Enter a number.\n";
    }
}

int getNonnegativeInteger(string message)
{
    while (true)
    {
        int value = getInteger(message);
        if (value >= 0)
        {
            return value;
        }
        cout << "Enter a non-negative number.\n";
    }
}

string getLocationName()
{
    string name;

    cout << "Enter location name: ";

    getline(cin, name);

    return name;
}

CompassDirection getCompassDirection()
{
    while (true)
    {
        cout << "Enter heading from source to destination "
             << "(North, South, East, or West): ";

        string value;
        getline(cin, value);
        CompassDirection direction;
        if (parseCompassDirection(value, direction) &&
            direction != CompassDirection::Unknown)
        {
            return direction;
        }

        cout << "Invalid heading. Enter North, South, East, or West.\n";
    }
}

bool findLocation(
    CampusGraph &graph,
    int &id,
    string prompt = "Enter location name: ")
{
    string name;

    cout << prompt;

    getline(cin, name);

    if (!graph.findLocationByName(name, id))
    {
        cout << "Location not found.\n";

        return false;
    }

    return true;
}

void adminMenu(CampusGraph &graph)
{
    while (true)
    {
        clearScreen();
        cout << "\n";
        cout << "====================================\n";
        cout << "          ADMIN MENU\n";
        cout << "====================================\n";

        cout << "1. Add Location\n";
        cout << "2. Remove Location\n";
        cout << "3. Add / Update Path\n";
        cout << "4. Remove Path\n";
        cout << "5. Display Locations\n";
        cout << "6. Display Connections\n";
        cout << "7. Save Data\n";
        cout << "8. Rename Location\n";
        cout << "0. Back\n";

        int choice =
            getInteger("Enter choice: ");

        if (choice == 0)
        {
            clearScreen();
            return;
        }

        switch (choice)
        {
        case 1:
        {
            string name = getLocationName();

            if (graph.addLocation(name))
            {
                cout
                    << "Location added successfully.\n";
            }
            else
            {
                cout
                    << "Failed to add location.\n";
            }

            break;
        }

        case 2:
        {
            int id;

            graph.displayLocations();

            id = getInteger(
                "Enter location ID to remove: ");

            if (graph.removeLocation(id))
            {
                cout
                    << "Location removed successfully.\n";
            }
            else
            {
                cout
                    << "Location not found.\n";
            }

            break;
        }

        case 3:
        {
            graph.displayLocations();

            int from =
                getInteger("Enter source ID: ");

            int to =
                getInteger("Enter destination ID: ");
            CompassDirection direction = getCompassDirection();
            int distance = getNonnegativeInteger(
                "Enter path distance: ");

            if (
                graph.addOrUpdatePath(
                    from,
                    to,
                    direction,
                    distance))
            {
                cout << "Path, reverse heading, and distance added/updated "
                     << "successfully.\n";
            }
            else
            {
                cout
                    << "Failed to add path.\n";
            }

            break;
        }

        case 4:
        {
            graph.displayConnections();

            int from =
                getInteger("Enter source ID: ");

            int to =
                getInteger("Enter destination ID: ");

            if (graph.removePath(from, to))
            {
                cout
                    << "Path removed successfully.\n";
            }
            else
            {
                cout
                    << "Path not found.\n";
            }

            break;
        }

        case 5:
        {
            graph.displayLocations();

            break;
        }

        case 6:
        {
            graph.displayConnections();

            break;
        }

        case 7:
        {
            if (graph.saveToFile(DATA_FILE))
            {
                cout
                    << "Data saved to "
                    << DATA_FILE
                    << ".\n";
            }
            else
            {
                cout
                    << "Failed to save data.\n";
            }

            break;
        }

        case 8:
        {
            graph.displayLocations();
            string currentName;
            cout << "\nEnter current location name: ";
            getline(cin, currentName);

            string newName;
            cout << "Enter new location name: ";
            getline(cin, newName);

            if (graph.renameLocation(
                    currentName,
                    newName))
            {
                cout << "Location renamed successfully.\n";
            }
            else
            {
                cout << "Unable to rename location. Check the name and try again.\n";
            }

            break;
        }

        default:
        {
            cout << "Invalid choice.\n";
        }
        }

        pauseScreen();
        clearScreen();
    }
}

void userMenu(CampusGraph &graph)
{
    while (true)
    {
        clearScreen();
        cout << "\n";
        cout << "====================================\n";
        cout << "       CAMPUS NAVIGATION\n";
        cout << "====================================\n";
        cout << "1. Display Location\n";
        cout << "2. Search Location\n";
        cout << "3. Display Connections\n";
        cout << "4. Shortest Path (fewest connections)\n";
        cout << "5. Shortest Path by Distance\n";
        cout << "6. Display Connectivity\n";
        cout << "7. Show Cyclic Path\n";
        cout << "8. View Route History\n";
        cout << "9. Back\n";

        int choice = getInteger("Enter choice: ");
        if (choice == 9)
        {
            clearScreen();
            return;
        }

        switch (choice)
        {
        case 1:
        {
            graph.displayLocations(false);
            pauseScreen();
            break;
        }
        case 2:
        {
            int id;
            if (findLocation(graph, id))
            {
                Location location;
                if (graph.getLocationById(id, location))
                {
                    cout << "\nLocation Found\n"
                         << "Name: " << location.name << '\n';
                }
            }
            pauseScreen();
            break;
        }
        case 3:
            graph.displayConnections(false);
            pauseScreen();
            break;
        case 4:
        {
            graph.displayLocations(false);
            int source;
            int destination;
            bool sourceFound = findLocation(
                graph,
                source,
                "\nEnter starting location name: ");
            bool destinationFound = sourceFound && findLocation(
                graph,
                destination,
                "Enter destination location name: ");

            if (sourceFound && destinationFound)
            {
                graph.shortestPath(
                    source,
                    destination,
                    ROUTE_HISTORY_FILE);
            }

            pauseScreen();
            break;
        }
        case 5:
        {
            graph.displayLocations(false);
            int source;
            int destination;
            bool sourceFound = findLocation(
                graph,
                source,
                "\nEnter starting location name: ");
            bool destinationFound = sourceFound && findLocation(
                graph,
                destination,
                "Enter destination location name: ");

            if (sourceFound && destinationFound)
            {
                graph.shortestPathByDistance(
                    source,
                    destination,
                    ROUTE_HISTORY_FILE);
            }
            pauseScreen();
            break;
        }
        case 6:
        {
            graph.displayLocations(false);
            int source;
            int destination;
            bool sourceFound = findLocation(
                graph,
                source,
                "\nEnter starting location name: ");
            bool destinationFound = sourceFound && findLocation(
                graph,
                destination,
                "Enter destination location name: ");

            if (sourceFound && destinationFound)
            {
                graph.depthFirstConnectivity(
                    source,
                    destination,
                    ROUTE_HISTORY_FILE);
            }
            pauseScreen();
            break;
        }
        case 7:
        {
            graph.displayLocations(false);
            int start;
            if (findLocation(
                    graph,
                    start,
                    "\nEnter starting location name: "))
            {
                graph.displayCyclicPath(start);
            }
            pauseScreen();
            break;
        }
        case 8:
            graph.displayRouteHistory(ROUTE_HISTORY_FILE);
            pauseScreen();
            break;
        default:
            cout << "Invalid choice.\n";
            pauseScreen();
        }

        clearScreen();
    }
}

bool adminLogin()
{
    string password;

    cout << "Enter admin password: ";

    getline(cin, password);

    if (password == ADMIN_PASSWORD)
    {
        cout << "Login successful.\n";

        return true;
    }

    cout << "Incorrect password.\n";

    return false;
}

int main()
{
    CampusGraph graph;

    if (!graph.clearRouteHistory(ROUTE_HISTORY_FILE))
    {
        cout << "Unable to clear route history file: "
             << ROUTE_HISTORY_FILE << ".\n";
        return 1;
    }

    clearScreen();
    cout << "========================================\n";
    cout << "       KUET CAMPUS NAVIGATION SYSTEM\n";
    cout << "========================================\n";

    if (graph.loadFromFile(DATA_FILE))
    {
        cout
            << "\nCampus data loaded from "
            << DATA_FILE
            << ".\n";

        cout
            << "Locations loaded: "
            << graph.getLocationCount()
            << '\n';
    }
    else
    {
        cout
            << "\n"
            << DATA_FILE
            << " not found.\n";

        cout
            << "Starting with an EMPTY campus graph.\n";

        cout
            << "Add your data to "
            << DATA_FILE
            << ".\n";
    }

    while (true)
    {
        clearScreen();
        cout << "\n";
        cout << "========================================\n";
        cout << "              MAIN MENU\n";
        cout << "========================================\n";

        cout << "1. User\n";
        cout << "2. Admin\n";
        cout << "0. Exit\n";

        int choice =
            getInteger("Enter choice: ");

        if (choice == 0)
        {
            graph.saveToFile(DATA_FILE);

            cout
                << "\nData saved.\n";

            if (!graph.clearRouteHistory(ROUTE_HISTORY_FILE))
            {
                cout << "Unable to clear route history file: "
                     << ROUTE_HISTORY_FILE << ".\n";
            }

            cout
                << "Thank you for using KUET Campus Navigation System.\n";

            break;
        }

        switch (choice)
        {
        case 1:
        {
            userMenu(graph);
            clearScreen();

            break;
        }

        case 2:
        {
            if (adminLogin())
            {
                adminMenu(graph);
            }
            else
            {
                pauseScreen();
            }

            clearScreen();

            break;
        }

        default:
        {
            cout << "Invalid choice.\n";
            pauseScreen();
            clearScreen();
        }
        }
    }

    return 0;
}