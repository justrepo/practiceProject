//
// Created by nikita on 7/7/20.
//

#include <catch2/catch.hpp>
#include "../TriangleBoolSquareMatrix.h"
#include <sstream>

using std::stringstream;

// Consider as axiom that writeToStreamTriangle works correctly
TEST_CASE("Test constructor", "[TriangleBoolSquareMatrix]") {
  SECTION("Constructor without arguments") {
    TriangleBoolSquareMatrix matrix;
    stringstream stream;
    matrix.writeToStreamTriangle(stream);
    REQUIRE(stream.str() == "0\n");
  }

  SECTION("Constructor with 0 dimension") {
    TriangleBoolSquareMatrix matrix(0);
    stringstream stream;
    matrix.writeToStreamTriangle(stream);
    REQUIRE(stream.str() == "0\n");
  }

  SECTION("Constructor with 1 dimension") {
    TriangleBoolSquareMatrix matrix(1);
    stringstream stream;
    matrix.writeToStreamTriangle(stream);
    REQUIRE(stream.str() == "1\n");
  }

  SECTION("Constructor with 2 dimension") {
    TriangleBoolSquareMatrix matrix(2);
    stringstream stream;
    matrix.writeToStreamTriangle(stream);
    REQUIRE(stream.str() == "2\n"
                            "0 \n");
  }

  SECTION("Constructor with 3 dimension") {
    TriangleBoolSquareMatrix matrix(3);
    stringstream stream;
    matrix.writeToStreamTriangle(stream);
    REQUIRE(stream.str() == "3\n"
                            "0 \n"
                            "0 0 \n");
  }

  SECTION("Constructor with 10 dimension") {
    TriangleBoolSquareMatrix matrix(10);
    stringstream stream;
    matrix.writeToStreamTriangle(stream);
    REQUIRE(stream.str() == "10\n"
                            "0 \n"
                            "0 0 \n"
                            "0 0 0 \n"
                            "0 0 0 0 \n"
                            "0 0 0 0 0 \n"
                            "0 0 0 0 0 0 \n"
                            "0 0 0 0 0 0 0 \n"
                            "0 0 0 0 0 0 0 0 \n"
                            "0 0 0 0 0 0 0 0 0 \n");
  }
}

// I have no idea how to test randomInit, so I just assume it works fine

TEST_CASE("Test getDimension", "[TriangleBoolSquareMatrix]") {
  {
    TriangleBoolSquareMatrix matrix;
    REQUIRE(matrix.getDimension() == 0);
  }
  {
    TriangleBoolSquareMatrix matrix(0);
    REQUIRE(matrix.getDimension() == 0);
  }
  {
    TriangleBoolSquareMatrix matrix(1);
    REQUIRE(matrix.getDimension() == 1);
  }
  {
    TriangleBoolSquareMatrix matrix(2);
    REQUIRE(matrix.getDimension() == 2);
  }
  {
    TriangleBoolSquareMatrix matrix(3);
    REQUIRE(matrix.getDimension() == 3);
  }
  {
    TriangleBoolSquareMatrix matrix(10);
    REQUIRE(matrix.getDimension() == 10);
  }
}

TEST_CASE("Test indexation", "[TriangleBoolSquareMatrix]") {
  // I won't test exceptions, assuming that all algorithms never go out of range
  SECTION("Indexation in 2 dimension matrix") {
    TriangleBoolSquareMatrix matrix(2);

    REQUIRE_FALSE(matrix.at(1, 0));

    matrix.at(1, 0) = true;

    stringstream stream;
    matrix.writeToStreamTriangle(stream);
    REQUIRE(stream.str() == "2\n"
                            "1 \n");

    REQUIRE(matrix.at(1, 0));
  }

  SECTION("Indexation in 5 dimension matrix") {
    TriangleBoolSquareMatrix matrix(5);

    // rather strange, but CLion warns that
    // "Variable 'i' used in loop condition is not updated in the loop"
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wfor-loop-analysis"
    for (size_t i = 1; i < 5; ++i) {
      for (size_t j = 0; j < i; ++j) {
        REQUIRE_FALSE(matrix.at(i, j));
      }
    }
#pragma clang diagnostic pop

    matrix.at(1, 0) = true;

    stringstream stream;
    matrix.writeToStreamTriangle(stream);
    REQUIRE(stream.str() == "5\n"
                            "1 \n"
                            "0 0 \n"
                            "0 0 0 \n"
                            "0 0 0 0 \n");

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wfor-loop-analysis"
    for (size_t i = 1; i < 5; ++i) {
      for (size_t j = 0; j < i; ++j) {
        if (i == 1 && j == 0) {
          REQUIRE(matrix.at(i, j));
        } else {
          REQUIRE_FALSE(matrix.at(i, j));
        }
      }
    }
#pragma clang diagnostic pop

    matrix.at(3, 1) = true;

    stream.str("");
    matrix.writeToStreamTriangle(stream);
    REQUIRE(stream.str() == "5\n"
                            "1 \n"
                            "0 0 \n"
                            "0 1 0 \n"
                            "0 0 0 0 \n");

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wfor-loop-analysis"
    for (size_t i = 1; i < 5; ++i) {
      for (size_t j = 0; j < i; ++j) {
        if ((i == 1 && j == 0) || (i == 3 && j == 1)) {
          REQUIRE(matrix.at(i, j));
        } else {
          REQUIRE_FALSE(matrix.at(i, j));
        }
      }
    }
#pragma clang diagnostic pop
  }
}

