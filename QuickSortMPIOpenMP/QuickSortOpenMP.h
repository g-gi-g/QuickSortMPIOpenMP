#include <vector>
#include <omp.h>
#include <algorithm>
#include <iostream>
#define PARALLEL_THRESHOLD 1000

std::vector<int> QuickSortOpenMP(std::vector<int>& array, int numThreads);

