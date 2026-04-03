#include "Generators.hpp"
#include <random>
#include <numeric>

// Базовые классы
class CompleteGraphGen : public GraphGenerator {
    int n_;
public:
    CompleteGraphGen(int n) : n_(n) {}
    std::string name() const override { return "Kn"; }
    Graph generate() override {
        Graph g;
        for(int i=0; i<n_; ++i) g.addVertex();
        for(int i=0; i<n_; ++i)
            for(int j=i+1; j<n_; ++j)
                g.addEdge(i, j);
        return g;
    }
};

class TreeGen : public GraphGenerator {
    int n_;
public:
    TreeGen(int n) : n_(n) {}
    std::string name() const override { return "Tn"; }
    Graph generate() override {
        Graph g;
        for(int i=0; i<n_; ++i) g.addVertex();
        std::mt19937 gen(42);
        for(int i=1; i<n_; ++i) {
            std::uniform_int_distribution<> dis(0, i-1);
            g.addEdge(i, dis(gen));
        }
        return g;
    }
};

class KComponentsGen : public GraphGenerator {
    int n_, k_;
public:
    KComponentsGen(int n, int k) : n_(n), k_(k) {}
    std::string name() const override { return "KComponents"; }
    Graph generate() override {
        Graph g;
        int per_comp = n_ / k_;
        int start = 0;
        for(int c=0; c<k_; ++c) {
            int count = (c == k_-1) ? (n_ - start) : per_comp;
            int base = g.vertexCount();
            for(int i=0; i<count; ++i) g.addVertex();
            for(int i=1; i<count; ++i) g.addEdge(base + i, base);
            start += count;
        }
        return g;
    }
};

class KBridgesGen : public GraphGenerator {
    int n_, k_;
public:
    KBridgesGen(int n, int k) : n_(n), k_(k) {}
    std::string name() const override { return "KBridges"; }
    Graph generate() override {
        Graph g;
        // Создаем цепочку из компонент, соединенных мостами
        // Нужно k мостов тогда k+1 компонента
        int comps = k_ + 1;
        int per_comp = std::max(1, n_ / comps);
        
        int prev_connector = -1;
        for(int c=0; c<comps; ++c) {
            int count = (c == comps-1) ? (n_ - g.vertexCount()) : per_comp;
            int base = g.vertexCount();
            for(int i=0; i<count; ++i) g.addVertex();
            
            // Связываем внутри компоненты
            if (count > 1) {
                for(int i=0; i<count; ++i) g.addEdge(base + i, base + (i+1)%count);
            }
            
            // Соединяем мостом с предыдущей
            if (prev_connector != -1 && g.vertexCount() > base) {
                g.addEdge(prev_connector, base); // Мост
            }
            if (g.vertexCount() > base) prev_connector = base;
        }
        return g;
    }
};

// Генератор 12: K точек сочленения
class KArticulationGen : public GraphGenerator {
    int n_, k_;
public:
    KArticulationGen(int n, int k) : n_(n), k_(k) {}
    std::string name() const override { return "KArticulation"; }
    Graph generate() override {
        Graph g;
        int current = 0;
        for(int i=0; i<k_; ++i) {
            if (g.vertexCount() >= n_) break;
            g.addVertex(); // Точка сочленения
            int art = g.vertexCount() - 1;
            
            // Прицепляем цикл к ней
            if (g.vertexCount() < n_) {
                g.addVertex(); 
                g.addEdge(art, g.vertexCount()-1);
                if (g.vertexCount() < n_) {
                    g.addVertex();
                    g.addEdge(g.vertexCount()-1, g.vertexCount()-2);
                    g.addEdge(g.vertexCount()-1, art);
                }
            }
        }
        // Добиваем вершины в последнюю компоненту
        while(g.vertexCount() < n_) {
            g.addVertex();
            g.addEdge(g.vertexCount()-1, g.vertexCount()-2);
        }
        return g;
    }
};

// Генератор 13: K 2-мостов 
class K2BridgesGen : public GraphGenerator {
    int n_, k_;
public:
    K2BridgesGen(int n, int k) : n_(n), k_(k) {}
    std::string name() const override { return "K2Bridges"; }
    Graph generate() override {
        Graph g;
        
        for(int i=0; i<n_; ++i) g.addVertex();
        
        // Базовый цикл
        for(int i=0; i<n_; ++i) g.addEdge(i, (i+1)%n_);
        return g;
    }
};

// Фабрика
std::unique_ptr<GraphGenerator> createGenerator(const std::string& type, int n, int m, int k) {
    if (type == "kn") return std::make_unique<CompleteGraphGen>(n);
    if (type == "tn") return std::make_unique<TreeGen>(n);
    if (type == "k_comp") return std::make_unique<KComponentsGen>(n, k);
    if (type == "k_bridges") return std::make_unique<KBridgesGen>(n, k);
    if (type == "k_articulation") return std::make_unique<KArticulationGen>(n, k);
    if (type == "k_2bridges") return std::make_unique<K2BridgesGen>(n, k);
    return nullptr;
}
