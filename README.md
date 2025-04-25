# SortBattleBenchmark

A benchmarking tool for comparing the performance of common sorting algorithms across different array sizes. The implemented algorithms are:

- Insertion Sort
- Merge Sort
- Quick Sort
- Radix Sort

Execution times are measured using C++'s `<chrono>` library and results are written to a CSV file.

## Features

- Measures sorting time for arrays of size 10 to 100,000
- Uses randomized data for realistic testing
- Handles long-running algorithms with a timeout
- Outputs cleanly formatted CSV and console tables

## Usage

1. Compile the program using a C++ compiler (e.g., g++):

```bash
g++ -std=c++11 -O2 -o benchmark main.cpp
