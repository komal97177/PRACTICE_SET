#include <iostream>
#include <vector>
#include <list>
#include <queue>
#include <unordered_map>
#include <unordered_set>
#include <chrono>
#include <random>
#include <algorithm>
#include <cmath>
#include <fstream>
#include <sstream>
#include <limits>
#include <functional>
#include <memory>
#include <iomanip>
#include <climits>
#include <cfloat>

// The include for Windows-specific functions, like Sleep()
#include <windows.h>

using namespace std;

// Constants and Configuration
const int MAX_NODES = 1000;
const int MAX_VEHICLES = 5000;
const int UPDATE_INTERVAL_MS = 100;
const int SIMULATION_DURATION_MS = 30000;
const double CONGESTION_FACTOR = 0.7;
const int ROAD_CAPACITY = 50;

// Forward declarations to allow class cross-referencing
class Node;
class Edge;
class Vehicle;
class Graph;
class TrafficSimulator;
class NavigationEngine;
class RealTimeUpdater;

// Node class representing intersections
class Node {
public:
    int id;
    double x, y; // Coordinates for visualization and heuristic calculation
    string name;
    
    Node(int id, double x, double y, const string& name = "") 
        : id(id), x(x), y(y), name(name) {}
    
    void display() const {
        if (name.empty()) {
            cout << "Node " << id << " (" << x << ", " << y << ")";
        } else {
            cout << name << " (" << x << ", " << y << ")";
        }
    }
};

// Edge class representing roads between intersections
class Edge {
public:
    int id;
    Node* from;
    Node* to;
    double baseTravelTime; // Travel time under ideal conditions
    double currentTravelTime; // Dynamic travel time based on congestion
    int vehicleCount; // Number of vehicles currently on this road
    int capacity; // Maximum vehicles this road can handle efficiently
    string name;
    
    Edge(int id, Node* from, Node* to, double baseTime, 
         int capacity = ROAD_CAPACITY, const string& name = "")
        : id(id), from(from), to(to), baseTravelTime(baseTime), 
          currentTravelTime(baseTime), vehicleCount(0), capacity(capacity), name(name) {}
    
    // Updates the current travel time based on the number of vehicles (congestion)
    void updateCongestion() {
        double congestionLevel = max(0.0, static_cast<double>(vehicleCount) / capacity - 0.5);
        currentTravelTime = baseTravelTime * (1 + CONGESTION_FACTOR * congestionLevel);
    }
    
    void display() const {
        if (name.empty()) {
            cout << "Edge " << id << " from ";
            from->display();
            cout << " to ";
            to->display();
        } else {
            cout << name << " (";
            from->display();
            cout << " -> ";
            to->display();
            cout << ")";
        }
        cout << " - Travel time: " << fixed << setprecision(2) << currentTravelTime << " (base: " << baseTravelTime 
             << "), Vehicles: " << vehicleCount << "/" << capacity;
    }
};

// Vehicle class representing cars in the simulation
class Vehicle {
public:
    int id;
    Node* currentPosition;
    Node* destination;
    vector<int> route; // Planned route as edge IDs
    int currentRouteIndex;
    double progress; // Progress along the current edge (0 to 1)
    double speed; // Current speed
    
    Vehicle(int id, Node* start, Node* dest)
        : id(id), currentPosition(start), destination(dest), 
          currentRouteIndex(-1), progress(0), speed(1.0) {}
    
    // Assigns a new route to the vehicle
    void assignRoute(const vector<int>& newRoute) {
        route = newRoute;
        currentRouteIndex = 0;
        progress = 0;
    }
    
    void display() const {
        cout << "Vehicle " << id << " at ";
        currentPosition->display();
        cout << " -> ";
        destination->display();
        cout << " on route segment " << currentRouteIndex << "/" << route.size();
    }
};

