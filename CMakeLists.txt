cmake_minimum_required(VERSION 3.0)

project(faceDetection VERSION 1.0.0 LANGUAGES CXX)

set(FACEDETECTION_LIBSDIR ${CMAKE_CURRENT_SOURCE_DIR}/lib)

set(Boost_DEBUG ON)
set(Boost_ARCHITECTURE      -x32)
set(Boost_USE_STATIC_RUNTIME ON)
set(Boost_USE_STATIC_LIBS    ON)
set(Boost_USE_DEBUG_LIBS     OFF)
set(Boost_USE_RELEASE_LIBS   ON)
set(Boost_USE_MULTITHREADED  ON)
set(BOOST_ROOT ${FACEDETECTION_LIBSDIR}/boost)

# add_definitions(-DBOOST_LOG_DYN_LINK)
find_package(Boost 1.76 REQUIRED COMPONENTS system container program_options thread log)
include_directories(${Boost_INCLUDE_DIRS})
if (NOT Boost_FOUND)
    message(FATAL_ERROR "Boost library not found in '${BOOST_ROOT}'")    
endif()

add_subdirectory(project/faceDet)
add_executable(faceDetection project/main.cpp project/app/app.h project/app/app.cpp)

target_link_libraries(faceDetection PRIVATE faceDet PUBLIC ${Boost_LIBRARIES})