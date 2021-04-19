cmake_minimum_required(VERSION 3.0)

project(faceDetection VERSION 1.0.0 LANGUAGES CXX)

set(FACEDETECTION_LIBSDIR ${CMAKE_CURRENT_SOURCE_DIR}/lib)

set(Boost_DEBUG ON)
set(Boost_USE_STATIC_LIBS ON)
set(BOOST_ROOT ${FACEDETECTION_LIBSDIR}/boost)
find_package(Boost 1.76 REQUIRED)
include_directories(${Boost_INCLUDE_DIRS})

add_subdirectory(project/faceDet)
add_executable(faceDetection project/main.cpp project/app/app.h project/app/app.cpp)

target_link_libraries(faceDetection PRIVATE faceDet PUBLIC ${Boost_LIBRARIES})