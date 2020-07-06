//
// Created by nikita on 7/6/20.
//
#pragma once

#include "TriangleBoolSquareMatrix.h"

class Graph {
  TriangleBoolSquareMatrix matrix;
public:
  Graph(TriangleBoolSquareMatrix matrix);

  [[nodiscard]] size_t countLinksInSubgraph(const vector<size_t> &vertices) const;

};