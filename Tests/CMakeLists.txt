list(APPEND SOURCE_FILES CombinationTests.cpp
        ../combinatorialUtils.h ../combinatorialUtils.cpp)
list(APPEND SOURCE_FILES TriangleBoolSquareMatrixTests.cpp
        ../TriangleBoolSquareMatrix.h ../TriangleBoolSquareMatrix.cpp)
list(APPEND SOURCE_FILES GraphTests.cpp
        ../Graph.h ../Graph.cpp
        ../ParallelTreeSolution.h ../ParallelTreeSolution.cpp)
add_executable(CatchTests CatchMain.cpp ${SOURCE_FILES})

if (CMAKE_SYSTEM_NAME STREQUAL "Linux")
    if (CMAKE_SYSTEM_PROCESSOR STREQUAL "x86")
        target_link_libraries(CatchTests ${PROJECT_SOURCE_DIR}/ParallelTreeTemplate/lib/Linux_x86/libParallelTree.so)
    elseif (CMAKE_SYSTEM_PROCESSOR STREQUAL "x86_64")
        target_link_libraries(CatchTests ${PROJECT_SOURCE_DIR}/ParallelTreeTemplate/lib/Linux_x86_64/libParallelTree.so)
    endif ()
endif ()