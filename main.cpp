#include <iostream>
#include <string>
#include "analysis.h"

Seria f{"F1-24"}, h{"I-hpc"};

int main() {
    freopen("raw_data.txt", "r", stdin);
    GetLines(f, h);
    BuildPlotData(f, h);
    //PrintTableFeedersFound(f);
    //PrintTableFeedersFound(h);
}