#include "warehouse_graph.h"

WarehouseGraph::WarehouseGraph() {}

std::string WarehouseGraph::toUpperCase(std::string str) {
    std::transform(str.begin(), str.end(), str.begin(), ::toupper);
    return str;
}

void WarehouseGraph::addWarehouse(std::string id, std::string name, const std::string& address) {
    id = toUpperCase(id);
    name = toUpperCase(name);
    
    if (warehouseData.find(id) == warehouseData.end()) {
        warehouseData[id] = {id, name, address};
        adjList[id] = std::list<std::pair<std::string, int>>();
    }
}

void WarehouseGraph::addRoute(std::string fromId, std::string toId, int distance) {
    fromId = toUpperCase(fromId);
    toId = toUpperCase(toId);
    
    if (hasWarehouse(fromId) && hasWarehouse(toId)) {
        adjList[fromId].push_back({toId, distance});
        adjList[toId].push_back({fromId, distance});
    }
}

bool WarehouseGraph::hasWarehouse(std::string id) {
    return warehouseData.find(toUpperCase(id)) != warehouseData.end();
}

std::vector<std::string> WarehouseGraph::getShortestPath(std::string startId, std::string endId, int& totalDistance) {
    startId = toUpperCase(startId);
    endId = toUpperCase(endId);

    if (!hasWarehouse(startId) || !hasWarehouse(endId)) {
        totalDistance = -1;
        return {};
    }

    std::map<std::string, int> distances;
    std::map<std::string, std::string> previous;
    // Priority queue untuk Dijkstra (jarak, simpul)
    std::priority_queue<std::pair<int, std::string>, std::vector<std::pair<int, std::string>>, std::greater<std::pair<int, std::string>>> pq;

    for (auto const& [id, _] : warehouseData) {
        distances[id] = std::numeric_limits<int>::max();
    }

    distances[startId] = 0;
    pq.push({0, startId});

    while (!pq.empty()) {
        std::string u = pq.top().second;
        int d = pq.top().first;
        pq.pop();

        if (d > distances[u]) continue;
        if (u == endId) break;

        for (auto const& edge : adjList[u]) {
            std::string v = edge.first;
            int weight = edge.second;

            if (distances[u] + weight < distances[v]) {
                distances[v] = distances[u] + weight;
                previous[v] = u;
                pq.push({distances[v], v});
            }
        }
    }

    std::vector<std::string> path;
    if (distances[endId] == std::numeric_limits<int>::max()) {
        totalDistance = -1;
        return path;
    }

    totalDistance = distances[endId];
    for (std::string at = endId; at != ""; at = previous[at]) {
        path.push_back(at);
        if (at == startId) break;
    }
    std::reverse(path.begin(), path.end());
    return path;
}

void WarehouseGraph::displayNetwork() {
    std::cout << "\n========================================" << std::endl;
    std::cout << "   DAFTAR JARINGAN GUDANG BERBOBOT" << std::endl;
    std::cout << "========================================" << std::endl;
    for (auto const& [u, neighbors] : adjList) {
        std::cout << "Gudang [" << u << "] terhubung ke:" << std::endl;
        for (auto const& neighbor : neighbors) {
            std::cout << "  -> " << neighbor.first << " (" << neighbor.second << " km)" << std::endl;
        }
    }
}