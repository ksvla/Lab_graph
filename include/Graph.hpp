#pragma once
#include <vector>
#include <unordered_set>
#include <optional>
#include <memory>
#include <map>

class Graph {
public:
    Graph(bool directed = false);
    
    // Вершины и ребра
    int addVertex();
    void addEdge(int u, int v, int weight = 1);
    bool removeEdge(int u, int v);
    bool hasEdge(int u, int v) const;
    bool hasVertex(int v) const;
    
    // Доступ
    int vertexCount() const;
    int edgeCount() const;
    const std::vector<int>& neighbors(int v) const;
    const std::vector<std::pair<int, int>>& edges() const;
    
    // Свойства вершин 
    void setVertexColor(int v, int color);
    int getVertexColor(int v) const;
    void setEdgeColor(int u, int v, int color);
    int getEdgeColor(int u, int v) const;

    // Утилиты
    void clear();
    void renumberVertices(const std::vector<int>& mapping);
    bool isLeaf(int v) const;

private:
    bool directed_;
    int n_;
    int m_;
    std::vector<std::vector<int>> adj_;
    std::vector<std::pair<int, int>> edge_list_;
    std::vector<int> vertex_colors_;
    std::map<std::pair<int, int>, int> edge_colors_; 
};
