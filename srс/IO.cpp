#include "IO.hpp"
#include <fstream>
#include <sstream>


class EdgeListParser : public GraphParser {
public:
    Graph parse(const std::string& filename) override {
        Graph g;
        std::ifstream f(filename);
        int u, v;
        std::vector<std::pair<int,int>> edges;
        int max_v = -1;
        while(f >> u >> v) {
            edges.push_back({u, v});
            max_v = std::max(max_v, std::max(u, v));
        }
        for(int i=0; i<=max_v; ++i) g.addVertex();
        for(auto& e : edges) g.addEdge(e.first, e.second);
        return g;
    }
};

class DimacsParser : public GraphParser {
public:
    Graph parse(const std::string& filename) override {
        Graph g;
        std::ifstream f(filename);
        std::string line;
        int n = 0;
        while(std::getline(f, line)) {
            if (line[0] == 'p') {
                sscanf(line.c_str(), "p edge %d", &n);
                for(int i=0; i<n; ++i) g.addVertex();
            } else if (line[0] == 'e') {
                int u, v;
                sscanf(line.c_str(), "e %d %d", &u, &v);
                g.addEdge(u-1, v-1); 
            }
        }
        return g;
    }
};


class GraphVizSerializer : public GraphSerializer {
public:
    void serialize(const Graph& g, const std::string& filename) override {
        std::ofstream f(filename);
        f << "graph G {\n";
        // Вершины с цветами
        for(int i=0; i<g.vertexCount(); ++i) {
            int c = g.getVertexColor(i);
            if (c != 0) f << "  " << i << " [fillcolor=" << c << ", style=filled];\n";
        }
        // Ребра
        for(auto& e : g.edges()) {
            int c = g.getEdgeColor(e.first, e.second);
            f << "  " << e.first << " -- " << e.second;
            if (c != 0) f << " [color=" << c << "]";
            f << ";\n";
        }
        f << "}\n";
    }
};

class EdgesSerializer : public GraphSerializer {
public:
    void serialize(const Graph& g, const std::string& filename) override {
        std::ofstream f(filename);
        for(auto& e : g.edges()) {
            f << e.first << " " << e.second << "\n";
        }
    }
};

std::unique_ptr<GraphParser> getParser(const std::string& format) {
    if (format == "edges") return std::make_unique<EdgeListParser>();
    if (format == "dimacs") return std::make_unique<DimacsParser>();
    return nullptr;
}

std::unique_ptr<GraphSerializer> getSerializer(const std::string& format) {
    if (format == "dot") return std::make_unique<GraphVizSerializer>();
    if (format == "edges") return std::make_unique<EdgesSerializer>();
    return nullptr;
}
