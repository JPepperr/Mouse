#!/bin/bash
python3 parser.py
g++ main.cpp graph.cpp mouse.cpp -o main -std=c++17
./main