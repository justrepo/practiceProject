//
// Created by nikita on 7/7/20.
//
#include "combinatorialUtils.h"

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
