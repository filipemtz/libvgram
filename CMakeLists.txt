
cmake_minimum_required(VERSION 2.8.9)
project(libvgram)

set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")

include_directories(include)

file(GLOB SRC_FILES "src/*.cpp")
add_library(vgram SHARED ${SRC_FILES})

install(DIRECTORY include/ DESTINATION /usr/include/vgram)
install(TARGETS vgram RUNTIME DESTINATION /usr/bin LIBRARY DESTINATION /usr/lib ARCHIVE DESTINATION /usr/lib)

add_subdirectory(example)

