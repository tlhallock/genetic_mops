cmake_minimum_required (VERSION 2.6)

set(GeneticMop_VERSION_MAJOR 1)
set(GeneticMop_VERSION_MINOR 0)

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wall -march=native")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -Wall -march=native")

# Represnter library
add_library(Mops
	${CMAKE_CURRENT_SOURCE_DIR}/../src/utils.cc
	${CMAKE_CURRENT_SOURCE_DIR}/../src/math_functions.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/../src/MOP.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/../src/image/VectorImageSet.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/../src/image/RtreeImageSet.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/../src/solver/IncrementalSampleSolver.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/../src/solver/DepthFirstRecursiveStencil.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/../src/solver/RandomSampleSolver.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/../src/solver/GeneticSolver.cpp)

# Test application library
add_executable(TestMops
	${CMAKE_CURRENT_SOURCE_DIR}/../src/tests/compare_solvers.cc
	${CMAKE_CURRENT_SOURCE_DIR}/../src/tests/compare_dfs.cc
	${CMAKE_CURRENT_SOURCE_DIR}/../src/tests/TestImageSet.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/../src/GeneticMOP.cpp)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../../out/lib)

# Depends on QTree
add_library(QTree STATIC IMPORTED)
set_property(TARGET QTree PROPERTY IMPORTED_LOCATION ${CMAKE_CURRENT_SOURCE_DIR}/../../out/lib/libQTree.a)

# Depends on Representer
add_library(Representer STATIC IMPORTED)
set_property(TARGET Representer PROPERTY IMPORTED_LOCATION ${CMAKE_CURRENT_SOURCE_DIR}/../../out/lib/libRepresenter.a)

target_link_libraries(Mops Representer)
target_link_libraries(Mops QTree)

target_link_libraries(TestMops Representer)
target_link_libraries(TestMops QTree)
target_link_libraries(TestMops Mops)

set (CMAKE_INSTALL_PREFIX ${CMAKE_CURRENT_SOURCE_DIR}/../../out)

install (TARGETS Mops DESTINATION lib)
install (TARGETS TestMops DESTINATION bin)




