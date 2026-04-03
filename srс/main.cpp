#include <iostream>
#include <string>
#include "Graph.hpp"
#include "Algorithms.hpp"
#include "Generators.hpp"
#include "IO.hpp"

void printHelp() {
    std::cout << "Usage:\n";
    std::cout << "  lab_graph generate <type> <n> [k] - Generate graph\n";
    std::cout << "  lab_graph parse <format> <file>   - Parse graph\n";
    std::cout << "  lab_graph metric <name> <source>  - Calc metric (source: file or gen)\n";
    std::cout << "  lab_graph serialize <format> <out> <source>\n";
    std::cout << "Types: kn, tn, k_comp, k_bridges, k_articulation, k_2bridges...\n";
    std::cout << "Metrics: density, diameter, components, bipartite...\n";
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        printHelp();
        return 1;
    }

    std::string cmd = argv[1];
    Graph g;

    try {
        if (cmd == "generate") {
            if (argc < 4) throw std::runtime_error("Args: type n [k]");
            std::string type = argv[2];
            int n = std::stoi(argv[3]);
            int k = (argc > 4) ? std::stoi(argv[4]) : 0;
            
            auto gen = createGenerator(type, n, 0, k);
            if (!gen) throw std::runtime_error("Unknown generator");
            g = gen->generate();
            std::cout << "Generated " << type << " with " << g.vertexCount() << " vertices.\n";
        } 
        else if (cmd == "parse") {
            if (argc < 4) throw std::runtime_error("Args: format file");
            auto parser = getParser(argv[2]);
            if (!parser) throw std::runtime_error("Unknown parser");
            g = parser->parse(argv[3]);
            std::cout << "Parsed graph with " << g.vertexCount() << " vertices.\n";
        }
        else if (cmd == "metric") {
            std::string m = argv[2];
            if (m == "density") std::cout << "Density: " << GraphMetrics::density(g) << "\n";
            else if (m == "components") std::cout << "Components: " << GraphMetrics::componentsCount(g) << "\n";
            else if (m == "bipartite") std::cout << "Bipartite: " << (GraphMetrics::isBipartite(g) ? "Yes" : "No") << "\n";
            else throw std::runtime_error("Unknown metric");
        }
        else if (cmd == "serialize") {
            if (argc < 4) throw std::runtime_error("Args: format out_file");
            auto ser = getSerializer(argv[2]);
            if (!ser) throw std::runtime_error("Unknown serializer");
            ser->serialize(g, argv[3]);
            std::cout << "Serialized to " << argv[3] << "\n";
        }
        else {
            printHelp();
        }
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << "\n";
        return 1;
    }

    return 0;
}
