//
// Created by nikita on 7/7/20.
//

#include <catch2/catch.hpp>
#include "../combinatorialUtils.h"
#include "../Graph.h"

TEST_CASE("Test countEdgesInSubgraph", "[Graph]") {
  SECTION("Test for empty graph") {
    Graph graph((TriangleBoolSquareMatrix()));

    REQUIRE(graph.countEdgesInSubgraph({}) == 0);
  }

  SECTION("Test for graph with 2 vertices") {
    {
      TriangleBoolSquareMatrix matrix(2);
      Graph graph(matrix);

      REQUIRE(graph.countEdgesInSubgraph({}) == 0);
      REQUIRE(graph.countEdgesInSubgraph({0}) == 0);
      REQUIRE(graph.countEdgesInSubgraph({1}) == 0);
      REQUIRE(graph.countEdgesInSubgraph({0, 1}) == 0);
    }

    {
      TriangleBoolSquareMatrix matrix(2);
      matrix.at(1, 0) = true;
      Graph graph(matrix);

      REQUIRE(graph.countEdgesInSubgraph({}) == 0);
      REQUIRE(graph.countEdgesInSubgraph({0}) == 0);
      REQUIRE(graph.countEdgesInSubgraph({1}) == 0);
      REQUIRE(graph.countEdgesInSubgraph({0, 1}) == 1);
    }
  }

  SECTION("Test for K5") {
    TriangleBoolSquareMatrix matrix(5);
    matrix.at(1, 0) = true;
    matrix.at(2, 0) = true;
    matrix.at(2, 1) = true;
    matrix.at(3, 0) = true;
    matrix.at(3, 1) = true;
    matrix.at(3, 2) = true;
    matrix.at(4, 0) = true;
    matrix.at(4, 1) = true;
    matrix.at(4, 2) = true;
    matrix.at(4, 3) = true;
    Graph graph(matrix);

    auto i = GENERATE(range(1ul, 6ul));
    vector<size_t> vertices(i);
    iota(vertices.begin(), vertices.end(), 0);

    do {
      REQUIRE(graph.countEdgesInSubgraph(vertices) == i * (i - 1) / 2);
    } while (nextCombination(vertices, 5));
  }

  SECTION("Test for sample graph") {
    /* Sample graph :
     *   0---|
     *   |   |
     *   1---2
     *   |
     *   3---4
     *
     * has matrix:
     * 0 1 1 0 0
     * 1 0 1 1 0
     * 1 1 0 0 0
     * 0 1 0 0 1
     * 0 0 0 1 0
     *
     *
     */

    TriangleBoolSquareMatrix matrix(5);
    matrix.at(1, 0) = true;
    matrix.at(2, 0) = true;
    matrix.at(2, 1) = true;
    matrix.at(3, 1) = true;
    matrix.at(4, 3) = true;
    Graph graph(matrix);

    REQUIRE(graph.countEdgesInSubgraph({}) == 0);

    REQUIRE(graph.countEdgesInSubgraph({0}) == 0);
    REQUIRE(graph.countEdgesInSubgraph({3}) == 0);

    REQUIRE(graph.countEdgesInSubgraph({0, 1}) == 1);
    REQUIRE(graph.countEdgesInSubgraph({0, 4}) == 0);

    REQUIRE(graph.countEdgesInSubgraph({0, 1, 2}) == 3);
    REQUIRE(graph.countEdgesInSubgraph({1, 3, 4}) == 2);
    REQUIRE(graph.countEdgesInSubgraph({0, 3, 4}) == 1);
    REQUIRE(graph.countEdgesInSubgraph({1, 2, 4}) == 1);
  }
}

bool nextBinaryString(vector<bool> &str) {
  if (str.back()) {
    size_t i = str.size() - 1;
    for (; str[i] && i > 0; --i) {
      str[i] = false;
    }
    if (i == 0 && str.front()) {
      str.front() = false;
      return false;
    } else {
      str[i] = true;
      return true;
    }
  } else {
    str.back() = true;
    return true;
  }
}