// Graph class representing the city map
class Graph {
private:
    vector<Node*> nodes;
    vector<Edge*> edges;
    // Adjacency list storing node_id -> list of (neighbor_id, edge_weight)
    unordered_map<int, list<pair<int, double>>> adjacencyList; 
    
public:
    // Destructor to free memory
    ~Graph() {
        for (auto node : nodes) delete node;
        for (auto edge : edges) delete edge;
    }
    
    Node* addNode(int id, double x, double y, const string& name = "") {
        if (getNode(id)) {
            cout << "Node with ID " << id << " already exists!" << endl;
            return nullptr;
        }
        Node* newNode = new Node(id, x, y, name);
        nodes.push_back(newNode);
        return newNode;
    }
    
    Edge* addEdge(int id, Node* from, Node* to, double baseTime, 
                  int capacity = ROAD_CAPACITY, const string& name = "") {
        if (getEdgeById(id)) {
            cout << "Edge with ID " << id << " already exists!" << endl;
            return nullptr;
        }
        Edge* newEdge = new Edge(id, from, to, baseTime, capacity, name);
        edges.push_back(newEdge);
        
        // Update adjacency list with the initial base travel time
        adjacencyList[from->id].push_back({to->id, baseTime});
        
        return newEdge;
    }
    
    Node* getNode(int id) const {
        for (auto node : nodes) {
            if (node->id == id) return node;
        }
        return nullptr;
    }
    
    Node* getNodeByName(const string& name) const {
        for (auto node : nodes) {
            if (node->name == name) return node;
        }
        return nullptr;
    }
    
    Edge* getEdge(int fromId, int toId) const {
        for (auto edge : edges) {
            if (edge->from->id == fromId && edge->to->id == toId) {
                return edge;
            }
        }
        return nullptr;
    }
    
    Edge* getEdgeById(int edgeId) const {
        for (auto edge : edges) {
            if (edge->id == edgeId) {
                return edge;
            }
        }
        return nullptr;
    }
    
    const vector<Node*>& getNodes() const { return nodes; }
    const vector<Edge*>& getEdges() const { return edges; }
    
    const list<pair<int, double>>& getNeighbors(int nodeId) const {
        static list<pair<int, double>> emptyList;
        auto it = adjacencyList.find(nodeId);
        return it != adjacencyList.end() ? it->second : emptyList;
    }
    
    // Dynamically updates the edge weight in the adjacency list
    void updateEdgeWeight(int fromId, int toId, double newWeight) {
        auto& neighbors = adjacencyList[fromId];
        for (auto& neighbor : neighbors) {
            if (neighbor.first == toId) {
                neighbor.second = newWeight;
                break;
            }
        }
    }
    
    void displayNodes() const {
        cout << "\n=== NODES ===" << endl;
        for (const auto& node : nodes) {
            cout << "ID: " << node->id;
            if (!node->name.empty()) cout << ", Name: " << node->name;
            cout << ", Position: (" << node->x << ", " << node->y << ")" << endl;
        }
    }
    
    void displayEdges() const {
        cout << "\n=== EDGES ===" << endl;
        for (const auto& edge : edges) {
            cout << "ID: " << edge->id;
            if (!edge->name.empty()) cout << ", Name: " << edge->name;
            cout << ", From: " << edge->from->id << ", To: " << edge->to->id;
            cout << ", Base Time: " << edge->baseTravelTime;
            cout << ", Capacity: " << edge->capacity << endl;
        }
    }
};

// Navigation Engine with advanced pathfinding algorithms
class NavigationEngine {
private:
    const Graph& graph;
    
    // Heuristic function for A* algorithm (Euclidean distance)
    double heuristic(Node* a, Node* b) const {
        double dx = a->x - b->x;
        double dy = a->y - b->y;
        return sqrt(dx*dx + dy*dy);
    }
    
