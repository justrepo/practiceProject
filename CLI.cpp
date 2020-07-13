//
// Created by nikita on 7/13/20.
//

#include "CLI.h"
#include "ParallelTreeSolution.h"

#include <fstream>
#include <algorithm>
#include <filesystem>

using std::cin;
using std::cout;
using std::ios_base;
using std::endl;
using std::numeric_limits;
using std::streamsize;
using std::swap;
using std::ifstream;
using std::ofstream;
using std::exception;
using std::runtime_error;

namespace fs = std::filesystem;
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

void CLI::printUsage() {
  cout << "Usage: " << programPath << " { <command> [<arguments>] [<options>] | help <command> }\n"
       << " where command can be \"interactive\", \"solve\", \"time\", \"matrix\" or \"dir\"\n"
       << "  interactive  start interactive mode; has no arguments nor options\n"
       << "  solve        solve specified problem\n"
       << "  time         calculate time of solution computation\n"
       << "  matrix       print matrix of solution computation time; has no arguments nor options\n"
       << "  dir          solve all problems in specified dir, for every problem output solution and time of computation"
       << endl;
}

void CLI::printUsageOfInteractive() {
  cout << "start interactive mode\n"
       << "Usage: " << programPath << " interactive" << endl;
}

void CLI::printUsageOfSolve() {
  cout << "solve specified problem\n"
       << "Usage: " << programPath << " solve PATH VERTICES [<options>]\n"
       << "  where:\n"
       << "    PATH      path to adjacency matrix of graph, by default matrix expected to be in binary format\n"
       << "    VERTICES  number of vertices in subgraph\n"
       << "  options:\n"
       << "    -t            read matrix in text format\n"
       << "    -a ALGORITHM  solve using specified algorithm, by default stack is used\n"
       << "                    ALGORITHM can be 1 for recursion, 2 for stack, 3 for parallel tree\n"
       << "    -p N          use N threads for parallel tree algorithm, by default numbers of CPU cores is used"
       << endl;
}

void CLI::printUsageOfTime() {
  cout << "calculate time of solution computation\n"
       << "Usage: " << programPath << " time PATH VERTICES [<options>]\n"
       << "  where:\n"
       << "    PATH      path to adjacency matrix of graph, by default matrix expected to be in binary format\n"
       << "    VERTICES  number of vertices in subgraph\n"
       << "  options:\n"
       << "    -t            read matrix in text format\n"
       << "    -a ALGORITHM  solve using specified algorithm, by default stack is used\n"
       << "                    ALGORITHM can be 1 for recursion, 2 for stack, 3 for parallel tree\n"
       << "    -p N          use N threads for parallel tree algorithm, by default numbers of CPU cores is used\n"
       << "    -n N          repeat computation N times and find mean time" << endl;
}

void CLI::printUsageOfMatrix() {
  cout << "print matrix of solution computation time\n"
       << "Usage: " << programPath << " matrix" << endl;
}

void CLI::printUsageOfDir() {
  cout << "solve all problems in specified directory, for every problem output solution and time of computation\n"
       << "Usage: " << programPath << " dir PATH\n"
       << "  where PATH - path to directory"
       << "every file in specified directory must be in format:\n"
       << "  1 line       1 for recursion, 2 for stack, 3 for parallel tree, for parallel tree next number - number of threads\n"
       << "  2 line       number of vertices in subgraph\n"
       << "  other lines  adjacency matrix of graph\n"
       << "for every file with name FILENAME generates file with name FILENAME_result \n"
       << "in which the 1 line has the solution and the 2 line - the average time for 10 starts" << endl;
}

