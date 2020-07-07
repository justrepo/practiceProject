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

TEST_CASE("Test number of combinations", "[numberOfCombinations]") {
  REQUIRE(numberOfCombinations(0, 0) == 1);

  REQUIRE(numberOfCombinations(1, 0) == 1);
  REQUIRE(numberOfCombinations(1, 1) == 1);

  REQUIRE(numberOfCombinations(2, 0) == 1);
  REQUIRE(numberOfCombinations(2, 1) == 2);
  REQUIRE(numberOfCombinations(2, 2) == 1);

  REQUIRE(numberOfCombinations(3, 0) == 1);
  REQUIRE(numberOfCombinations(3, 1) == 3);
  REQUIRE(numberOfCombinations(3, 2) == 3);
  REQUIRE(numberOfCombinations(3, 3) == 1);

  REQUIRE(numberOfCombinations(4, 0) == 1);
  REQUIRE(numberOfCombinations(4, 1) == 4);
  REQUIRE(numberOfCombinations(4, 2) == 6);
  REQUIRE(numberOfCombinations(4, 3) == 4);
  REQUIRE(numberOfCombinations(4, 4) == 1);

  REQUIRE(numberOfCombinations(5, 0) == 1);
  REQUIRE(numberOfCombinations(5, 1) == 5);
  REQUIRE(numberOfCombinations(5, 2) == 10);
  REQUIRE(numberOfCombinations(5, 3) == 10);
  REQUIRE(numberOfCombinations(5, 4) == 5);
  REQUIRE(numberOfCombinations(5, 5) == 1);

}

TEST_CASE("Test combination of collection", "[combination]") {
  SECTION("Test for empty collection") {
    vector<size_t> elements;

    REQUIRE_FALSE(nextCombination(elements, 0));
    REQUIRE(elements.empty());

    REQUIRE_FALSE(nextCombination(elements, 1));
    REQUIRE(elements.empty());

    REQUIRE_FALSE(nextCombination(elements, 2));
    REQUIRE(elements.empty());
  }SECTION("Test for collection with one element") {
    vector<size_t> elements{0};

    REQUIRE_FALSE(nextCombination(elements, 0));
    elements = {0};

    REQUIRE_FALSE(nextCombination(elements, 1));
    elements = {0};

    REQUIRE(nextCombination(elements, 2));
    REQUIRE(elements.size() == 1);
    REQUIRE(elements.back() == 1);
    REQUIRE_FALSE(nextCombination(elements, 2));
    REQUIRE(elements.size() == 1);
    REQUIRE(elements.back() == 2);
    elements = {0};

    REQUIRE(nextCombination(elements, 3));
    REQUIRE(elements.size() == 1);
    REQUIRE(elements.back() == 1);
    REQUIRE(nextCombination(elements, 3));
    REQUIRE(elements.size() == 1);
    REQUIRE(elements.back() == 2);
    REQUIRE_FALSE(nextCombination(elements, 3));
    REQUIRE(elements.size() == 1);
    REQUIRE(elements.back() == 3);
  }SECTION("Test for all collections with 2 to 10 elements") {
    auto n = GENERATE(range(2ul, 11ul));
    auto k = GENERATE_COPY(range(1ul, n + 1));
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
}