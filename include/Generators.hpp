#pragma once
#include "Graph.hpp"
#include <memory>

class GraphGenerator {
public:
    virtual ~GraphGenerator() = default;
    virtual Graph generate() = 0;
    virtual std::string name() const = 0;
};

// Фабрика генераторов
std::unique_ptr<GraphGenerator> createGenerator(const std::string& type, int n, int m = 0, int k = 0);