TEST_CASE("Test findSubgraphWithMaxEdges", "[Graph]") {
  auto algorithm = GENERATE(Graph::AlgorithmType::RECURSION, Graph::AlgorithmType::STACK);
  SECTION("Test for graph with 2 vertices") {
    {
      TriangleBoolSquareMatrix matrix(2);
      Graph graph(matrix);

      REQUIRE(graph.findSubgraphWithMaxEdges(0, algorithm).empty());
      auto result = graph.findSubgraphWithMaxEdges(1, algorithm);
      REQUIRE((result == vector<size_t>{0} || result == vector<size_t>{1}));
      REQUIRE(graph.findSubgraphWithMaxEdges(2, algorithm) == vector<size_t>{0, 1});
    }

    {
      TriangleBoolSquareMatrix matrix(2);
      matrix.at(1, 0) = true;
      Graph graph(matrix);

      REQUIRE(graph.findSubgraphWithMaxEdges(0, algorithm).empty());
      auto result = graph.findSubgraphWithMaxEdges(1, algorithm);
      REQUIRE((result == vector<size_t>{0} || result == vector<size_t>{1}));
      REQUIRE(graph.findSubgraphWithMaxEdges(2, algorithm) == vector<size_t>{0, 1});
    }
  }

  SECTION("Test for K5") {
    TriangleBoolSquareMatrix matrix(5);
    matrix.at(1, 0) = true;
    matrix.at(2, 0) = true;
    matrix.at(2, 1) = true;
    matrix.at(3, 0) = true;
    matrix.at(3, 1) = true;
    matrix.at(3, 2) = true;
    matrix.at(4, 0) = true;
    matrix.at(4, 1) = true;
    matrix.at(4, 2) = true;
    matrix.at(4, 3) = true;
    Graph graph(matrix);

    auto i = GENERATE(range(1ul, 6ul));
    auto result = graph.findSubgraphWithMaxEdges(i, algorithm);
    REQUIRE(isSorted(result));
    REQUIRE(result.back() < 5);
    REQUIRE(result.size() == i);
  }

  SECTION("Test for sample graph 1") {
    /* Sample graph :
     *   0---|
     *   |   |
     *   1---2
     *   |
     *   3---4
     *
     * has matrix:
     * 0 1 1 0 0
     * 1 0 1 1 0
     * 1 1 0 0 0
     * 0 1 0 0 1
     * 0 0 0 1 0
     *
     *
     */

    TriangleBoolSquareMatrix matrix(5);
    matrix.at(1, 0) = true;
    matrix.at(2, 0) = true;
    matrix.at(2, 1) = true;
    matrix.at(3, 1) = true;
    matrix.at(4, 3) = true;
    Graph graph(matrix);

    REQUIRE(graph.findSubgraphWithMaxEdges(0, algorithm).empty());
    REQUIRE(graph.countEdgesInSubgraph(graph.findSubgraphWithMaxEdges(1, algorithm)) == 0);
    REQUIRE(graph.countEdgesInSubgraph(graph.findSubgraphWithMaxEdges(2, algorithm)) == 1); // for example {0, 1}
    REQUIRE(graph.countEdgesInSubgraph(graph.findSubgraphWithMaxEdges(3, algorithm)) == 3); // for example {0, 1, 2}
    REQUIRE(graph.countEdgesInSubgraph(graph.findSubgraphWithMaxEdges(4, algorithm)) == 4); // for example {0, 1, 2, 3}
    REQUIRE(
        graph.countEdgesInSubgraph(graph.findSubgraphWithMaxEdges(5, algorithm)) == 5); // for example {0, 1, 2, 3, 4}
  }

  SECTION("Test for sample graph 2") {
    /* Sample graph :
     * |---2---|
     * |   |   |
     * 0---4---1
     * |   |   |
     * |---3---|
     *
     * has matrix:
     * 0 0 1 1 1
     * 0 0 1 1 1
     * 1 1 0 0 1
     * 1 1 0 0 1
     * 1 1 1 1 0
     *
     *
     */

    TriangleBoolSquareMatrix matrix(5);
    matrix.at(2, 0) = true;
    matrix.at(2, 1) = true;
    matrix.at(3, 0) = true;
    matrix.at(3, 1) = true;
    matrix.at(4, 0) = true;
    matrix.at(4, 1) = true;
    matrix.at(4, 2) = true;
    matrix.at(4, 3) = true;
    Graph graph(matrix);

    REQUIRE(graph.findSubgraphWithMaxEdges(0, algorithm).empty());
    REQUIRE(graph.countEdgesInSubgraph(graph.findSubgraphWithMaxEdges(1, algorithm)) == 0);
    REQUIRE(graph.countEdgesInSubgraph(graph.findSubgraphWithMaxEdges(2, algorithm)) == 1); // for example {0, 2}
    REQUIRE(graph.countEdgesInSubgraph(graph.findSubgraphWithMaxEdges(3, algorithm)) == 3); // for example {0, 2, 4}
    REQUIRE(graph.countEdgesInSubgraph(graph.findSubgraphWithMaxEdges(4, algorithm)) == 5); // for example {0, 1, 2, 4}
    REQUIRE(
        graph.countEdgesInSubgraph(graph.findSubgraphWithMaxEdges(5, algorithm)) == 8); // for example {0, 1, 2, 3, 4}
  }
}

TEST_CASE("Test equality of algorithms", "[Graph][long]") {
  for (size_t i = 2; i < 6; ++i) {
    vector<bool> str(i * (i - 1) / 2);
    do {
      TriangleBoolSquareMatrix matrix(i);
      size_t counter = 0;
      for (size_t j = 1; j < i; ++j) {
        for (size_t k = 0; k < j; ++k) {
          matrix.at(j, k) = str[counter];
          ++counter;
        }
      }

      Graph graph(matrix);
      for (size_t j = 0; j <= i; ++j) {
        auto a = graph.findSubgraphWithMaxEdges(j, Graph::AlgorithmType::STACK),
            b = graph.findSubgraphWithMaxEdges(j, Graph::AlgorithmType::RECURSION);
        REQUIRE(graph.countEdgesInSubgraph(a) == graph.countEdgesInSubgraph(b));
      }
    } while (nextBinaryString(str));
  }
}