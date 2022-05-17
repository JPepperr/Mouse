#include <iostream>
#include <string>
#include "graph.h"
#include "mouse.h"
#include "analysis.h"

Graph g;
Seria f{"F1-24"}, h{"I-hpc"};

int main() {
    freopen("raw_data.txt", "r", stdin);
    GetLines(f, h);
    g.BuildGraph(f); g.BuildGraph(h);
    DistMatrix d(g);
    // d.PrintMatrix();
    //auto r = GetCompressions(f);
    //r.Print();
    //std::cout << double(r.res_.size()) / double(f.GetMice().size()) << '\n';
    //auto r2 = GetCompressions(h);
    //r2.Print();
    //std::cout << double(r2.res_.size()) / double(h.GetMice().size()) << '\n';
    BuildPlotData(f, h);
}