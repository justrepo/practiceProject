//
// Created by nikita on 7/6/20.
//
#pragma once

#pragma clang diagnostic push
#pragma ide diagnostic ignored "UnusedGlobalDeclarationInspection"

#include "TriangleBoolSquareMatrix.h"

using std::pair;

class Graph {
  TriangleBoolSquareMatrix matrix;

  [[nodiscard]] vector<size_t> findSubgraphWithMaxEdgesUsingStack(size_t targetVerticesNumber) const;

  [[nodiscard]] pair<size_t, vector<size_t>> findSubgraphWithMaxEdgesUsingRecursion(size_t targetVerticesNumber,
                                                                                    size_t level,
                                                                                    const vector<size_t> &vertices) const;

public:
  explicit Graph(TriangleBoolSquareMatrix matrix);

  void setMatrix(TriangleBoolSquareMatrix newMatrix);

  [[nodiscard]] size_t countEdgesInSubgraph(const vector<size_t> &vertices) const;

  [[nodiscard]] TriangleBoolSquareMatrix getMatrix() const;

  enum class AlgorithmType {
    STACK, RECURSION
  };

  [[nodiscard]] vector<size_t> findSubgraphWithMaxEdges(size_t targetVerticesNumber, AlgorithmType type) const;
};

#pragma clang diagnostic pop