    // Reconstructs the path from the 'cameFrom' map
    vector<int> reconstructPath(const unordered_map<int, int>& cameFrom, int startId, int endId) const {
        vector<int> path;
        if (cameFrom.find(endId) == cameFrom.end()) {
            return path; // No path found
        }
        
        for (int at = endId; at != startId; at = cameFrom.at(at)) {
            Edge* edge = graph.getEdge(cameFrom.at(at), at);
            if (edge) {
                path.push_back(edge->id);
            } else {
                return {}; // Should not happen if the graph is consistent
            }
        }
        reverse(path.begin(), path.end());
        return path;
    }
    
public:
    NavigationEngine(const Graph& g) : graph(g) {}
    
    // A* algorithm for finding the most optimal path considering a heuristic
    vector<int> findPathAStar(Node* start, Node* end) {
        unordered_map<int, double> gScore; // Cost from start to node
        unordered_map<int, double> fScore; // Estimated total cost (gScore + heuristic)
        unordered_map<int, int> cameFrom; // The node that leads to the current node
        
        // Custom comparator for the priority queue to prioritize lower fScore
        auto nodeCmp = [&](const pair<double, int>& a, const pair<double, int>& b) { 
            return a.first > b.first; 
        };
        // The priority queue holds pairs of (fScore, node_id)
        priority_queue<pair<double, int>, 
                       vector<pair<double, int>>, 
                       decltype(nodeCmp)> openSet(nodeCmp);
        unordered_set<int> closedSet; // Nodes already evaluated
        
        // Initialize scores
        for (auto node : graph.getNodes()) {
            gScore[node->id] = numeric_limits<double>::infinity();
            fScore[node->id] = numeric_limits<double>::infinity();
        }
        
        gScore[start->id] = 0;
        fScore[start->id] = heuristic(start, end);
        openSet.push({fScore[start->id], start->id});
        
        while (!openSet.empty()) {
            int current = openSet.top().second;
            openSet.pop();
            
            if (current == end->id) {
                return reconstructPath(cameFrom, start->id, end->id);
            }
            
            if (closedSet.find(current) != closedSet.end()) continue;
            closedSet.insert(current);
            
            for (const auto& neighbor : graph.getNeighbors(current)) {
                int neighborId = neighbor.first;
                if (closedSet.find(neighborId) != closedSet.end()) continue;
                
                double tentativeGScore = gScore[current] + neighbor.second;
                
                if (tentativeGScore < gScore[neighborId]) {
                    cameFrom[neighborId] = current;
                    gScore[neighborId] = tentativeGScore;
                    fScore[neighborId] = gScore[neighborId] + heuristic(graph.getNode(neighborId), end);
                    openSet.push({fScore[neighborId], neighborId});
                }
            }
        }
        
        return {}; // No path found
    }
    
    // Find path with congestion avoidance (currently a placeholder for A*)
    vector<int> findCongestionAwarePath(Node* start, Node* end) {
        // In a more complex model, this would use a more sophisticated
        // algorithm that considers current congestion from the simulator.
        return findPathAStar(start, end);
    }
};

// Traffic simulator that generates vehicle movement data
class TrafficSimulator {
private:
    Graph& graph;
    NavigationEngine& navigator;
    vector<Vehicle> vehicles;
    mt19937 rng;
    
public:
    TrafficSimulator(Graph& g, NavigationEngine& nav, int vehicleCount = 1000)
        : graph(g), navigator(nav), rng(random_device{}()) {
        
        // Create vehicles with random start and end points
        const auto& nodes = graph.getNodes();
        if (nodes.size() < 2) return;
        
        uniform_int_distribution<size_t> dist(0, nodes.size() - 1);
        
        for (int i = 0; i < vehicleCount; i++) {
            size_t startIdx = dist(rng);
            size_t endIdx;
            do {
                endIdx = dist(rng);
            } while (endIdx == startIdx);
            
            vehicles.emplace_back(i, nodes[startIdx], nodes[endIdx]);
        }
    }
    
