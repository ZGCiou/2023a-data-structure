/*****************************************************************************
    Data Structure 2023 Autumn
    Assignment #6   : Graph & Shortest Path
    Author          : Zheng-Gang, Ciou (nycu311511022.ee11@nycu.edu.tw)
    File name       : 311511022.cpp

*****************************************************************************/
#include <iostream>
#include <fstream>
#include <sstream>
#include <list>
#include <map>
#include <vector>
#include <queue>
#include <algorithm>
#include <utility>
#include <set>
using namespace std;

//===============================================================
// DiGraph.h
//----- Definition of Directed Graph class
template <typename T>
class DiGraph {
 public:
    //----- Mamber Function
    // DiGraph();
    // ~DiGraph();
    void read(istream &in);
    DiGraph& insertArc(T start, T destination, int weight=1);
    pair<vector<T>, int> shortestPath(T start, T destination);
    void showAdjacency(T node);
    map<T, map<T, int> > getAdjacencyMap();

 private:
    //----- Nested Class for adjacency list
    struct NextVertex {
        T vertex;
        int cost;
        bool operator>(const NextVertex &next) const {
            return cost > next.cost;
        }
    };
    //----- Data Member
    map<T, map<T, int> > adjacencyMap;
};

//===============================================================
// DiGraph.cpp
//----- Definition of Directed Graph Function
// Constructor
// template <typename T>
// DiGraph<T>::DiGraph() {}

// Insert Arc
template <typename T>
DiGraph<T>& DiGraph<T>::insertArc(T start, T destination, int weight) {
    adjacencyMap[start][destination] = weight;
    return *this;
}
// Shortest Path
template <typename T>
pair<vector<T>, int> DiGraph<T>::shortestPath(T start, T destination) {
    map<T, bool> visitedLabel; // Record the visited vertex
    map<T, int> costLabel; // Distance of each vertex
    map<T, T> preLabel; // Predecessor of each vertex
    costLabel[start] = 0;
    T curVertex;
    priority_queue<NextVertex, vector<NextVertex>, greater<NextVertex> > vertexQueue;
    vertexQueue.push(NextVertex{start, 0});
    // Loop if haven't visited the destination vertex && queue is not empty.
    while (visitedLabel.find(destination) == visitedLabel.end() && !vertexQueue.empty()) {
        curVertex = vertexQueue.top().vertex;
        vertexQueue.pop();
        visitedLabel[curVertex] = true;
        for (auto it : adjacencyMap[curVertex]) {
            if (visitedLabel.find(it.first) != visitedLabel.end()) { // Only check vertices that haven't visited
                continue;
            }
            if (costLabel.find(it.first) == costLabel.end() || // No cost infomation
                costLabel[it.first] > (costLabel[curVertex] + it.second)) { // New cost is smaller
                    costLabel[it.first] = costLabel[curVertex] + it.second; // Update cost
                    preLabel[it.first] = curVertex;
                    vertexQueue.push(NextVertex{it.first, costLabel[it.first]});
            }
        }
    }

    vector<T> path;
    if (visitedLabel.find(destination) == visitedLabel.end()) {
        // cout << "Destination not reachable from start vertex" << endl;
        return make_pair(path, 286331153);
    } else {
        path.push_back(destination);
        while (preLabel[path.back()] != start) {
            path.push_back(preLabel[path.back()]);
        }
        path.push_back(start);
        reverse(path.begin(), path.end());
    }
    return make_pair(path, costLabel[destination]);
}
// Show Adjacency
template <typename T>
void DiGraph<T>::showAdjacency(T node) {
    cout << "Node " << node << endl;
    for (const auto &it : adjacencyMap[node]) {
        cout << it.first << ", weight: " << it.second << endl;
    }
}
// Get adjacencyMap
template <typename T>
map<T, map<T, int> > DiGraph<T>::getAdjacencyMap() {
    return adjacencyMap;
}

//===============================================================
// PackRoute.h
//----- Definition of PackRoute class
template<typename T>
class PackRoute : public DiGraph<T> {
 public:
    void updateIO();
    bool read(istream &in);
    pair<vector<T>, int> shortestIOPath();
    void showIO();
    void writeShortestIOPath(ostream &out);

 private:
    set<T> nodes;
    set<T> inputs;
    set<T> outputs;
};

//===============================================================
// PackRoute.cpp
//----- Definition of PackRoute class
// Update I/O ports
template<typename T>
void PackRoute<T>::updateIO() {
    map<T, map<T, int> > adjMap = DiGraph<T>::getAdjacencyMap();
    bool isInput;
    for (auto it : nodes) { // Check all nodes
        isInput = true;
        if (adjMap.find(it) == adjMap.end()) { // No arc to other node 
            outputs.insert(it);
        }
        for (auto it2 : adjMap) { // Check adjacency matrix, weather other nodes have a arc to this node.
            if (it2.second.find(it) != it2.second.end()) { // Find a arc to this node
                isInput = false;
                break;
            }
        }
        if (isInput) {
            inputs.insert(it);
        }
    }
}
// Read graph
template<typename T>
bool PackRoute<T>::read(istream &in) {
    istringstream iss;
    string line, symbol;
    int node1, node2, weight;
    while (!in.eof()) {
        getline(in, line);
        // cout << line << endl; // Debug
        iss.str(line);
        iss >> node1 >> symbol >> node2 >> symbol >> symbol;
        if (symbol != "weight") {
            cout << "Input Format Error." << endl;
            return false;
        }
        iss >> symbol >> weight;
        // Add arc to graph
        DiGraph<T>::insertArc(node1, node2, weight);
        nodes.insert(node1);
        nodes.insert(node2);
        // Clean string stream
        iss.str("");
        iss.clear();
        // cout << node1 << " " << node2 << " " << weight << endl; // Debug
    }
    updateIO();
    return true;
}
// Shortest input to output path
template<typename T>
pair<vector<T>, int> PackRoute<T>::shortestIOPath() { 
    int minCost = 286331153;
    pair<vector<T>, int> path_cost, path_cost_min;
    for (auto iport : inputs) {
        for (auto oport : outputs) {
            path_cost = DiGraph<T>::shortestPath(iport, oport);
            if (path_cost.second < minCost) {
                minCost = path_cost.second;
                path_cost_min = path_cost;
            }
        }
    }

    return path_cost_min;
}
// Show I/O ports
template<typename T>
void PackRoute<T>::showIO() {
    cout << "Input: ";
    for (auto it : inputs) {
        cout << it << " ";
    }
    cout << endl << "Output: ";
    for (auto it : outputs) {
        cout << it << " ";
    }
    cout << endl;
}
// Write Shortest input to output path
template<typename T>
void PackRoute<T>::writeShortestIOPath(ostream &out) {
    pair<vector<T>, int> path_cost = shortestIOPath();
    out << "Shortest Path Length:" << endl;
    out << path_cost.second << endl;
    out << "Shortest Path:" << endl;
    auto it = path_cost.first.begin();
    out << *(it++);
    for (;it!=path_cost.first.end(); it++) {
        out << " -> " << *it;
    }
}

//===============================================================
// main.cpp
int main(int argc, char **argv) {
    ifstream fin(argv[1]);
    ofstream fout(argv[2]);

    PackRoute<int> pktRouting;
    pktRouting.read(fin);
    pktRouting.writeShortestIOPath(fout);

    fin.close();
    fout.close();
    return 0;
}