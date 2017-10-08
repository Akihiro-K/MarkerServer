cmake_minimum_required(VERSION 2.8)
project(markerserver)
SET(CMAKE_MODULE_PATH ${CMAKE_INSTALL_PREFIX}/lib/cmake/ )
SET(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} "-std=c++11 -pthread")

find_package(raspicam REQUIRED)
find_package(OpenCV REQUIRED)
find_package(aruco REQUIRED)
find_package(Eigen3 REQUIRED)
include_directories(EIGEN3_INCLUDE_DIR)

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/../libraries/arucowrapper ${CMAKE_CURRENT_BINARY_DIR}/arucowrapper_build)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/../libraries/serial ${CMAKE_CURRENT_BINARY_DIR}/serial_build)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/../libraries/utserial ${CMAKE_CURRENT_BINARY_DIR}/utserial_build)

add_executable(markerserver src/markerserver.cpp)
target_link_libraries(markerserver  ${aruco_LIBS} ${OpenCV_LIBS} ${raspicam_CV_LIBS} ArucoWrapper UTSerial Serial)