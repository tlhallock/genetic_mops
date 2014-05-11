cmake_minimum_required (VERSION 2.6)

set(Representer_VERSION_MAJOR 1)
set(Representer_VERSION_MINOR 0)

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wall -march=native")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -Wall -march=native")

# Represnter library
add_library(Representer 
	${CMAKE_CURRENT_SOURCE_DIR}/../src/InitialSet.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/../src/initial_set_generator.cc
	${CMAKE_CURRENT_SOURCE_DIR}/../src/GeneticRepresenter.cpp 
	${CMAKE_CURRENT_SOURCE_DIR}/../src/heuristcs/sort_representer.cc 
	${CMAKE_CURRENT_SOURCE_DIR}/../src/heuristcs/greedy.cc 
	${CMAKE_CURRENT_SOURCE_DIR}/../src/heuristcs/random_sample_representer.cc 
	${CMAKE_CURRENT_SOURCE_DIR}/../src/heuristcs/breadth_first_quad_rep.cc 
	${CMAKE_CURRENT_SOURCE_DIR}/../src/plotter.cc
	${CMAKE_CURRENT_SOURCE_DIR}/../src/backtracker.cc
	${CMAKE_CURRENT_SOURCE_DIR}/../src/metrics/Scalarization.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/../src/metrics/Epsilon.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/../src/metrics/MaximizeDists.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/../src/metrics/DistToClosest.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/../src/metrics/Variation.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/../src/metrics/RepresentationMetric.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/../src/metrics/Delta.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/../src/maths_utils.cc)
target_link_libraries(Representer QTree)


# Test application library
add_executable(TestRepresenter
	${CMAKE_CURRENT_SOURCE_DIR}/../src/main.cc
	${CMAKE_CURRENT_SOURCE_DIR}/../src/test.cc)
target_link_libraries(TestRepresenter Representer)

#Depends on QTree
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../../out/lib)

add_library(QTree STATIC IMPORTED)
set_property(TARGET QTree PROPERTY IMPORTED_LOCATION ${CMAKE_CURRENT_SOURCE_DIR}/../../out/lib/libQTree.a)

target_link_libraries(Representer QTree)
target_link_libraries(TestRepresenter QTree)





set (CMAKE_INSTALL_PREFIX ${CMAKE_CURRENT_SOURCE_DIR}/../../out)

install (TARGETS Representer DESTINATION lib)
install (TARGETS TestRepresenter DESTINATION bin)
