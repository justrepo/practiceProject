#include <iostream>
#include <numeric>
#include <random>
#include <chrono>
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

int main() {
  const size_t n = 30;
  auto matrix = getRandomMatrix(n);
  matrix.printFull(cout) << endl;

  Graph graph(matrix);

  vector<size_t> vertices{0};
  auto start = cr::high_resolution_clock::now(), end = start;
  for (size_t i = 2; i <= n; ++i) {
    vertices.push_back(0);
    iota(vertices.begin(), vertices.end(), 0);

    size_t maxLinks = 0;
    vector<size_t> verticesForMax;

    start = cr::high_resolution_clock::now();
    do {
      auto links = graph.countLinksInSubgraph(vertices);
      if (links > maxLinks) {
        maxLinks = links;
        verticesForMax = vertices;
      }
    } while (nextCombination(vertices, n));
    end = cr::high_resolution_clock::now();

    cout << "for k = " << i << " max links = " << maxLinks << " for, for example, vertices " << verticesForMax << endl;
    cout << "matrix with that vertices: " << endl;
    matrix.printFullSubmatrix(cout, verticesForMax) << endl;
    cout << "it took " << (end - start).count() << "ns = " << double((end - start).count()) * 1e-9 << "s" << endl;
  }
  return 0;
}