    // Update vehicle positions and generate updates for the graph
    void update(queue<pair<int, int>>& dataQueue) {
        for (auto& vehicle : vehicles) {
            if (vehicle.currentRouteIndex >= 0 && 
                vehicle.currentRouteIndex < vehicle.route.size()) {
                
                int edgeId = vehicle.route[vehicle.currentRouteIndex];
                Edge* edge = graph.getEdgeById(edgeId);
                
                if (edge) {
                    // Move vehicle along the edge
                    vehicle.progress += 0.01 * vehicle.speed;
                    
                    if (vehicle.progress >= 1.0) {
                        // Vehicle reached the end of the current edge
                        dataQueue.push({edgeId, -1}); // Vehicle leaving edge
                        
                        vehicle.currentRouteIndex++;
                        vehicle.progress = 0;
                        
                        if (vehicle.currentRouteIndex < vehicle.route.size()) {
                            // Entering new edge
                            int newEdgeId = vehicle.route[vehicle.currentRouteIndex];
                            dataQueue.push({newEdgeId, 1}); // Vehicle entering edge
                        } else {
                            // Vehicle reached destination
                            vehicle.currentPosition = vehicle.destination;
                        }
                    }
                }
            } else if (vehicle.currentPosition != vehicle.destination) {
                // Need to calculate a new route
                vector<int> route = navigator.findPathAStar(
                    vehicle.currentPosition, vehicle.destination);
                vehicle.assignRoute(route);
                
                if (!route.empty()) {
                    // Vehicle entering the first edge of the route
                    dataQueue.push({route[0], 1});
                }
            }
        }
    }

    const vector<Vehicle>& getVehicles() const {
        return vehicles;
    }
};

// Utility functions for user input
int getIntInput(const string& prompt, int min = INT_MIN, int max = INT_MAX) {
    int value;
    while (true) {
        cout << prompt;
        cin >> value;
        if (cin.fail() || value < min || value > max) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid input. Please enter an integer between " 
                     << min << " and " << max << "." << endl;
        } else {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            return value;
        }
    }
}

double getDoubleInput(const string& prompt, double min = -DBL_MAX, double max = DBL_MAX) {
    double value;
    while (true) {
        cout << prompt;
        cin >> value;
        if (cin.fail() || value < min || value > max) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid input. Please enter a number between " 
                     << min << " and " << max << "." << endl;
        } else {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            return value;
        }
    }
}

string getStringInput(const string& prompt) {
    string value;
    cout << prompt;
    getline(cin, value);
    return value;
}

// Main application class
class CitySync {
private:
    Graph graph;
    NavigationEngine navigator;
    
