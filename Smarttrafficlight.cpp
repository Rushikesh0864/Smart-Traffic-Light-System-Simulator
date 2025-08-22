#include <iostream>
#include <vector>
#include <thread>
#include <chrono>
#include <random>
#include <atomic>
#include <fstream>

using namespace std;

// Color codes for console output
#define RED     "\033[1;31m"
#define GREEN   "\033[1;32m"
#define YELLOW  "\033[1;33m"
#define RESET   "\033[0m"

// Lane structure representing each traffic lane
struct Lane {
    int id;             // Lane ID
    int VC;             // Vehicle capacity placeholder (unused directly)
    int vehicleCount;   // Number of vehicles currently in lane
    bool hasVIP;        // VIP vehicle present
    bool isGreen;       // Green light status
    bool isYellow;      // Yellow light status
    int waitTime;       // Accumulated wait time
    int incidentType;   // Incident type (0 = none, 1 = accident, etc.)
    bool hasFiretruck;  // Firetruck present
    bool hasAmbulance;  // Ambulance present

    // Constructor initializes lane with default values
    Lane(int id) : id(id), vehicleCount(0), hasVIP(false), hasFiretruck(false),
                   hasAmbulance(false), incidentType(0), isGreen(false), isYellow(false), waitTime(0) {}
};

// Weather conditions
vector<string> weather = { "Sunny", "Cloudy", "Windy", "Rainy", "Stormy", "Snowy", "Foggy" };
// Incident types (index matches condition ID)
string Incident[4] = {"   No   ", "Accident", "Road Work", "Police"};

// Global flags and log file
atomic<bool> running(true);       // Used to stop simulation when 'q' is pressed
ofstream logFile("traffic_log.txt"); // Log file for saving simulation output

// Select a random weather condition for the simulation
string today_weather(){
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dist(0, weather.size() - 1);
    return weather[dist(gen)];
}

// Ask user for incident condition for each lane
int Conditions(){
    int ch;
    cout << "Is there any Incident at this lane:\n";
    cout << "1. Accident\n2. Road Work\n3. Police Chase\n4. No\n";
    cin >> ch;
    if (ch >= 1 && ch <= 3) return ch;
    return 0;
}

// Display lane status in tabular form on console
void displayStatus(const vector<Lane>& lanes, int priorityLane, const string& reason, const string& weatherToday) {
#if defined(_WIN32) || defined(_WIN64)
    system("CLS");   // Clear screen on Windows
#else
    system("clear"); // Clear screen on Linux/Mac
#endif

    // Weather intensity scale (based on index)
    int intensity = 0;
    for(int i = 0; i < weather.size(); i++) {
        if(weatherToday == weather[i]) {
            intensity = i + 1;
            break;
        }
    }

    // Print table header
    cout << "Today's Weather: " << weatherToday << "\n";
    cout << "(Press 'q' then ENTER to end simulation)\n\n";
    cout << "+--------+------------+-------------+-----------+----------------------+----------+-----------+\n";
    cout << "| Lane   | Vehicles   | Light       | Intensity | Priority Reason      | WaitTime |  Incident |\n";
    cout << "+--------+------------+-------------+-----------+----------------------+----------+-----------+\n";

    // Print row for each lane
    for (const auto& lane : lanes) {
        cout << "| Lane " << lane.id + 1 << " |     " << lane.vehicleCount << "     |  ";

        if (lane.isGreen)
            cout << GREEN << "GREEN      " << RESET;
        else if (lane.isYellow)
            cout << YELLOW << "YELLOW     " << RESET;
        else
            cout << RED << "RED [WALK] " << RESET;

        cout << " |     " << intensity << "     | ";

        // Show priority reason
        if (lane.id == priorityLane)
            cout << reason;
        else if (lane.hasAmbulance)       cout << "      Ambulance      ";
        else if (lane.hasVIP)             cout << "         VIP         ";
        else if (lane.hasFiretruck)       cout << "     Firetruck       ";
        else                              cout << "                    ";

        // Print wait time and incident type
        cout << " |    " << lane.waitTime << "s    | " << Incident[lane.incidentType] << " |\n";
    }

    cout << "+--------+------------+-------------+-----------+----------------------+----------+-----------+\n";
}

