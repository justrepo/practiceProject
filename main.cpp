#include <iostream>
#include <numeric>
#include <random>
#include <chrono>
#include <fstream>
#include "Graph.h"

using namespace std;
namespace cr = std::chrono;

bool nextCombination(vector<size_t> &collection, size_t maxValue) {
  if (++collection.back() == maxValue) {
    size_t i = collection.size() - 1, currentMaxValue = maxValue;
    for (; i > 0 && collection[i] == currentMaxValue; --i) {
      ++collection[i - 1];
      --currentMaxValue;
    }
    if (i == 0 && collection.front() == currentMaxValue) {
      return false;
    }
    ++i;
    for (; i < collection.size(); ++i) {
      collection[i] = collection[i - 1] + 1;
    }
  }
  return true;
}

ostream &operator<<(ostream &out, const vector<size_t> &arr) {
  if (arr.empty()) {
    return out << "{}";
  } else {
    out << "{" << arr.front();
    for (size_t i = 1; i < arr.size(); ++i) {
      out << ", " << arr[i];
    }
    return out << "}";
  }
}

TriangleBoolSquareMatrix getRandomMatrix(size_t n) {
  TriangleBoolSquareMatrix matrix(n);

  static mt19937 twisterEngine((random_device()()));
  static bernoulli_distribution distribution;

  for (size_t i = 1; i < n; ++i) {
    for (size_t j = 0; j < i; ++j) {
      matrix.at(i, j) = distribution(twisterEngine);
    }
  }

  return matrix;
}

vector<size_t> findSubgraphWithMaxEdges(const Graph &graph, size_t targetVerticesNumber) {
  vector<size_t> vertices(targetVerticesNumber);
  iota(vertices.begin(), vertices.end(), 0);

  size_t maxEdges = 0;
  vector<size_t> verticesForMax;

  auto graphVerticesNumber = graph.getMatrix().getDimension();

  do {
    auto edges = graph.countEdgesInSubgraph(vertices);
    if (edges > maxEdges) {
      maxEdges = edges;
      verticesForMax = vertices;
    }
  } while (nextCombination(vertices, graphVerticesNumber));

  return verticesForMax;
}

int main() {
  const size_t n = 15;
  auto matrix = getRandomMatrix(n);

  Graph graph(matrix);

  auto start = cr::high_resolution_clock::now(), end = start;
  for (size_t i = 2; i <= n; ++i) {
    start = cr::high_resolution_clock::now();
    auto verticesForMax = findSubgraphWithMaxEdges(graph, i);
    end = cr::high_resolution_clock::now();

    auto maxEdges = graph.countEdgesInSubgraph(verticesForMax);

    cout << "for k = " << i << " max links = " << maxEdges << " for, for example, vertices " << verticesForMax << endl;
    cout << "matrix with that vertices: " << endl;
    matrix.printFullSubmatrix(cout, verticesForMax) << endl;
    cout << "it took " << (end - start).count() << "ns = " << double((end - start).count()) * 1e-9 << "s" << endl;
  }
  return 0;
}
