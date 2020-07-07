//
// Created by nikita on 7/7/20.
//
#include "combinatorialUtils.h"
#include <stdexcept>

using std::overflow_error;

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

size_t numberOfCombinations(size_t n, size_t k) {
  if (n == 0 || k > n) {
    return 0;
  }
  if (k == 0 || n == k) {
    return 1;
  }

  if (n > 27) {
    throw overflow_error("number of combinations may be too big");
  }

  if (k > n - k) {
    k = n - k;
  }

  size_t result = 1;
  for (size_t i = n; i > n - k; --i) {
    result *= i;
  }
  for (size_t i = 2; i <= k; ++i) {
    result /= i;
  }

  return result;
}
