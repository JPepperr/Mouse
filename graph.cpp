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

void Graph::BuildGraph(Seria& s) {
    auto it = s.Begin();
    while (it != s.End()) {
        const auto& str = *it;
        for (size_t i = 0; i < str.size() - 1; i++) {
            AddEdge(str.substr(i, 1), str.substr(i + 1, 1));
        }
        it = s.NextIt(it);
    }
}

const std::vector<std::shared_ptr<Node>>& Node::GetEdges() const {
    return edges_;
}

uint32_t Node::GetNum() const {
    return num_;
}

const std::string& Node::GetName() const {
    return name_;
}

std::shared_ptr<Node> Graph::GetNode(uint32_t num) const{
    return g_[num];
}

std::vector<uint32_t> DistMatrix::CalcDist(uint32_t num) {
    std::vector<uint32_t> d(g_->CNT_NODES, -1);
    std::queue<uint32_t> q;
    d[num] = 0;
    q.push(num);
    while (!q.empty()) {
        auto v = q.front();
        q.pop();
        for (const auto e : g_->GetNode(v)->GetEdges()) {
            uint32_t to = e->GetNum();
            if (d[to] == -1) {
                d[to] = d[v] + 1;
                q.push(to);
            }
        }
    }
    return d;
}

DistMatrix::DistMatrix(const Graph& g) {
    g_ = std::make_shared<Graph>(g);
    d_.resize(g.CNT_NODES);
    for (size_t i = 0; i < d_.size(); i++) {
        d_[i] = CalcDist(i);
    }
}

void DistMatrix::PrintMatrix() const {
    std::cout << "\t";  
    for (size_t i = 0; i < d_.size(); i++) {
        std::cout << g_->GetNode(i)->GetName() << '\t';
    }
    std::cout << '\n';
    for (size_t i = 0; i < d_.size(); i++) {
        std::cout << g_->GetNode(i)->GetName() << '\t';
        for (auto j : d_[i]) {
            std::cout << j << '\t';
        }
        std::cout << '\n';
    }
}

uint32_t Graph::GetNum(const std::string& name) {
    return dict_[name];
}

uint32_t DistMatrix::GetDist(const std::string& from, const std::string& to) const {
    return d_[g_->GetNum(from)][g_->GetNum(to)];
}