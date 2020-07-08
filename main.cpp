#include <algorithm>
#include "Graph.h"

using namespace std;

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
       << "2 - print matrix of subgraph\n"
       << "3 - count edges in subgraph in current graph\n"
       << "4 - find subgraph with maximal number of edges" << endl;
}

int main() {
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
        case '3': {
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

            cout << "Subgraph with vertices " << vertices << " has " << graph.countEdgesInSubgraph(vertices) << " edges"
                 << endl;
          }
        }
          break;
        case '4': {
          cout << "Enter number of vertices in subgraph >";
          size_t n;
          cin >> n;
          if (n > matrix.getDimension()) {
            cout << "Subgraph cannot contain more vertices than the graph" << endl;
          } else {
            auto vertices = graph.findSubgraphWithMaxEdges(n);
            cout << "A subgraph in the graph contains no more than " << graph.countEdgesInSubgraph(vertices) << " edges"
                 << endl;
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
  return 0;
}
