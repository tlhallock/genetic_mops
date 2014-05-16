cmake_minimum_required (VERSION 2.6)

set(GeneticMop_VERSION_MAJOR 1)
set(GeneticMop_VERSION_MINOR 0)

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wall -march=native")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -Wall -march=native")

# Representer library
file (GLOB lib_SRC
	${CMAKE_CURRENT_SOURCE_DIR}/../src/*.cc
	${CMAKE_CURRENT_SOURCE_DIR}/../src/image/*.cc
	${CMAKE_CURRENT_SOURCE_DIR}/../src/represent/*.cc
	${CMAKE_CURRENT_SOURCE_DIR}/../src/solver/*.cc)
add_library(Mops ${lib_SRC})

# Test application library
file(GLOB application_SRC
	${CMAKE_CURRENT_SOURCE_DIR}/../src/tests/*.cc)
add_executable(TestMops ${application_SRC})



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




