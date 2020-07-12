//
// Created by nikita on 7/10/20.
//

#pragma once

#include "Graph.h"

vector<size_t> findSubgraphWithMaxEdgesUsingParallelTree(size_t targetVerticesNumber, const Graph &graph,
                                                         size_t threads = 0);
