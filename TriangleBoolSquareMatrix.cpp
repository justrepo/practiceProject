//
// Created by nikita on 7/6/20.
//

#include "TriangleBoolSquareMatrix.h"
#include <stdexcept>

using std::out_of_range;
using std::to_string;

TriangleBoolSquareMatrix::TriangleBoolSquareMatrix(size_t n) : n(n), data(n * (n - 1) / 2) {}

bool TriangleBoolSquareMatrix::at(size_t i, size_t j) const {
  if (i > 0 && i < n && j < i) {
    return data[i * (i - 1) / 2 + j];
  } else {
    throw out_of_range(to_string(i) + ", " + to_string(j) + " is out of range of triangle square matrix of " +
                       to_string(n) + " dimension");
  }
}

bool TriangleBoolSquareMatrix::unsafeAt(size_t i, size_t j) const {
  return data[i * (i - 1) / 2 + j];
}

std::vector<bool>::reference TriangleBoolSquareMatrix::at(size_t i, size_t j) {
  if (i > 0 && i < n && j < i) {
    return data[i * (i - 1) / 2 + j];
  } else {
    throw out_of_range(to_string(i) + ", " + to_string(j) + " is out of range of triangle square matrix of " +
                       to_string(n) + " dimension");
  }
}

std::vector<bool>::reference TriangleBoolSquareMatrix::unsafeAt(size_t i, size_t j) {
  return data[i * (i - 1) / 2 + j];
}

ostream &operator<<(ostream &out, const TriangleBoolSquareMatrix &matrix) {
  for (size_t i = 1; i < matrix.n; ++i) {
    for (size_t j = 0; j < i; ++j) {
      out << matrix.at(i, j) << " ";
    }
    out << endl;
  }
  return out;
}

ostream &TriangleBoolSquareMatrix::printTriangle(ostream &out) const {
  return out << *this;
}

ostream &TriangleBoolSquareMatrix::printFull(ostream &out) const {
  for (size_t i = 0; i < n; ++i) {
    for (size_t j = 0; j < n; ++j) {
      if (i == j) {
        out << 0 << " ";
      } else if (j > i) {
        out << at(j, i) << " ";
      } else {
        out << at(i, j) << " ";
      }
    }
    out << endl;
  }

  return out;
}

ostream &TriangleBoolSquareMatrix::printFullSubmatrix(ostream &out, const vector<size_t> &indexes) const {
  for (size_t i = 0; i < indexes.size(); ++i) {
    for (size_t j = 0; j < indexes.size(); ++j) {
      if (i == j) {
        out << 0 << " ";
      } else if (j > i) {
        out << at(indexes[j], indexes[i]) << " ";
      } else {
        out << at(indexes[i], indexes[j]) << " ";
      }
    }
    out << endl;
  }

  return out;
}

ostream &TriangleBoolSquareMatrix::printTriangleSubmatrix(ostream &out, const vector<size_t> &indexes) const {
  for (size_t i = 1; i < indexes.size(); ++i) {
    for (size_t j = 0; j < i; ++j) {
      out << at(indexes[i], indexes[j]) << " ";
    }
    out << endl;
  }

  return out;
}


