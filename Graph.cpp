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
      if (matrix.unsafeAt(vertices[i], vertices[j])) {
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

vector<size_t> Graph::findSubgraphWithMaxEdges(size_t targetVerticesNumber, AlgorithmType type) const {
  switch (type) {
    case AlgorithmType::STACK:
      return findSubgraphWithMaxEdgesUsingStack(targetVerticesNumber);
    case AlgorithmType::RECURSION:
      return findSubgraphWithMaxEdgesUsingRecursion(matrix.getDimension() - targetVerticesNumber, 0,
                                                    vector<size_t>(targetVerticesNumber)).second;
  }
  return vector<size_t>();
}

void Graph::setMatrix(TriangleBoolSquareMatrix newMatrix) {
  matrix = std::move(newMatrix);
}

pair<size_t, vector<size_t>> Graph::findSubgraphWithMaxEdgesUsingRecursion(size_t difference, size_t level,
                                                                           const vector<size_t> &vertices) const {
  if (vertices.empty() || level == vertices.size()) {
    return {countEdgesInSubgraph(vertices), vertices};
  } else {
    auto newVertices = vertices;
    if (level > 0) {
      newVertices[level] = newVertices[level - 1] + 1;
    }
    auto maxValue = findSubgraphWithMaxEdgesUsingRecursion(difference, level + 1, newVertices);
    while (newVertices[level] < difference + level) {
      ++newVertices[level];
      auto currentValue = findSubgraphWithMaxEdgesUsingRecursion(difference, level + 1, newVertices);
      if (currentValue.first > maxValue.first) {
        maxValue = currentValue;
      }
    }
    return maxValue;
  }
}

vector<size_t> Graph::findSubgraphWithMaxEdgesUsingStack(size_t targetVerticesNumber) const {
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