cmake_minimum_required(VERSION 3.0)

project(facedetection VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fvisibility=hidden -fvisibility-inlines-hidden")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fvisibility=hidden")

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

set(FACEDETECTION_LIBSDIR ${CMAKE_CURRENT_SOURCE_DIR}/lib)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${FACEDETECTION_LIBSDIR}/lib/cmake")

set(OpenCV_STATIC ON)
set(OpenCV_DIR ${FACEDETECTION_LIBSDIR})
find_package(OpenCV REQUIRED)

add_subdirectory(project/faceDet)
add_executable(facedetection project/main.cpp project/app/app.h project/app/app.cpp)

target_include_directories(facedetection PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/lib/include)
target_link_directories(facedetection PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/lib/lib)
target_link_libraries(facedetection PRIVATE ${OpenCV_LIBS} jsoncpp faceDet)

enable_testing()
add_subdirectory(test)

if(NOT EXISTS ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/testsamples) 
    file(COPY testsamples DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
endif()