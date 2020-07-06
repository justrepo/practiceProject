//
// Created by nikita on 7/6/20.
//
#pragma once

#include "TriangleBoolSquareMatrix.h"

class Graph {
  TriangleBoolSquareMatrix matrix;
public:
  explicit Graph(TriangleBoolSquareMatrix matrix);

  [[nodiscard]] size_t countEdgesInSubgraph(const vector<size_t> &vertices) const;

  TriangleBoolSquareMatrix getMatrix() const;
};