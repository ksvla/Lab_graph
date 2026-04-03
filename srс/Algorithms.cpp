#include "Algorithms.hpp"
#include <queue>
#include <stack>
#include <algorithm>
#include <climits>



// Вспомогательная рекурсивная функция для DFS
void dfsRecursive(const Graph& g, int v, std::vector<bool>& visited, DFSVisitor& visitor) {
    visited[v] = true;
    visitor.onEnter(v);
    for (int to : g.neighbors(v)) {
        visitor.onEdge(v, to);
        if (!visited[to]) {
            dfsRecursive(g, to, visited, visitor);
        }
    }
    visitor.onExit(v);
}

void DFS::run(const Graph& g, DFSVisitor& visitor) {
    std::vector<bool> visited(g.vertexCount(), false);
    for (int i = 0; i < g.vertexCount(); ++i) {
        if (!visited[i]) {
            dfsRecursive(g, i, visited, visitor);
        }
    }
}

void DFS::runFrom(const Graph& g, int start, DFSVisitor& visitor) {
    std::vector<bool> visited(g.vertexCount(), false);
    dfsRecursive(g, start, visited, visitor);
}

// Graph Metrics

double GraphMetrics::density(const Graph& g) {
    int n = g.vertexCount();
    if (n < 2) return 0.0;
    double max_edges = (double)n * (n - 1) / 2.0;
    return g.edgeCount() / max_edges;
}

int GraphMetrics::componentsCount(const Graph& g) {
    std::vector<bool> visited(g.vertexCount(), false);
    int components = 0;
    for (int i = 0; i < g.vertexCount(); ++i) {
        if (!visited[i]) {
            components++;
            std::queue<int> q;
            q.push(i);
            visited[i] = true;
            while(!q.empty()){
                int v = q.front(); q.pop();
                for(int to : g.neighbors(v)){
                    if(!visited[to]){
                        visited[to] = true;
                        q.push(to);
                    }
                }
            }
        }
    }
    return components;
}

bool GraphMetrics::isBipartite(const Graph& g) {
    std::vector<int> color(g.vertexCount(), -1);
    for (int i = 0; i < g.vertexCount(); ++i) {
        if (color[i] == -1) {
            std::queue<int> q;
            q.push(i);
            color[i] = 0;
            while (!q.empty()) {
                int v = q.front(); q.pop();
                for (int to : g.neighbors(v)) {
                    if (color[to] == -1) {
                        color[to] = 1 - color[v];
                        q.push(to);
                    } else if (color[to] == color[v]) {
                        return false;
                    }
                }
            }
        }
    }
    return true;
}

int GraphMetrics::diameter(const Graph& g) {
    if (g.vertexCount() == 0) return 0;
    int max_dist = 0;
    for (int i = 0; i < g.vertexCount(); ++i) {
        std::vector<int> dist(g.vertexCount(), -1);
        std::queue<int> q;
        q.push(i);
        dist[i] = 0;
        while (!q.empty()) {
            int v = q.front(); q.pop();
            for (int to : g.neighbors(v)) {
                if (dist[to] == -1) {
                    dist[to] = dist[v] + 1;
                    max_dist = std::max(max_dist, dist[to]);
                    q.push(to);
                }
            }
        }
    }
    return max_dist;
}

double GraphMetrics::transitivity(const Graph& g) {
    long long triangles = 0;
    long long triples = 0;
    for (int v = 0; v < g.vertexCount(); ++v) {
        int deg = g.neighbors(v).size();
        if (deg < 2) continue;
        triples += (long long)deg * (deg - 1) / 2;
        for (size_t i = 0; i < g.neighbors(v).size(); ++i) {
            for (size_t j = i + 1; j < g.neighbors(v).size(); ++j) {
                if (g.hasEdge(g.neighbors(v)[i], g.neighbors(v)[j])) {
                    triangles++;
                }
            }
        }
    }
    if (triples == 0) return 0.0;
    return (double)triangles / triples;
}



// Вспомогательная рекурсивная функция для поиска точек сочленения и мостов
void findCutPointsAndBridgesDFS(const Graph& g, 
                                int v, 
                                int parent, 
                                std::vector<int>& tin, 
                                std::vector<int>& low, 
                                int& timer,
                                std::vector<bool>& is_articulation,
                                int& bridge_count,
                                std::vector<bool>& visited) {
    visited[v] = true;
    tin[v] = low[v] = timer++;
    int children = 0;
    
    for (int to : g.neighbors(v)) {
        if (to == parent) continue;
        
        if (visited[to]) {
            // Обратное ребро 
            low[v] = std::min(low[v], tin[to]);
        } else {
            // Прямое ребро 
            findCutPointsAndBridgesDFS(g, to, v, tin, low, timer, 
                                       is_articulation, bridge_count, visited);
            low[v] = std::min(low[v], low[to]);
            
            // Проверка на мост
            if (low[to] > tin[v]) {
                bridge_count++;
            }
            
            // Проверка на точку сочленения (не корень)
            if (parent != -1 && low[to] >= tin[v]) {
                is_articulation[v] = true;
            }
            children++;
        }
    }
    
    // Корень является точкой сочленения, если у него 2+ детей в дереве DFS
    if (parent == -1 && children > 1) {
        is_articulation[v] = true;
    }
}

int GraphMetrics::articulationPointsCount(const Graph& g) {
    int n = g.vertexCount();
    if (n == 0) return 0;
    
    std::vector<int> tin(n, -1), low(n, -1);
    std::vector<bool> is_articulation(n, false);
    std::vector<bool> visited(n, false);
    int timer = 0;
    int bridge_count = 0; // не используем результат, но нужен для сигнатуры
    
    for (int i = 0; i < n; ++i) {
        if (!visited[i]) {
            findCutPointsAndBridgesDFS(g, i, -1, tin, low, timer, 
                                       is_articulation, bridge_count, visited);
        }
    }
    
    // Подсчитываем количество точек сочленения
    int count = 0;
    for (bool is_art : is_articulation) {
        if (is_art) count++;
    }
    return count;
}

int GraphMetrics::bridgesCount(const Graph& g) {
    int n = g.vertexCount();
    if (n == 0) return 0;
    
    std::vector<int> tin(n, -1), low(n, -1);
    std::vector<bool> is_articulation(n, false);
    std::vector<bool> visited(n, false);
    int timer = 0;
    int bridge_count = 0;
    
    for (int i = 0; i < n; ++i) {
        if (!visited[i]) {
            findCutPointsAndBridgesDFS(g, i, -1, tin, low, timer, 
                                       is_articulation, bridge_count, visited);
        }
    }
    
    return bridge_count;
}


ArticulationFinder::ArticulationFinder(const Graph& g) 
    : g_(g), timer_(0), articulation_count_(0) {
    tin_.assign(g.vertexCount(), -1);
    low_.assign(g.vertexCount(), -1);
    is_articulation_.assign(g.vertexCount(), false);
}

void ArticulationFinder::onEnter(int v) {
    tin_[v] = low_[v] = timer_++;
}

void ArticulationFinder::onEdge(int u, int v) {
}

void ArticulationFinder::onExit(int v) {
}
