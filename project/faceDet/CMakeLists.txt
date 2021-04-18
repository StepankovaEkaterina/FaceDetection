cmake_minimum_required(VERSION 3.0)

project(faceDet VERSION 1.0)

set(OpenCV_STATIC ON)
set(OpenCV_DIR ${FACEDETECTION_LIBSDIR}/opencv)
find_package(OpenCV REQUIRED)

add_library(faceDet SHARED faceDet.cpp)
set(faceDet_INCLUDE_DIRS ${CMAKE_CURRENT_SOURCE_DIR})
set_target_properties(faceDet PROPERTIES PUBLIC_HEADER ${faceDet_INCLUDE_DIRS}/faceDet.h LINK_FLAGS "-Wl,--no-undefined")
target_include_directories(faceDet PUBLIC ${OpenCV_INCLUDE_DIRS} ${faceDet_INCLUDE_DIRS}/..)
target_link_libraries(faceDet PUBLIC ${OpenCV_LIBS})
install(TARGETS faceDet
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})