TEST_CASE("Test operator==", "[TriangleBoolSquareMatrix]") {
  SECTION("Test for empty matrices") {
    TriangleBoolSquareMatrix matrix, matrix1;
    TriangleBoolSquareMatrix matrix2(0);
    TriangleBoolSquareMatrix matrix3(1);
    TriangleBoolSquareMatrix matrix4(5);
    matrix4.randomInit();

    REQUIRE(matrix == matrix1);
    REQUIRE(matrix == matrix2);
    REQUIRE_FALSE(matrix == matrix3);
    REQUIRE_FALSE(matrix == matrix4);
  }

  SECTION("Test for 2 dimension matrix") {
    TriangleBoolSquareMatrix matrix(2), matrix1(1);
    TriangleBoolSquareMatrix matrix2(2), matrix3(2);
    TriangleBoolSquareMatrix matrix4(5);

    REQUIRE_FALSE(matrix == matrix1);
    REQUIRE(matrix == matrix2);
    REQUIRE(matrix == matrix3);
    REQUIRE_FALSE(matrix == matrix4);

    matrix.at(1, 0) = true;

    REQUIRE_FALSE(matrix == matrix1);
    REQUIRE_FALSE(matrix == matrix2);
    REQUIRE_FALSE(matrix == matrix3);
    REQUIRE_FALSE(matrix == matrix4);

    matrix2.at(1, 0) = true;

    REQUIRE_FALSE(matrix == matrix1);
    REQUIRE(matrix == matrix2);
    REQUIRE_FALSE(matrix == matrix3);
    REQUIRE_FALSE(matrix == matrix4);
  }
}

TEST_CASE("Test writeToStream and readFromStream", "[TriangleBoolSquareMatrix]") {
  // I'll test only for stringstream, assuming that other streams works the same
  SECTION("Test for empty matrices") {
    TriangleBoolSquareMatrix matrix, matrix1;
    stringstream out;
    matrix.writeToStream(out);
    stringstream in(out.str());
    matrix1.readFromStream(in);
    REQUIRE(matrix == matrix1);
  }

  SECTION("Test for 2 dimension matrices") {
    TriangleBoolSquareMatrix matrix(2), matrix1;
    REQUIRE_FALSE(matrix == matrix1);

    stringstream out;
    matrix.writeToStream(out);
    stringstream in(out.str());
    matrix1.readFromStream(in);
    REQUIRE(matrix == matrix1);

    matrix.at(1, 0) = true;
    TriangleBoolSquareMatrix matrix2;
    REQUIRE_FALSE(matrix == matrix2);
    out.str("");
    matrix.writeToStream(out);
    in.str(out.str());
    matrix2.readFromStream(in);
    REQUIRE(matrix == matrix2);
    REQUIRE(matrix2.at(1, 0));
  }

  SECTION("Test for random 10 dimension matrices") {
    TriangleBoolSquareMatrix matrix(10), matrix1(10);
    matrix.randomInit();
    while (matrix == matrix1) {
      matrix.randomInit();
    }
    REQUIRE_FALSE(matrix == matrix1);

    stringstream out;
    matrix.writeToStream(out);
    stringstream in(out.str());
    matrix1.readFromStream(in);
    REQUIRE(matrix == matrix1);
  }
}