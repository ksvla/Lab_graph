#pragma once
#include "Graph.hpp"
#include <string>
#include <memory>

class GraphParser {
public:
    virtual ~GraphParser() = default;
    virtual Graph parse(const std::string& filename) = 0;
};

class GraphSerializer {
public:
    virtual ~GraphSerializer() = default;
    virtual void serialize(const Graph& g, const std::string& filename) = 0;
};

std::unique_ptr<GraphParser> getParser(const std::string& format);
std::unique_ptr<GraphSerializer> getSerializer(const std::string& format);
