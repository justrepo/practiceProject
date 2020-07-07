#include <iostream>
#include <chrono>
#include "Graph.h"

using namespace std;
namespace cr = std::chrono;

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

int main() {
  const size_t n = 15;
  auto matrix = TriangleBoolSquareMatrix(n);
  matrix.randomInit();

  Graph graph(matrix);

  auto start = cr::high_resolution_clock::now(), end = start;
  for (size_t i = 2; i <= n; ++i) {
    start = cr::high_resolution_clock::now();
    auto verticesForMax = graph.findSubgraphWithMaxEdges(i);
    end = cr::high_resolution_clock::now();

    auto maxEdges = graph.countEdgesInSubgraph(verticesForMax);

    cout << "for k = " << i << " max links = " << maxEdges << " for, for example, vertices " << verticesForMax << endl;
    cout << "matrix with that vertices: " << endl;
    matrix.printFullSubmatrix(cout, verticesForMax) << endl;
    cout << "it took " << (end - start).count() << "ns = " << double((end - start).count()) * 1e-9 << "s" << endl;
  }
  return 0;
}
