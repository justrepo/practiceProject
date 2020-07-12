#include <algorithm>
#include <fstream>
#include <chrono>
#include "Graph.h"
#include "ParallelTreeSolution.h"

using namespace std;
using timer = std::chrono::high_resolution_clock;

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

TriangleBoolSquareMatrix matrix;

void printInfo() {
  cout << "Graph matrix:" << endl;
  if (matrix.getDimension() == 0) {
    cout << "empty" << endl;
  } else {
    matrix.printFull(cout);
  }

  cout << "Actions:\n"
       << "0 - exit\n"
       << "1 - change matrix\n"
       << "2 - load matrix from file\n"
       << "3 - save matrix to file\n"
       << "4 - print matrix of subgraph\n"
       << "5 - count edges in subgraph in current graph\n"
       << "6 - find subgraph with maximal number of edges" << endl;
}

void printUsage(const string &programPath) {
  cout << "Usage: " << programPath
       << " matrixFileName numberOfVertices [algorithm] [numberOfRepeats] [matrixInTextForm]\n" <<
       " where algorithm can be 1 for recursion, 2 for stack, 3 for parallel tree, stack is set by default" << endl;
}

int main(int argc, char **argv) {
  /*ifstream in("/home/nikita/Projects/CLionProjects/graphTest/cmake-build-debug/somename");
  matrix.readFromStream(in);
  for (size_t verticesNum = 0; verticesNum <= 20; ++verticesNum) {
    Graph graph(matrix);

    size_t result = 0;
    vector<size_t> vertices;
    for (size_t i = 0; i < 10; ++i) {
      auto timeBeforeStart = timer::now();
      vertices = graph.findSubgraphWithMaxEdges(verticesNum, Graph::AlgorithmType::SOMETHING);
      auto timeAfterEnd = timer::now();
      result += size_t((timeAfterEnd - timeBeforeStart).count()) / 10;
    }
    cout << log(result) << ", ";
  }
  return 0;*/
  if (argc == 1) {
    cin.exceptions(ios_base::failbit | ios_base::badbit);

    Graph graph(matrix);

    printInfo();
    char input = static_cast<char>(cin.get());
    while (input != '0') {
      try {
        switch (input) {
          case '1':
            cout << "Choice action:\n"
                 << "1 - change matrix dimension\n"
                 << "2 - change element of matrix\n"
                 << "3 - randomly init matrix\n"
                 << "else - cancel"
                 << endl;

            cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            input = static_cast<char>(cin.get());

            switch (input) {
              case '1': {
                size_t newDimension;
                cout << "Enter matrix new dimension >";
                cin >> newDimension;
                matrix.setDimension(newDimension);
                graph.setMatrix(matrix);
              }
                break;
              case '2': {
                size_t i, j;
                cout << "Enter element position >";
                cin >> i >> j;
                if (j > i) {
                  swap(i, j);
                }
                if (i > matrix.getDimension()) {
                  cout << i << " > " << matrix.getDimension() << " is out of range" << endl;
                } else if (i == j) {
                  cout << "Simple graph contains no loops" << endl;
                } else {
                  matrix.at(i, j) = !matrix.at(i, j);
                  graph.setMatrix(matrix);
                }
              }
                break;
              case '3':
                matrix.randomInit();
                graph.setMatrix(matrix);
                break;
              default:
                break;
            }
            break;
          case '2': {
            cout << "Enter file name >";
            string filename;
            cin >> filename;
            ifstream in(filename);
            if (in) {
              matrix.readFromStreamFull(in);
              graph.setMatrix(matrix);
            } else {
              cout << "Something wrong with file" << endl;
            }
          }
            break;
          case '3': {
            cout << "Enter file name >";
            string filename;
            cin >> filename;
            ofstream out(filename);
            if (out) {
              matrix.writeToStreamFull(out);
            } else {
              cout << "Something wrong with file" << endl;
            }
          }
            break;
          case '4': {
            vector<size_t> vertices;
            cout << "Enter number of vertices in subgraph >";
            size_t n;
            cin >> n;
            if (n > matrix.getDimension()) {
              cout << "Subgraph cannot contain more vertices than the graph" << endl;
            } else {
              vertices.resize(n);

              cout << "Enter vertices numbers >";
              for (size_t i = 0; i < n; ++i) {
                cin >> vertices[i];
              }
              std::sort(vertices.begin(), vertices.end());
              vertices.erase(std::unique(vertices.begin(), vertices.end()), vertices.end());

              cout << "Subgraph with vertices " << vertices << " has matrix:" << endl;
              matrix.printFullSubmatrix(cout, vertices);
            }
          }
            break;
          case '5': {
            vector<size_t> vertices;
            cout << "Enter number of vertices in subgraph >";
            size_t n;
            cin >> n;
            if (n > matrix.getDimension()) {
              cout << "Subgraph cannot contain more vertices than the graph" << endl;
            } else {
              vertices.resize(n);

              cout << "Enter vertices numbers >";
              for (size_t i = 0; i < n; ++i) {
                cin >> vertices[i];
              }
              std::sort(vertices.begin(), vertices.end());
              vertices.erase(std::unique(vertices.begin(), vertices.end()), vertices.end());

              cout << "Subgraph with vertices " << vertices << " has " << graph.countEdgesInSubgraph(vertices)
                   << " edges"
                   << endl;
            }
          }
            break;
          case '6': {
            cout << "Enter number of vertices in subgraph >";
            size_t n;
            cin >> n;
            if (n > matrix.getDimension()) {
              cout << "Subgraph cannot contain more vertices than the graph" << endl;
            } else {
              cout << "Choice algorithm:\n"
                   << "1 - recursion\n"
                   << "2 - parallel tree\n"
                   << "else - stack" << endl;

              cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
              input = static_cast<char>(cin.get());

              Graph::AlgorithmType type;
              if (input == '1') {
                type = Graph::AlgorithmType::RECURSION;
              } else {
                type = Graph::AlgorithmType::STACK;
              }

              vector<size_t> vertices;
              auto timeBeforeStart = timer::now();
              if (input == '2') {
                vertices = findSubgraphWithMaxEdgesUsingParallelTree(n, graph);
              } else {
                vertices = graph.findSubgraphWithMaxEdges(n, type);
              }
              auto timeAfterEnd = timer::now();
              cout << "Finished in " << (timeAfterEnd - timeBeforeStart).count() << "ns = "
                   << double((timeAfterEnd - timeBeforeStart).count()) * 1e-9 << "s" << endl;
              cout << "A subgraph with " << n << " vertices in the graph contains no more than "
                   << graph.countEdgesInSubgraph(vertices) << " edges" << endl;
              cout << "For example, subgraph with vertices " << vertices << " contains "
                   << graph.countEdgesInSubgraph(vertices) << " edges" << endl;
            }
          }
            break;
          default:
            break;
        }
      } catch (const ios_base::failure &e) {
        cout << "IO error: " << e.what() << endl;
        cin.clear();
      }

      printInfo();
      if (input != '\n') {
        cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
      }
      input = static_cast<char>(cin.get());
    }
  } else if (argc == 2 || argc > 6) {
    printUsage(argv[0]);
  } else {
    string filename = argv[1], arg2 = argv[2];
    size_t verticesNum, repeatsNum = 1;
    bool binaryFile = true;
    try {
      verticesNum = stoul(arg2);
    } catch (const exception &e) {
      cout << arg2 << " isn't a valid number" << endl;
      printUsage(argv[0]);
      return 0;
    }
    Graph::AlgorithmType type = Graph::AlgorithmType::STACK;
    bool parallelTree = false;
    if (argc >= 4) {
      string arg3 = argv[3];
      if (arg3 == "1") {
        type = Graph::AlgorithmType::RECURSION;
      } else if (arg3 == "2") {
        type = Graph::AlgorithmType::STACK;
      } else if (arg3 == "3") {
        parallelTree = true;
      } else {
        cout << arg3 << " isn't a valid algorithm" << endl;
        printUsage(argv[0]);
        return 0;
      }
    }
    if (argc >= 5) {
      string arg4 = argv[4];
      try {
        repeatsNum = stoul(arg4);
      } catch (const exception &e) {
        cout << arg4 << " isn't a valid number" << endl;
        printUsage(argv[0]);
        return 0;
      }
    }
    if (argc == 6) {
      binaryFile = false;
    }
    ifstream in(filename);
    if (in) {
      if (binaryFile) {
        matrix.readFromStream(in);
      } else {
        matrix.readFromStreamFull(in);
      }
      if (verticesNum > matrix.getDimension()) {
        cout << "numberOfVertices can't be more that number of vertices in graph" << endl;
        printUsage(argv[0]);
        return 0;
      }
      Graph graph(matrix);

      size_t result = 0;
      vector<size_t> vertices;
      for (size_t i = 0; i < repeatsNum; ++i) {
        auto timeBeforeStart = timer::now();
        if (parallelTree) {
          vertices = findSubgraphWithMaxEdgesUsingParallelTree(verticesNum, graph);
        } else {
          vertices = graph.findSubgraphWithMaxEdges(verticesNum, type);
        }
        auto timeAfterEnd = timer::now();
        result += size_t((timeAfterEnd - timeBeforeStart).count()) / repeatsNum;
      }
      ofstream out("subgraph");
      out << vertices;
      ofstream out1("time");
      out1 << size_t(result) << "ns";
    } else {
      cout << "Something wrong with file \"" << filename << "\"" << endl;
    }
  }
  return 0;
}