// Select the lane with highest priority based on rules
int selectPriorityLane(vector<Lane>& lanes, string &reason) {
    // Ambulance priority
    for (const auto& lane : lanes) {
        if (lane.hasAmbulance) {
            reason = "      Ambulance     ";
            return lane.id;
        }
    }
    // Firetruck priority
    for (const auto& lane : lanes) {
        if (lane.hasFiretruck) {
            reason = "      Firetruck     ";
            return lane.id;
        }
    }
    // VIP priority
    for (const auto& lane : lanes) {
        if (lane.hasVIP) {
            reason = "         VIP        ";
            return lane.id;
        }
    }
    // Otherwise, select lane with maximum vehicles
    int maxLane = 0, maxCount = lanes[0].vehicleCount;
    for (const auto& lane : lanes) {
        if (lane.vehicleCount > maxCount) {
            maxCount = lane.vehicleCount;
            maxLane = lane.id;
        }
    }
    reason = "    Most vehicles   ";
    return maxLane;
}

// Separate thread to listen for 'q' input and stop simulation
void inputListener() {
    char c;
    while (running) {
        cin >> c;
        if (c == 'q' || c == 'Q') {
            running = false;
            break;
        }
    }
}

int main() {
    int numLanes;
    cout << "Enter number of traffic lanes: ";
    cin >> numLanes;

    vector<Lane> lanes;
    for (int i = 0; i < numLanes; ++i) {
        Lane lane(i);
        char vip, amb;

        // Ask if lane has VIP
        cout << "Is there a [ VIP ] at Lane " << i + 1 << "? (y/n): ";
        cin >> vip;
        lane.hasVIP = (vip == 'y' || vip == 'Y');

        // If no VIP, generate random vehicle count
        if (!lane.hasVIP) {
            lane.VC = lane.vehicleCount = rand() % 30 + 5;
        }

        // Check for ambulance
        cout << "Ambulance at Lane " << i + 1 << "? (y/n): ";
        cin >> amb;
        lane.hasAmbulance = (amb == 'y' || amb == 'Y');

        // Check for firetruck
        cout << "Firetruck at Lane " << i + 1 << "? (y/n): ";
        cin >> amb;
        lane.hasFiretruck = (amb == 'y' || amb == 'Y');

        // Incident input
        lane.incidentType = Conditions();
        lanes.push_back(lane);
    }

    // Generate today's weather
    string weatherToday = today_weather();

    // Start input listener thread
    thread listener(inputListener);

    // Random generator for vehicle arrival
    random_device rd;                
    mt19937 gen(rd());               
    uniform_int_distribution<> vehGen(0, 5); // Vehicles arrive randomly (0-5)

    while (running) {
        // Log current lane states before update
        logFile << "=== BEFORE UPDATE ===\n";
        for (const auto& lane : lanes) {
            logFile << "Lane " << lane.id+1 << ": Vehicles=" << lane.vehicleCount 
                    << ", WaitTime=" << lane.waitTime << "\n";
        }

        // Update vehicles & wait times for each lane
        for (auto& lane : lanes) {
            // Random arrival of new vehicles
            int randomArrival = vehGen(gen);
            lane.vehicleCount += randomArrival;

            if (lane.isGreen) {
                // Vehicles pass through green light
                int passed = min(5 + rand() % 3, lane.vehicleCount);
                lane.vehicleCount -= passed;
                lane.waitTime = 0;
            } else if (lane.incidentType > 0) {
                // Incident delays vehicles more
                lane.waitTime += 7;
            } else {
                // Normal wait increment
                lane.waitTime += 5;
            }
        }

        // Reset traffic light colors
        for (auto& lane : lanes) {
            lane.isGreen = false;
            lane.isYellow = false;
        }

        // Select lane with priority
        string reason;
        int priorityLane = selectPriorityLane(lanes, reason);

        // Show yellow before green
        lanes[priorityLane].isYellow = true;
        displayStatus(lanes, priorityLane, reason, weatherToday);
        this_thread::sleep_for(chrono::seconds(2));

        // Turn green
        lanes[priorityLane].isYellow = false;
        lanes[priorityLane].isGreen = true;

        displayStatus(lanes, priorityLane, reason, weatherToday);

        // Log after update
        logFile << "=== AFTER UPDATE (Priority Lane " << priorityLane+1 
                << " Reason: " << reason << ") ===\n";
        for (const auto& lane : lanes) {
            logFile << "Lane " << lane.id+1 << ": Vehicles=" << lane.vehicleCount 
                    << ", WaitTime=" << lane.waitTime << "\n";
        }
        logFile << "--------------------------\n";

        // Reset priority flags so they don’t persist forever
        lanes[priorityLane].hasVIP = false;
        lanes[priorityLane].hasAmbulance = false;
        lanes[priorityLane].hasFiretruck = false;

        this_thread::sleep_for(chrono::seconds(4));
    }

    // Stop listener thread and close log file
    listener.join();
    logFile.close();
    cout << "\nSimulation ended. Log saved to traffic_log.txt\n";
    return 0;
}
