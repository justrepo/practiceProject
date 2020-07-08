//
// Created by nikita on 7/6/20.
//
#pragma once

#pragma clang diagnostic push
#pragma ide diagnostic ignored "UnusedGlobalDeclarationInspection"

#include "TriangleBoolSquareMatrix.h"

class Graph {
  TriangleBoolSquareMatrix matrix;
public:
  explicit Graph(TriangleBoolSquareMatrix matrix);

  void setMatrix(TriangleBoolSquareMatrix newMatrix);

  [[nodiscard]] size_t countEdgesInSubgraph(const vector<size_t> &vertices) const;

  [[nodiscard]] TriangleBoolSquareMatrix getMatrix() const;

  [[nodiscard]] vector<size_t> findSubgraphWithMaxEdges(size_t targetVerticesNumber) const;
};

#pragma clang diagnostic pop