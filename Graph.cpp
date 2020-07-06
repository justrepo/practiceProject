//
// Created by nikita on 7/6/20.
//

#include "Graph.h"

size_t Graph::countEdgesInSubgraph(const vector<size_t> &vertices) const {
  size_t result = 0;
  for (size_t i = 1; i < vertices.size(); ++i) {
    for (size_t j = 0; j < i; ++j) {
      if (matrix.at(vertices[i], vertices[j])) {
        ++result;
      }
    }
  }
  return result;
}

Graph::Graph(TriangleBoolSquareMatrix matrix) : matrix(std::move(matrix)) {}
