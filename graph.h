#ifndef GPARH_H
#define GRAPH_H

#include <vector>
#include <string>
#include <memory>
#include <map>
#include <queue>
#include "mouse.h"

class Node {
public:
    Node(uint32_t num, const std::string& name) : num_(num), name_(name) {}
    void AddEdge(std::shared_ptr<Node> to);
    const std::vector<std::shared_ptr<Node>>& GetEdges() const;
    uint32_t GetNum() const;
    const std::string& GetName() const;
private:
    std::vector<std::shared_ptr<Node>> edges_;
    uint32_t num_;
    std::string name_;
};

class Graph {
public:
    uint32_t CNT_NODES = 0;
    void AddEdge(const std::string& from, const std::string& to);
    void AddNode(const std::string& name);
    void BuildGraph(Seria& s);
    std::shared_ptr<Node> GetNode(uint32_t num) const;
    uint32_t GetNum(const std::string& name);
private:
    std::map<std::string, uint32_t> dict_;
    std::vector<std::shared_ptr<Node>> g_;
};

class DistMatrix {
public:
    DistMatrix() = default;
    DistMatrix(const Graph& g);
    std::vector<uint32_t> CalcDist(uint32_t num);
    void PrintMatrix() const;
    uint32_t GetDist(const std::string& from, const std::string& to) const;
private:
    std::shared_ptr<Graph> g_;
    std::vector<std::vector<uint32_t>> d_;
};

#endif