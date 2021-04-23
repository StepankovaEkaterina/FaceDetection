cmake_minimum_required(VERSION 3.0)

project(facedetection VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

set(FACEDETECTION_LIBSDIR ${CMAKE_CURRENT_SOURCE_DIR}/lib)

set(Boost_DEBUG ON)
set(Boost_USE_STATIC_RUNTIME ON)
set(Boost_USE_STATIC_LIBS    ON)
add_definitions(-DBOOST_ALL_NO_LIB)
set(BOOST_ROOT ${FACEDETECTION_LIBSDIR}/boost)
find_package(Boost 1.76 REQUIRED COMPONENTS system container program_options thread log)
include_directories(${Boost_INCLUDE_DIRS})

add_subdirectory(project/faceDet)
add_executable(facedetection project/main.cpp project/app/app.h project/app/app.cpp)

target_link_libraries(facedetection PRIVATE faceDet PUBLIC ${Boost_LIBRARIES})

enable_testing()
add_subdirectory(test)

if(NOT EXISTS ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/haarcascade_frontalface_alt2.xml) 
    file(COPY project/app/haarcascade_frontalface_alt2.xml DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
endif()

if(NOT EXISTS ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/testsamples) 
    file(COPY testsamples DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
endif()