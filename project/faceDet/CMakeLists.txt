cmake_minimum_required(VERSION 3.0)

project(faceDet VERSION 1.0)

set(OpenCV_STATIC ON)
set(OpenCV_DIR ${FACEDETECTION_LIBSDIR}/opencv)
find_package(OpenCV REQUIRED)

add_library(faceDet SHARED faceDet.cpp)
set(faceDet_INCLUDE_DIRS ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories(faceDet PUBLIC ${OpenCV_INCLUDE_DIRS} ${faceDet_INCLUDE_DIRS}/..)
target_link_libraries(faceDet PRIVATE ${OpenCV_LIBS})

if(NOT EXISTS ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/haarcascade_frontalface_alt2.xml) 
    file(COPY project/faceDet/haarcascade_frontalface_alt2.xml DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
endif()

install(TARGETS faceDet
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})