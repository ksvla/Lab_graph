#include <catch2/catch_test_macros.hpp>
#include "Graph.hpp"
#include "Generators.hpp"
#include "Algorithms.hpp"

TEST_CASE("Graph Basic Operations", "[graph]") {
    Graph g;
    int v1 = g.addVertex();
    int v2 = g.addVertex();
    g.addEdge(v1, v2);
    REQUIRE(g.vertexCount() == 2);
    REQUIRE(g.edgeCount() == 1);
    REQUIRE(g.hasEdge(v1, v2));
}

TEST_CASE("Generator Kn", "[gen]") {
    auto gen = createGenerator("kn", 5, 0, 0);
    REQUIRE(gen != nullptr);
    Graph g = gen->generate();
    REQUIRE(g.vertexCount() == 5);
    REQUIRE(g.edgeCount() == 10); // 5*4/2
}

TEST_CASE("Generator Tree", "[gen]") {
    auto gen = createGenerator("tn", 10, 0, 0);
    Graph g = gen->generate();
    REQUIRE(g.vertexCount() == 10);
    REQUIRE(g.edgeCount() == 9); // N-1
    REQUIRE(GraphMetrics::componentsCount(g) == 1);
}

TEST_CASE("Metric Density", "[metric]") {
    Graph g;
    g.addVertex(); g.addVertex(); g.addVertex();
    g.addEdge(0, 1); g.addEdge(1, 2); g.addEdge(0, 2);
    REQUIRE(GraphMetrics::density(g) == 1.0); // K3
}

TEST_CASE("Generator K Components", "[gen]") {
    auto gen = createGenerator("k_comp", 10, 0, 2); // 10 vertices, 2 components
    Graph g = gen->generate();
    REQUIRE(GraphMetrics::componentsCount(g) == 2);
}
