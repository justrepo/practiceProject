//
// Created by nikita on 7/6/20.
//
#pragma clang diagnostic push
#pragma ide diagnostic ignored "UnusedGlobalDeclarationInspection"

#include "Graph.h"
#include "combinatorialUtils.h"
#include <numeric>

using std::iota;

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

TriangleBoolSquareMatrix Graph::getMatrix() const {
  return matrix;
}

vector<size_t> Graph::findSubgraphWithMaxEdges(size_t targetVerticesNumber) const {
  vector<size_t> vertices(targetVerticesNumber);
  iota(vertices.begin(), vertices.end(), 0);

  size_t maxEdges = countEdgesInSubgraph(vertices);
  vector<size_t> verticesForMax = vertices;

  auto graphVerticesNumber = matrix.getDimension();

  while (nextCombination(vertices, graphVerticesNumber)) {
    auto edges = countEdgesInSubgraph(vertices);
    if (edges > maxEdges) {
      maxEdges = edges;
      verticesForMax = vertices;
    }
  }

  return verticesForMax;
}


#pragma clang diagnostic pop