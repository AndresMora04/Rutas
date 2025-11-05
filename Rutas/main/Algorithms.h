#pragma once
#include <iostream>
#include <queue>
#include <stack>
#include <map>
#include <climits>
#include "Graph.h"
class Algorithms
{
public:
    Algorithms();

    void BFS(Graph* graph, Station* start);
    void DFS(Graph* graph, Station* start);
    void Dijkstra(Graph* graph, Station* start, Station* end);
};

