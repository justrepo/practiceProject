//
// Created by nikita on 7/10/20.
//

#include "ParallelTreeSolution.h"
#include "ParallelTreeTemplate/include/ParallelTree.hpp"

using std::unique_ptr;
using std::make_unique;

class CombinationRecord : public Record {
public:
  const Graph &graph;
  vector<size_t> vertices;

  explicit CombinationRecord(const Graph &graph) : graph(graph) {}

  virtual ~CombinationRecord() = default;

  explicit CombinationRecord(const Graph &graph, vector<size_t> vertices) : graph(graph), vertices(move(vertices)) {}

  [[nodiscard]] bool betterThan(const Record &other) const override {
    return graph.countEdgesInSubgraph(vertices) >
           graph.countEdgesInSubgraph(dynamic_cast<const CombinationRecord &>(other).vertices);
  }

  [[nodiscard]] unique_ptr<Record> clone() const override {
    return make_unique<CombinationRecord>(graph, vertices);
  }
};

class CombinationTreeNode : public Node {
public:
  const Graph &graph;
  vector<size_t> vertices;
  size_t level = 0;

  virtual ~CombinationTreeNode() = default;

  CombinationTreeNode(const Graph &graph, size_t k) : graph(graph), vertices(k) {}

  CombinationTreeNode(const Graph &graph, vector<size_t> vertices, size_t level) : graph(graph),
                                                                                   vertices(move(vertices)),
                                                                                   level(level) {}

  vector<unique_ptr<Node>> process(Record &record) override {
    if (vertices.empty()) {
      return vector<unique_ptr<Node>>();
    } else {
      if (level > vertices.size() - 1) {
        auto &recordCast = dynamic_cast<CombinationRecord &>(record);
        if (recordCast.vertices.empty() ||
            graph.countEdgesInSubgraph(vertices) > graph.countEdgesInSubgraph(recordCast.vertices)) {
          recordCast.vertices = move(vertices);
        }
        return vector<unique_ptr<Node>>();
      } else {
        if (level > 0) {
          vertices[level] = vertices[level - 1] + 1;
        }
        vector<unique_ptr<Node>> childNodes;
        childNodes.reserve(graph.getMatrix().getDimension() - vertices.size() + level - vertices[level] + 1);

        childNodes.push_back(make_unique<CombinationTreeNode>(graph, vertices, level + 1));
        while (vertices[level] < graph.getMatrix().getDimension() - vertices.size() + level) {
          ++vertices[level];

          childNodes.push_back(make_unique<CombinationTreeNode>(graph, vertices, level + 1));
        }

        return childNodes;
      }
    }
  }

  [[nodiscard]] bool hasHigherPriority(const Node &other) const override {
    return this > &other;
  }
};

vector<size_t> findSubgraphWithMaxEdgesUsingParallelTree(size_t targetVerticesNumber, const Graph &graph,
                                                         size_t threads) {
  CombinationRecord initialRecord(graph);
  unique_ptr<Record> bestSolution = parallelTree(make_unique<CombinationTreeNode>(graph, targetVerticesNumber),
                                                 initialRecord, static_cast<unsigned int>(threads));
  return dynamic_cast<const CombinationRecord *>(bestSolution.get())->vertices;
}
