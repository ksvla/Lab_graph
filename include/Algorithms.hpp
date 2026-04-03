#pragma once
#include "Graph.hpp"
#include <vector>
#include <functional>

// Паттерн Visitor для DFS
class DFSVisitor {
public:
    virtual ~DFSVisitor() = default;
    virtual void onEnter(int v) {}
    virtual void onExit(int v) {}
    virtual void onEdge(int u, int v) {}
};

class DFS {
public:
    static void run(const Graph& g, DFSVisitor& visitor);
    static void runFrom(const Graph& g, int start, DFSVisitor& visitor);
};

// Классы для вычисления метрик
class GraphMetrics {
public:
    static double density(const Graph& g);
    static int componentsCount(const Graph& g);
    static int articulationPointsCount(const Graph& g);
    static int bridgesCount(const Graph& g);
    static bool isBipartite(const Graph& g);
    static int diameter(const Graph& g);
    static double transitivity(const Graph& g);
};

// Специализированные визиторы
class ArticulationFinder : public DFSVisitor {
public:
    ArticulationFinder(const Graph& g);
    void onEnter(int v) override;
    void onExit(int v) override;
    void onEdge(int u, int v) override;
    int count() const { return articulation_count_; }
private:
    const Graph& g_;
    std::vector<int> tin_, low_;
    int timer_;
    int articulation_count_;
    std::vector<bool> is_articulation_;
};
