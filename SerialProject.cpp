#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <map>
#include <limits>
#include <list>
#include <queue>
#include <cstdlib>
#include <ctime>

using namespace std;

const int infinity = 9999; // Define a constant for infinity


struct Edge {
    string source;
    string target;
    int weight;
    string type;
};

struct Path {
    int total_cost;
    vector<int> node_sequence;
};

vector<Edge> readCSV(const string& filename) {
    vector<Edge> edges;
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Error: Unable to open file " << filename << endl;
        return edges;
    }

    string line;
    while (getline(file, line)) {
        stringstream ss(line);
        string source, target, type;
        int weight;
        if (getline(ss, source, ',') &&
            getline(ss, target, ',') &&
            (ss >> weight)) {
            getline(ss >> ws, type);
            if (!type.empty() && type[0] == ',') {
                type = type.substr(1);
            }
            edges.push_back({ source, target, weight, type });
        }
    }

    file.close();
    return edges;
}

map<string, int> createNodeIndex(const vector<Edge>& edges) {
    map<string, int> nodeIndex;
    int index = 0;
    for (const auto& edge : edges) {
        if (nodeIndex.find(edge.source) == nodeIndex.end()) {
            nodeIndex[edge.source] = index++;
        }
        if (nodeIndex.find(edge.target) == nodeIndex.end()) {
            nodeIndex[edge.target] = index++;
        }
    }
    return nodeIndex;
}

vector<vector<int>> createAdjMatrix(const vector<Edge>& edges, const map<string, int>& nodeIndex) 
{
    int size = nodeIndex.size();
    vector<vector<int>> adjMatrix(size, vector<int>(size, infinity)); // Use the infinity constant
    for (int i = 0; i < size; i++) {
        adjMatrix[i][i] = 0; // Distance from node to itself is 0
    }

    for (const auto& edge : edges) {
        int u = nodeIndex.at(edge.source);
        int v = nodeIndex.at(edge.target);
        adjMatrix[u][v] = edge.weight;
    }

    return adjMatrix;
}

// Pass the reverse map as an additional parameter
void findKShortestPaths(const vector<vector<int>>& adjMatrix, const map<int, string>& indexToNode, int start, int end, int k) {
    int n = adjMatrix.size();
    list<Path> paths;

    auto cmp = [](const Path& a, const Path& b) {
        return a.total_cost > b.total_cost;
    };
    priority_queue<Path, vector<Path>, decltype(cmp)> pq(cmp);

    pq.push({ 0, {start} });

    
        
            while (!pq.empty()) {
                Path current_path = pq.top();
                pq.pop();
                int last_node = current_path.node_sequence.back();

                if (last_node == end) {
                    #pragma omp critical
                    {
                        paths.push_back(current_path);
                    }
                    if (paths.size() == k) break;
                }

                if (paths.size() < k) {
                    for (int i = 0; i < n; i++) {
                        if (adjMatrix[last_node][i] != infinity && i != last_node) {
                            int new_cost = current_path.total_cost + adjMatrix[last_node][i];
                            vector<int> new_sequence = current_path.node_sequence;
                            new_sequence.push_back(i);
                            
                                pq.push({ new_cost, new_sequence });
                            
                        }
                    }
                }
            }
        
    

    // Print the collected paths using city names
    int count = 1;
    for (const auto& path : paths) {
        cout << count << "th shortest path from " << indexToNode.at(start) << " to " << indexToNode.at(end) << " is:" << endl;
        cout << "Cost: " << path.total_cost << ", Path: ";
        for (int node : path.node_sequence) {
            cout << indexToNode.at(node) << " -> ";
        }
        cout << endl;
        count++;
    }

    if (paths.size() < k) {
        cout << "Only " << paths.size() << " paths found out of " << k << " requested." << endl;
    }
}

int main() 
{
    srand(time(0));

    int *TotalscaterInfo, k = 3;
    // k is count of TOP SHORTEST PATHS NEEDED


    string filename = "doctorwho.csv";
    vector<Edge> edges = readCSV(filename);
    map<string, int> nodeIndex = createNodeIndex(edges);
    map<int, string> indexToNode;
    
    for (const auto& pair : nodeIndex) 
    {
        indexToNode[pair.second] = pair.first;
    }

    vector<vector<int>> adjMatrix = createAdjMatrix(edges, nodeIndex);
        //create 'paircount' pairs of nodes to calculate K Shortest Paths for

        int paircount = 10;
        TotalscaterInfo = new int[paircount*2];

        for (int i = 0; i < paircount*2; ++i)
        {
            int s1e1Node = rand() % nodeIndex.size();
            TotalscaterInfo[i] = s1e1Node;
        }
    

    int count=0;
    time_t startTime = time(nullptr);
    for(int i=0; i<10; i++)
    {
        cout << "Process 0 is finding shortest path from " << TotalscaterInfo[i*2] << " to " <<TotalscaterInfo[i*2+1] << endl; 
        findKShortestPaths(adjMatrix, indexToNode, TotalscaterInfo[i*2], TotalscaterInfo[i*2+1], k);
        cout << endl; 
    }
    time_t endTime = time(nullptr);
    time_t totaltime = endTime - startTime;
    cout << endl << " total time taken to execute: " << totaltime << endl;
    


    return 0;
}
