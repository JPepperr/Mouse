#include <iostream>
#include <string>
#include "graph.h"
#include "mouse.h"

Graph g;
Seria f{"F1-24"}, h{"I-hpc"};

int main() {
    freopen("raw_data.txt", "r", stdin);
    GetLines(f, h);
    g.BuildGraph(f); g.BuildGraph(h);
}