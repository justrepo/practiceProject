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

TEST_CASE("Test findSubgraphWithMaxEdges", "[Graph]") {
  SECTION("Test for graph with 2 vertices") {
    {
      TriangleBoolSquareMatrix matrix(2);
      Graph graph(matrix);

      REQUIRE(graph.findSubgraphWithMaxEdges(0).empty());
      auto result = graph.findSubgraphWithMaxEdges(1);
      REQUIRE((result == vector<size_t>{0} || result == vector<size_t>{1}));
      REQUIRE(graph.findSubgraphWithMaxEdges(2) == vector<size_t>{0, 1});
    }

    {
      TriangleBoolSquareMatrix matrix(2);
      matrix.at(1, 0) = true;
      Graph graph(matrix);

      REQUIRE(graph.findSubgraphWithMaxEdges(0).empty());
      auto result = graph.findSubgraphWithMaxEdges(1);
      REQUIRE((result == vector<size_t>{0} || result == vector<size_t>{1}));
      REQUIRE(graph.findSubgraphWithMaxEdges(2) == vector<size_t>{0, 1});
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
    auto result = graph.findSubgraphWithMaxEdges(i);
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

    REQUIRE(graph.findSubgraphWithMaxEdges(0).empty());
    REQUIRE(graph.countEdgesInSubgraph(graph.findSubgraphWithMaxEdges(1)) == 0);
    REQUIRE(graph.countEdgesInSubgraph(graph.findSubgraphWithMaxEdges(2)) == 1); // for example {0, 1}
    REQUIRE(graph.countEdgesInSubgraph(graph.findSubgraphWithMaxEdges(3)) == 3); // for example {0, 1, 2}
    REQUIRE(graph.countEdgesInSubgraph(graph.findSubgraphWithMaxEdges(4)) == 4); // for example {0, 1, 2, 3}
    REQUIRE(graph.countEdgesInSubgraph(graph.findSubgraphWithMaxEdges(5)) == 5); // for example {0, 1, 2, 3, 4}
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

    REQUIRE(graph.findSubgraphWithMaxEdges(0).empty());
    REQUIRE(graph.countEdgesInSubgraph(graph.findSubgraphWithMaxEdges(1)) == 0);
    REQUIRE(graph.countEdgesInSubgraph(graph.findSubgraphWithMaxEdges(2)) == 1); // for example {0, 2}
    REQUIRE(graph.countEdgesInSubgraph(graph.findSubgraphWithMaxEdges(3)) == 3); // for example {0, 2, 4}
    REQUIRE(graph.countEdgesInSubgraph(graph.findSubgraphWithMaxEdges(4)) == 5); // for example {0, 1, 2, 4}
    REQUIRE(graph.countEdgesInSubgraph(graph.findSubgraphWithMaxEdges(5)) == 8); // for example {0, 1, 2, 3, 4}
  }
}