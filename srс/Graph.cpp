#include "Graph.hpp"
#include <algorithm>
#include <map>

Graph::Graph(bool directed) : directed_(directed), n_(0), m_(0) {}

int Graph::addVertex() {
    adj_.emplace_back();
    vertex_colors_.push_back(0); // Default color
    return n_++;
}

void Graph::addEdge(int u, int v, int weight) {
    if (u < 0 || v < 0 || u >= n_ || v >= n_) return;
    if (hasEdge(u, v)) return;

    adj_[u].push_back(v);
    if (!directed_) adj_[v].push_back(u);
    
    edge_list_.push_back({u, v});
    m_++;
}

bool Graph::removeEdge(int u, int v) {
    // Упрощенная реализация удаления (O(deg))
    auto it = std::find(adj_[u].begin(), adj_[u].end(), v);
    if (it == adj_[u].end()) return false;
    adj_[u].erase(it);
    
    if (!directed_) {
        auto it2 = std::find(adj_[v].begin(), adj_[v].end(), u);
        if (it2 != adj_[v].end()) adj_[v].erase(it2);
    }
    
    // Удаляем из edge_list (медленно, но для лабы ок)
    edge_list_.erase(std::remove_if(edge_list_.begin(), edge_list_.end(), 
        [u, v, this](const std::pair<int,int>& e){
            return (e.first == u && e.second == v) || (!directed_ && e.first == v && e.second == u);
        }), edge_list_.end());
    m_--;
    return true;
}

bool Graph::hasEdge(int u, int v) const {
    if (u < 0 || v < 0 || u >= n_ || v >= n_) return false;
    return std::find(adj_[u].begin(), adj_[u].end(), v) != adj_[u].end();
}

bool Graph::hasVertex(int v) const {
    return v >= 0 && v < n_;
}

int Graph::vertexCount() const { return n_; }
int Graph::edgeCount() const { return m_; }

const std::vector<int>& Graph::neighbors(int v) const {
    return adj_[v];
}

const std::vector<std::pair<int, int>>& Graph::edges() const {
    return edge_list_;
}

void Graph::setVertexColor(int v, int color) {
    if (v >= 0 && v < n_) vertex_colors_[v] = color;
}
int Graph::getVertexColor(int v) const {
    return (v >= 0 && v < n_) ? vertex_colors_[v] : 0;
}

void Graph::setEdgeColor(int u, int v, int color) {
    if (u > v) std::swap(u, v);
    edge_colors_[{u, v}] = color;
}
int Graph::getEdgeColor(int u, int v) const {
    if (u > v) std::swap(u, v);
    auto it = edge_colors_.find({u, v});
    return (it != edge_colors_.end()) ? it->second : 0;
}

void Graph::clear() {
    adj_.clear();
    edge_list_.clear();
    vertex_colors_.clear();
    edge_colors_.clear();
    n_ = 0;
    m_ = 0;
}

bool Graph::isLeaf(int v) const {
    return hasVertex(v) && adj_[v].size() == 1;
}
