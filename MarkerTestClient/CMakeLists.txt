cmake_minimum_required(VERSION 2.8)
project(markerclient)
SET(CMAKE_MODULE_PATH ${CMAKE_INSTALL_PREFIX}/lib/cmake/ )
SET(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} "-std=c++11 -pthread")

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/../libraries/fifo ${CMAKE_CURRENT_BINARY_DIR}/fifo_build)

add_executable(markerclient src/markerclient.cpp)
target_link_libraries(markerclient FIFO)
