//
// Created by nikita on 7/7/20.
//

#include "catch2/catch.hpp"
#include "../combinatorialUtils.h"

#include <set>

using std::set;

bool isSorted(const vector<size_t> &elements) {
  if (elements.size() < 2) {
    return true;
  } else {
    auto it = elements.begin();
    auto lastElement = *it;
    ++it;
    for (; it != elements.end(); ++it) {
      if (lastElement >= *it) {
        return false;
      }
      lastElement = *it;
    }
    return true;
  }
}

TEST_CASE("Test isSorted", "[isSorted][utils]") {
  SECTION("Empty list is sorted") {
    REQUIRE(isSorted({}));
  }

  SECTION("List with one element is always sorted") {
    REQUIRE(isSorted({1}));
    REQUIRE(isSorted({0}));
  }

  SECTION("Test for lists with two elements") {
    REQUIRE(isSorted({0, 1}));
    REQUIRE(isSorted({0, 2}));
    REQUIRE(isSorted({1, 10}));

    REQUIRE_FALSE(isSorted({1, 0}));
    REQUIRE_FALSE(isSorted({1, 1}));
    REQUIRE_FALSE(isSorted({10, 1}));
    REQUIRE_FALSE(isSorted({0, 0}));
  }

  SECTION("Test for lists with three elements") {
    REQUIRE(isSorted({0, 1, 2}));
    REQUIRE(isSorted({0, 2, 3}));
    REQUIRE(isSorted({0, 10, 12}));

    REQUIRE(isSorted({1, 2, 3}));
    REQUIRE(isSorted({11, 19, 212}));

    REQUIRE_FALSE(isSorted({0, 1, 1}));
    REQUIRE_FALSE(isSorted({0, 0, 1}));
    REQUIRE_FALSE(isSorted({0, 1, 0}));

    REQUIRE_FALSE(isSorted({10, 1, 12}));
  }
}


TEST_CASE("test combination of 2 to 10 elements", "[combination]") {
  auto n = GENERATE(range(2ul, 10ul));
  auto k = GENERATE_COPY(range(1ul, n));
  vector<size_t> elements(k);
  iota(elements.begin(), elements.end(), 0);

  set<vector<size_t>> uniqueCombinations;
  do {
    REQUIRE(isSorted(elements));
    REQUIRE(elements.size() == k);
    REQUIRE(elements.back() < n);
    REQUIRE(uniqueCombinations.insert(elements).second);
  } while (nextCombination(elements, n));

  REQUIRE(uniqueCombinations.size() == numberOfCombinations(n, k));
}