    // Creates a pre-defined grid-like city layout for demonstration
    void createDefaultCity() {
        cout << "Creating default city layout..." << endl;
        
        // Create a simple grid of nodes
        for (int i = 0; i < 5; i++) {
            for (int j = 0; j < 5; j++) {
                int id = i * 5 + j;
                string name = "Location_" + to_string(i) + "_" + to_string(j);
                graph.addNode(id, j * 100.0, i * 100.0, name);
            }
        }
        
        // Create horizontal edges
        int edgeId = 0;
        for (int i = 0; i < 5; i++) {
            for (int j = 0; j < 4; j++) {
                Node* from = graph.getNode(i * 5 + j);
                Node* to = graph.getNode(i * 5 + j + 1);
                string name = "Road_" + to_string(i) + "_" + to_string(j) + 
                                     "_to_" + to_string(i) + "_" + to_string(j+1);
                graph.addEdge(edgeId++, from, to, 1.0 + (i + j) % 3, ROAD_CAPACITY, name);
            }
        }
        
        // Create vertical edges
        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 5; j++) {
                Node* from = graph.getNode(i * 5 + j);
                Node* to = graph.getNode((i + 1) * 5 + j);
                string name = "Road_" + to_string(i) + "_" + to_string(j) + 
                                     "_to_" + to_string(i+1) + "_" + to_string(j);
                graph.addEdge(edgeId++, from, to, 1.0 + (i + j) % 3, ROAD_CAPACITY, name);
            }
        }
        
        cout << "Default city created with " << graph.getNodes().size() 
                 << " nodes and " << graph.getEdges().size() << " edges." << endl;
    }
    
    // Allows the user to create a custom city layout from scratch
    void createCustomCity() {
        cout << "\n=== Creating Custom City ===" << endl;
        
        int nodeCount = getIntInput("Enter number of nodes: ", 2, MAX_NODES);
        
        // Create nodes
        for (int i = 0; i < nodeCount; i++) {
            cout << "\n--- Node " << i << " ---" << endl;
            double x = getDoubleInput("Enter X coordinate: ");
            double y = getDoubleInput("Enter Y coordinate: ");
            string name = getStringInput("Enter name (optional): ");
            
            graph.addNode(i, x, y, name);
        }
        
        // Create edges
        int edgeCount = getIntInput("\nEnter number of edges: ", 1, nodeCount * nodeCount);
        
        for (int i = 0; i < edgeCount; i++) {
            cout << "\n--- Edge " << i << " ---" << endl;
            graph.displayNodes();
            
            int fromId = getIntInput("Enter FROM node ID: ", 0, nodeCount - 1);
            int toId = getIntInput("Enter TO node ID: ", 0, nodeCount - 1);
            
            if (fromId == toId) {
                cout << "Self-loop edges are not allowed. Skipping this edge." << endl;
                continue;
            }
            
            double baseTime = getDoubleInput("Enter base travel time: ", 0.1, 100.0);
            int capacity = getIntInput("Enter road capacity: ", 1, 1000);
            string name = getStringInput("Enter name (optional): ");
            
            Node* fromNode = graph.getNode(fromId);
            Node* toNode = graph.getNode(toId);
            
            if (fromNode && toNode) {
                graph.addEdge(i, fromNode, toNode, baseTime, capacity, name);
            } else {
                cout << "Invalid node IDs. Skipping this edge." << endl;
            }
        }
        
        cout << "Custom city created with " << graph.getNodes().size() 
                 << " nodes and " << graph.getEdges().size() << " edges." << endl;
    }
    
    // UI for finding a path between two user-specified nodes
    void findPathUI() {
        cout << "\n=== Path Finding ===" << endl;
        
        if (graph.getNodes().size() < 2) {
            cout << "Not enough nodes in the graph to find a path." << endl;
            return;
        }
        
        graph.displayNodes();
        
        int startId = getIntInput("Enter start node ID: ", 0, graph.getNodes().size() - 1);
        int endId = getIntInput("Enter end node ID: ", 0, graph.getNodes().size() - 1);
        
        if (startId == endId) {
            cout << "Start and end nodes are the same. No path needed." << endl;
            return;
        }
        
        Node* start = graph.getNode(startId);
        Node* end = graph.getNode(endId);
        
        if (!start || !end) {
            cout << "Invalid node IDs." << endl;
            return;
        }
        
        cout << "\nFinding path from ";
        start->display();
        cout << " to ";
        end->display();
        cout << "..." << endl;
        
        auto path = navigator.findPathAStar(start, end);
        
        if (path.empty()) {
            cout << "No path found!" << endl;
        } else {
            cout << "Path found with " << path.size() << " edges:" << endl;
            
            // Calculate total travel time
            double totalTime = 0;
            for (int edgeId : path) {
                Edge* edge = graph.getEdgeById(edgeId);
                if (edge) {
                    cout << "  ";
                    edge->display();
                    cout << endl;
                    totalTime += edge->currentTravelTime;
                }
            }
            cout << "Total estimated travel time: " << fixed << setprecision(2) << totalTime << " units" << endl;
        }
    }
    
    // Displays the current congestion levels on the city's roads
    void displayCurrentTraffic() {
        cout << "\n=== Current Traffic Conditions ===" << endl;
        
        const auto& edges = graph.getEdges();
        if (edges.empty()) {
            cout << "No edges in the graph." << endl;
            return;
        }
        
        // Sort edges by congestion level (vehicle count / capacity)
        vector<Edge*> sortedEdges(edges.begin(), edges.end());
        sort(sortedEdges.begin(), sortedEdges.end(), [](Edge* a, Edge* b) {
            double congestionA = static_cast<double>(a->vehicleCount) / a->capacity;
            double congestionB = static_cast<double>(b->vehicleCount) / b->capacity;
            return congestionA > congestionB;
        });
        
        cout << "Most congested roads:" << endl;
        for (int i = 0; i < min(5, static_cast<int>(sortedEdges.size())); i++) {
            Edge* edge = sortedEdges[i];
            double congestion = static_cast<double>(edge->vehicleCount) / edge->capacity;
            cout << "  ";
            edge->display();
            cout << ", Congestion: " << fixed << setprecision(2) 
                     << (congestion * 100) << "%" << endl;
        }
    }
    
    // UI to configure and run the traffic simulation
    void runSimulationUI() {
        cout << "\n=== Running Simulation ===" << endl;
        
        if (graph.getNodes().size() < 2 || graph.getEdges().size() < 1) {
            cout << "Not enough nodes or edges for simulation." << endl;
            return;
        }
        
        int vehicleCount = getIntInput("Enter number of vehicles: ", 1, MAX_VEHICLES);
        int duration = getIntInput("Enter simulation duration (seconds): ", 1, 300);
        
        cout << "Starting simulation with " << vehicleCount 
                 << " vehicles for " << duration << " seconds..." << endl;
        
        // Initialize the simulator
        TrafficSimulator simulator(graph, navigator, vehicleCount);
        queue<pair<int, int>> dataQueue;

        // Main simulation loop
        auto startTime = chrono::steady_clock::now();
        while (true) {
            auto currentTime = chrono::steady_clock::now();
            auto elapsedMs = chrono::duration_cast<chrono::milliseconds>(
                currentTime - startTime).count();
            
            if (elapsedMs >= duration * 1000) {
                break;
            }
            
            // Process updates from the queue
            while (!dataQueue.empty()) {
                pair<int, int> update = dataQueue.front();
                dataQueue.pop();
                
                int edgeId = update.first;
                int delta = update.second;
                
                Edge* edge = graph.getEdgeById(edgeId);
                if (edge) {
                    edge->vehicleCount += delta;
                    edge->updateCongestion();
                    
                    // Update the graph's adjacency list with new weight
                    graph.updateEdgeWeight(edge->from->id, edge->to->id, edge->currentTravelTime);
                }
            }
            
            // Run one step of the simulation
            simulator.update(dataQueue);
            
            // Display simulation status every 5 seconds
            if (static_cast<int>(elapsedMs) % 5000 == 0) {
                cout << "\nSimulation time: " << elapsedMs / 1000 << " seconds" << endl;
                displayCurrentTraffic();
                cout << endl;
            }
            
            Sleep(UPDATE_INTERVAL_MS);
        }
        
        cout << "Simulation completed." << endl;
    }
    
public:
    CitySync() : navigator(graph) {}
    
    void run() {
        cout << "=== CitySync Traffic Optimization System ===" << endl;
        
        while (true) {
            cout << "\n=== Main Menu ===" << endl;
            cout << "1. Create default city" << endl;
            cout << "2. Create custom city" << endl;
            cout << "3. Display city layout" << endl;
            cout << "4. Find path between nodes" << endl;
            cout << "5. Run traffic simulation" << endl;
            cout << "6. Display current traffic" << endl;
            cout << "7. Exit" << endl;
            
            int choice = getIntInput("Enter your choice: ", 1, 7);
            
            switch (choice) {
                case 1:
                    createDefaultCity();
                    break;
                case 2:
                    createCustomCity();
                    break;
                case 3:
                    graph.displayNodes();
                    graph.displayEdges();
                    break;
                case 4:
                    findPathUI();
                    break;
                case 5:
                    runSimulationUI();
                    break;
                case 6:
                    displayCurrentTraffic();
                    break;
                case 7:
                    cout << "Exiting CitySync. Goodbye!" << endl;
                    return;
            }
        }
    }
};

// Main function to start the application
int main() {
    CitySync citySync;
    citySync.run();
    return 0;
}
