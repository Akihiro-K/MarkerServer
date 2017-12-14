cmake_minimum_required(VERSION 2.8)
project(multimarkerserver)
SET(CMAKE_MODULE_PATH ${CMAKE_INSTALL_PREFIX}/lib/cmake/ )
SET(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} "-std=c++11 -pthread")

find_package(raspicam REQUIRED)
find_package(OpenCV REQUIRED)
find_package(aruco REQUIRED)
find_package(Eigen3 REQUIRED)
include_directories(${EIGEN3_INCLUDE_DIR})

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/../libraries/arucowrapper ${CMAKE_CURRENT_BINARY_DIR}/arucowrapper_build)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/../libraries/tcp ${CMAKE_CURRENT_BINARY_DIR}/tcp_build)

add_executable(multimarkerserver src/multimarkerserver.cpp)
target_link_libraries(multimarkerserver  ${aruco_LIBS} ${OpenCV_LIBS} ${raspicam_CV_LIBS} ArucoWrapper TCPServer)
