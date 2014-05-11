cmake_minimum_required (VERSION 2.6)

set(QTree_VERSION_MAJOR 1)
set(QTree_VERSION_MINOR 0)

add_library(QTree
	${CMAKE_CURRENT_SOURCE_DIR}/../src/QtreeLeaf.cc
	${CMAKE_CURRENT_SOURCE_DIR}/../src/qtree_mop.cc
	${CMAKE_CURRENT_SOURCE_DIR}/../src/QtreeBranch.cc
	${CMAKE_CURRENT_SOURCE_DIR}/../src/qtree_point.cc
	${CMAKE_CURRENT_SOURCE_DIR}/../src/Qtree.cc)
	
add_executable(TestQTree
	${CMAKE_CURRENT_SOURCE_DIR}/../src/main.cc
	${CMAKE_CURRENT_SOURCE_DIR}/../src/qtree_test_utils.cc)
target_link_libraries(TestQTree QTree)

set (CMAKE_INSTALL_PREFIX ${CMAKE_CURRENT_SOURCE_DIR}/../../out)

install (TARGETS QTree DESTINATION lib)
install (TARGETS TestQTree DESTINATION bin)