void printInfo(const TriangleBoolSquareMatrix &matrix) {
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

void interactive() {
  cin.exceptions(ios_base::failbit | ios_base::badbit);

  TriangleBoolSquareMatrix matrix;
  Graph graph(matrix);

  printInfo(matrix);
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

          cin.ignore(numeric_limits<streamsize>::max(), '\n');
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
            sort(vertices.begin(), vertices.end());
            vertices.erase(unique(vertices.begin(), vertices.end()), vertices.end());

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
            sort(vertices.begin(), vertices.end());
            vertices.erase(unique(vertices.begin(), vertices.end()), vertices.end());

            cout << "Subgraph with vertices " << vertices << " has " << graph.countEdgesInSubgraph(vertices)
                 << " edges" << endl;
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

            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            input = static_cast<char>(cin.get());

            size_t numberOfThreads = 0;
            if (input == '2') {
              cout << "Enter number of threads >";
              cin >> numberOfThreads;
            }

            Graph::AlgorithmType type;
            if (input == '1') {
              type = Graph::AlgorithmType::RECURSION;
            } else {
              type = Graph::AlgorithmType::STACK;
            }

            vector<size_t> vertices;
            auto timeBeforeStart = timer::now();
            if (input == '2') {
              vertices = findSubgraphWithMaxEdgesUsingParallelTree(n, graph, numberOfThreads);
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

    printInfo(matrix);
    if (input != '\n') {
      cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
    input = static_cast<char>(cin.get());
  }
}

size_t CLI::parseArgument(size_t number, const string &name) {
  string arg = arguments[number];
  try {
    return stoul(arg);
  } catch (const exception &e) {
    throw runtime_error(arg + " isn't a valid number for " + name);
  }
}

pair<vector<size_t>, size_t> solveProblem(const TriangleBoolSquareMatrix &newMatrix, size_t numberOfVertices,
                                          size_t numberOfRepeats, Graph::AlgorithmType type, bool parallelTree,
                                          size_t numberOfThreads) {
  Graph graph(newMatrix);

  size_t result = 0;
  vector<size_t> vertices;
  for (size_t i = 0; i < numberOfRepeats; ++i) {
    auto timeBeforeStart = timer::now();
    if (parallelTree) {
      vertices = findSubgraphWithMaxEdgesUsingParallelTree(numberOfVertices, graph, numberOfThreads);
    } else {
      vertices = graph.findSubgraphWithMaxEdges(numberOfVertices, type);
    }
    auto timeAfterEnd = timer::now();
    result += size_t((timeAfterEnd - timeBeforeStart).count());
  }

  return {vertices, result / numberOfRepeats};
}

void CLI::solveOrTime(bool time) {
  try {
    size_t numberOfVertices;
    string path;
    if (arguments.size() < 4) {
      throw runtime_error((string("Not enough arguments for ") + (time ? "time" : "solve")));
    } else {
      path = arguments[2];
      numberOfVertices = parseArgument(3, "number of vertices");
    }

    bool binary = true;
    size_t numberOfThreads = 0, repeats = 1;
    bool parallelTree = false;
    Graph::AlgorithmType type = Graph::AlgorithmType::STACK;

    for (size_t i = 4; i < arguments.size(); ++i) {
      if (arguments[i] == "-t") {
        binary = false;
      } else if (arguments[i] == "-a") {
        if (i == arguments.size() - 1) {
          throw runtime_error("Missed argument for -a");
        } else {
          ++i;
          string arg = arguments[i];
          if (arg == "1") {
            type = Graph::AlgorithmType::RECURSION;
          } else if (arg == "2") {
            type = Graph::AlgorithmType::STACK;
          } else if (arg == "3") {
            parallelTree = true;
          } else {
            throw runtime_error("Wrong algorithm " + arg);
          }
        }
      } else if (arguments[i] == string("-p")) {
        if (i == arguments.size() - 1) {
          throw runtime_error("Missed argument for -p");
        } else {
          ++i;
          numberOfThreads = parseArgument(i, "number of threads");
        }
      } else if (arguments[i] == string("-n")) {
        if (time) {
          if (i == arguments.size() - 1) {
            throw runtime_error("Missed argument for -n");
          } else {
            ++i;
            repeats = parseArgument(i, "number of repeats");
          }
        } else {
          throw runtime_error(arguments[i] + " is wrong argument for solve");
        }
      } else {
        throw runtime_error(arguments[i] + " is wrong argument for " + (time ? "time" : "solve"));
      }
    }

    ifstream in(path);
    TriangleBoolSquareMatrix matrix;
    if (binary) {
      matrix.readFromStream(in);
    } else {
      matrix.readFromStreamFull(in);
    }

    const auto &[vertices, result] = solveProblem(matrix, numberOfVertices, repeats, type, parallelTree,
                                                  numberOfThreads);

    if (time) {
      cout << result << "ns" << endl;
    } else {
      cout << vertices << endl;
    }
  } catch (const exception &e) {
    cout << e.what() << endl;
    if (time) {
      printUsageOfTime();
    } else {
      printUsageOfSolve();
    }
  }
}

size_t measureTimeForVerticesNum(size_t algorithm, size_t verticesNum, size_t numberOfMatrices,
                                 size_t numberOfExperiments, TriangleBoolSquareMatrix &matrix) {
  size_t result = 0;
  for (size_t j = 0; j < numberOfMatrices; ++j) {
    matrix.randomInit();
    Graph graph(matrix);

    vector<size_t> vertices;
    for (size_t k = 0; k < numberOfExperiments; ++k) {
      auto timeBeforeStart = timer::now();
      if (algorithm == 1) {
        vertices = graph.findSubgraphWithMaxEdges(verticesNum, Graph::AlgorithmType::STACK);
      } else if (algorithm == 2) {
        vertices = graph.findSubgraphWithMaxEdges(verticesNum, Graph::AlgorithmType::RECURSION);
      } else if (algorithm == 3) {
        vertices = findSubgraphWithMaxEdgesUsingParallelTree(verticesNum, graph, 1);
      } else {
        vertices = findSubgraphWithMaxEdgesUsingParallelTree(verticesNum, graph);
      }
      auto timeAfterEnd = timer::now();
      result += size_t((timeAfterEnd - timeBeforeStart).count());
    }
  }
  return result / numberOfMatrices / numberOfExperiments;
}

void printComputationTimeMatrix(size_t algorithm, ofstream &out) {
  size_t maxN = 24;
  out << "{";
  for (size_t i = 0; i < maxN; ++i) {
    TriangleBoolSquareMatrix matrix;
    matrix.setDimension(i);
    out << "{";
    for (size_t verticesNum = 0; verticesNum <= i; ++verticesNum) {
      out << measureTimeForVerticesNum(algorithm, verticesNum, 10, (i < 15 ? 200 : i < 100 ? 10 : 50),
                                       matrix);
      if (verticesNum != i) {
        out << ", ";
      }
    }
    out << "}";
    if (i != maxN - 1) {
      out << "," << endl;
    }
  }
  out << "}" << endl;
}

CLI::CLI(vector<string> newArguments) : arguments(move(newArguments)) {
  programPath = arguments[0];
}

void CLI::run() {
  if (arguments.size() == 1) {
    interactive();
  } else {
    string command = arguments[1];

    if (command == "help") {
      if (arguments.size() == 3) {
        string requiredCommand = arguments[2];
        if (requiredCommand == "interactive") {
          printUsageOfInteractive();
        } else if (requiredCommand == "solve") {
          printUsageOfSolve();
        } else if (requiredCommand == "time") {
          printUsageOfTime();
        } else if (requiredCommand == "matrix") {
          printUsageOfMatrix();
        } else if (requiredCommand == "dir") {
          printUsageOfDir();
        } else {
          cout << "There isn't " << requiredCommand << " command" << endl;
        }
      } else {
        printUsage();
      }
    } else if (command == "interactive") {
      if (arguments.size() == 2) {
        interactive();
      } else {
        printUsageOfInteractive();
      }
    } else if (command == "solve") {
      solveOrTime(false);
    } else if (command == "time") {
      solveOrTime(true);
    } else if (command == "matrix") {
      if (arguments.size() == 2) {
        string fileNames[] = {"", "stackMatrix", "recursionMatrix", "parallelTree1ThreadMatrix",
                              "parallelTree4ThreadMatrix"};
        for (size_t algorithm = 1; algorithm < 5; ++algorithm) {
          ofstream out(fileNames[algorithm]);
          printComputationTimeMatrix(algorithm, out);
        }
      } else {
        printUsageOfMatrix();
      }
    } else if (command == "dir") {
      if (arguments.size() == 3) {
        for (const auto &entry:fs::directory_iterator(arguments[2])) {
          ifstream in(entry.path());
          size_t numberOfThreads = 0, numberOfVertices;
          bool parallelTree = false;
          Graph::AlgorithmType type = Graph::AlgorithmType::STACK;

          {
            auto algorithm = in.get();
            if (algorithm == '1') {
              type = Graph::AlgorithmType::RECURSION;
            } else if (algorithm == '2') {
              type = Graph::AlgorithmType::STACK;
            } else if (algorithm == '3') {
              parallelTree = true;

              string s;
              getline(in, s);
              if (!s.empty()) {
                auto numPos = s.find_first_not_of(" \t");
                if (numPos != string::npos) {
                  try {
                    numberOfThreads = stoul(s.substr(numPos));
                  } catch (const exception &e) {
                    numberOfThreads = 0;
                  }
                }
              }
            }
          }

          in >> numberOfVertices;

          TriangleBoolSquareMatrix matrix;
          matrix.readFromStreamFull(in);

          const auto &[vertices, result] = solveProblem(matrix, numberOfVertices, 10, type, parallelTree,
                                                        numberOfThreads);

          ofstream out(entry.path().string() + "_result");
          out << vertices << endl << result << endl;
        }
      } else {
        printUsageOfDir();
      }
    } else {
      cout << "There isn't " << command << " command" << endl;
      printUsage();
    }
  }
}
