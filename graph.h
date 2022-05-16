#ifndef GPARH_H
#define GRAPH_H

#include <vector>
#include <string>
#include <memory>
#include <map>
#include "mouse.h"

class Node {
public:
    Node(uint32_t num, const std::string& name): num_(num), name_(name) {}
    void AddEdge(std::shared_ptr<Node> to);
private:
    
    std::vector<std::shared_ptr<Node>> edges_;
    uint32_t num_;
    std::string name_;
};

class Graph {
public:
    inline static uint32_t CNT_NODES = 0;
    void AddEdge(const std::string& from, const std::string& to);
    void AddNode(const std::string& name);
    void BuildGraph(Seria& s);
private:
    std::map<std::string, uint32_t> dict_;
    std::vector<std::shared_ptr<Node>> g_;
};

#endif