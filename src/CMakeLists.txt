cmake_minimum_required(VERSION 4.0.0)
project(ssf)

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_BINARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

message(${CMAKE_CURRENT_SOURCE_DIR})

set(files
  "${CMAKE_CURRENT_SOURCE_DIR}/gfx/gfx.cpp"
)

set(includes)
set(libs)
set(lib_directories)
set(include_directories)

add_library(${PROJECT_NAME} ${files})
target_include_directories(${PROJECT_NAME} PRIVATE ${include_directories})
target_link_LIBRARIES(${PROJECT_NAME} PRIVATE ${libs})
target_link_directories(${PROJECT_NAME} PRIVATE ${lib_directories})
