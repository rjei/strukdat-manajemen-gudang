#ifndef WAREHOUSE_GRAPH_H
#define WAREHOUSE_GRAPH_H

#include <iostream>
#include <vector>
#include <list>
#include <map>
#include <string>
#include <queue>
#include <limits>
#include <algorithm> // Untuk std::transform
#include <cctype>    // Untuk ::toupper

struct WarehouseLocation {
    std::string id;
    std::string name;
    std::string address;
};

class WarehouseGraph {
private:
    // Adjacency list berbobot: ID tetangga dan jarak (int)
    std::map<std::string, std::list<std::pair<std::string, int>>> adjList;
    std::map<std::string, WarehouseLocation> warehouseData;

    std::string toUpperCase(std::string str);

public:
    WarehouseGraph();
    void addWarehouse(std::string id, std::string name, const std::string& address);

    // Menambah rute dengan parameter jarak
    void addRoute(std::string fromId, std::string toId, int distance);

    // Mencari rute terpendek menggunakan Dijkstra
    std::vector<std::string> getShortestPath(std::string startId, std::string endId, int& totalDistance);

    void displayNetwork();
    bool hasWarehouse(std::string id);
    
    // Menampilkan graf berbobot sebagai string dengan hasil analisis Dijkstra
    std::string getWeightedGraphString(std::string startId, std::string endId);
    
    // Mendapatkan daftar ID gudang
    std::vector<std::string> getWarehouseIds();
};

#endif
