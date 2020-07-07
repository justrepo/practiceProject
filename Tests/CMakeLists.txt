list(APPEND SOURCE_FILES CombinationTests.cpp
        ../combinatorialUtils.h ../combinatorialUtils.cpp)
list(APPEND SOURCE_FILES TriangleBoolSquareMatrixTests.cpp
        ../TriangleBoolSquareMatrix.h ../TriangleBoolSquareMatrix.cpp)
list(APPEND SOURCE_FILES GraphTests.cpp
        ../Graph.h ../Graph.cpp)
add_executable(CatchTests CatchMain.cpp ${SOURCE_FILES})