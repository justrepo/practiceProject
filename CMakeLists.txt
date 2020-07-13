cmake_minimum_required(VERSION 3.17)
project(graphTest)

set(CMAKE_CXX_STANDARD 17)

add_executable(graphTest main.cpp Graph.cpp Graph.h TriangleBoolSquareMatrix.cpp TriangleBoolSquareMatrix.h combinatorialUtils.h combinatorialUtils.cpp ParallelTreeSolution.h ParallelTreeSolution.cpp CLI.cpp CLI.h)

if (NOT CMAKE_CXX_FLAGS STREQUAL "")
    string(REPLACE " " ";" CMAKE_CXX_FLAGS_LIST ${CMAKE_CXX_FLAGS})
endif ()
list(APPEND CMAKE_CXX_FLAGS_LIST -Wno-unknown-pragmas -Wno-trigraphs) # -Wno-unknown-pragmas to avoid warnings to pragma ide
list(JOIN CMAKE_CXX_FLAGS_LIST " " CMAKE_CXX_FLAGS)

if (NOT CMAKE_CXX_FLAGS_RELEASE STREQUAL "")
    string(REPLACE " " ";" CMAKE_CXX_FLAGS_RELEASE_LIST ${CMAKE_CXX_FLAGS_RELEASE})
endif ()
list(APPEND CMAKE_CXX_FLAGS_RELEASE_LIST -O3)
list(JOIN CMAKE_CXX_FLAGS_RELEASE_LIST " " CMAKE_CXX_FLAGS_RELEASE)

if (NOT CMAKE_CXX_FLAGS_DEBUG STREQUAL "")
    string(REPLACE " " ";" CMAKE_CXX_FLAGS_DEBUG_LIST ${CMAKE_CXX_FLAGS_DEBUG})
endif ()
list(APPEND CMAKE_CXX_FLAGS_DEBUG_LIST
        -Wall
        -Wextra
        -Wpedantic

        -Wcast-align
        -Wcast-qual
        -Wconversion
        -Wctor-dtor-privacy
        -Wduplicated-branches
        -Wduplicated-cond
        -Wextra-semi
        -Wfloat-equal
        -Wlogical-op
        -Wnon-virtual-dtor
        -Wold-style-cast
        -Woverloaded-virtual
        -Wredundant-decls
        -Wsign-conversion
        -Wsign-promo)
list(JOIN CMAKE_CXX_FLAGS_DEBUG_LIST " " CMAKE_CXX_FLAGS_DEBUG)

add_subdirectory(Tests)
if (CMAKE_SYSTEM_NAME STREQUAL "Linux")
    if (CMAKE_SYSTEM_PROCESSOR STREQUAL "x86")
        target_link_libraries(${PROJECT_NAME} ${PROJECT_SOURCE_DIR}/ParallelTreeTemplate/lib/Linux_x86/libParallelTree.so)
    elseif (CMAKE_SYSTEM_PROCESSOR STREQUAL "x86_64")
        target_link_libraries(${PROJECT_NAME} ${PROJECT_SOURCE_DIR}/ParallelTreeTemplate/lib/Linux_x86_64/libParallelTree.so)
    endif ()
endif ()