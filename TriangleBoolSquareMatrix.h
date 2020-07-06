//
// Created by nikita on 7/6/20.
//
#pragma once

#include <vector>
#include <iostream>

using std::vector;
using std::ostream;
using std::endl;

class TriangleBoolSquareMatrix {
  size_t n;
  vector<bool> data;
public:
  explicit TriangleBoolSquareMatrix(size_t n);

  [[nodiscard]] bool at(size_t i, size_t j) const;

  [[nodiscard]] bool unsafeAt(size_t i, size_t j) const;

  std::vector<bool>::reference at(size_t i, size_t j);

  std::vector<bool>::reference unsafeAt(size_t i, size_t j);

  friend ostream &operator<<(ostream &out, const TriangleBoolSquareMatrix &matrix);

  ostream &printTriangle(ostream &out) const;

  ostream &printFull(ostream &out) const;

  ostream &printTriangleSubmatrix(ostream &out, const vector<size_t> &indexes) const;

  ostream &printFullSubmatrix(ostream &out, const vector<size_t> &indexes) const;
};