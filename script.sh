#!/bin/bash
# python3 parser.py
g++ main.cpp graph.cpp mouse.cpp analysis.cpp -o main -std=c++17
./main
python3 build_plot.py