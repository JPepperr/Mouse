#include "graph.h"

void Node::AddEdge(std::shared_ptr<Node> to) {
    edges_.emplace_back(to);
}

void Graph::AddNode(const std::string& name){
    dict_[name] = CNT_NODES;
    g_.emplace_back(std::make_shared<Node>(CNT_NODES++, name));
}

void Graph::AddEdge(const std::string& from, const std::string& to) {
    if (dict_.find(from) == dict_.end()) { AddNode(from); }
    if (dict_.find(to) == dict_.end()) { AddNode(to); }
    uint32_t from_v = dict_[from];
    uint32_t to_v = dict_[to];
    g_[from_v]->AddEdge(g_[to_v]);
}