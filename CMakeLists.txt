cmake_minimum_required(VERSION 3.10)
project(btopk)

set(CMAKE_CXX_STANDARD 14)

### add qhull
include_directories(/usr/local/include/libqhullcpp)
### qhullstatic reentrant library
add_library(libqhullstatic_r STATIC IMPORTED)
set_property(TARGET libqhullstatic_r PROPERTY IMPORTED_LOCATION /usr/local/lib/${CMAKE_STATIC_LIBRARY_PREFIX}qhullstatic_r${CMAKE_STATIC_LIBRARY_SUFFIX})
add_dependencies(libqhullstatic_r qhull)
### qhullcpp library
add_library(libqhullcpp STATIC IMPORTED)
set_property(TARGET libqhullcpp PROPERTY IMPORTED_LOCATION /usr/local/lib/${CMAKE_STATIC_LIBRARY_PREFIX}qhullcpp${CMAKE_STATIC_LIBRARY_SUFFIX})
set_property(TARGET libqhullcpp PROPERTY INTERFACE_LINK_LIBRARIES libqhullstatic_r)
add_dependencies(libqhullcpp qhull)

include_directories(${CMAKE_SOURCE_DIR}/lib/rtree)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3")


add_executable(btopk main.cpp utils.cpp utils.h ch.cpp ch.h qhull_adapter.h qhull_adapter.cpp rtree_adapter.h rtree_adapter.cpp
        lib/rtree/collection.h
        lib/rtree/collection.cpp
        lib/rtree/filemem.h
        lib/rtree/filemem.cpp
        lib/rtree/global.h
        lib/rtree/global.cpp
        lib/rtree/header.h
        lib/rtree/hypercube.h
        lib/rtree/hypercube.cpp
        lib/rtree/mem.h
        lib/rtree/param.h
        lib/rtree/param.cpp
        lib/rtree/point.h
        lib/rtree/point.cpp
        lib/rtree/rentry.h
        lib/rtree/rentry.cpp
        lib/rtree/rnode.h
        lib/rtree/rnode.cpp
        lib/rtree/rtree.h
        lib/rtree/rtree.cpp
        lib/rtree/tgs.h
        lib/rtree/tgs.cpp
        lib/rtree/virtualRNode.h
        lib/rtree/virtualRNode.cpp)

target_link_libraries(${PROJECT_NAME} PRIVATE libqhullcpp)
