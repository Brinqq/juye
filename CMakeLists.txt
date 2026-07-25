cmake_minimum_required(VERSION 4.0.0)
project(juye)

if(${CMAKE_CXX_STANDARD} LESS 17)
  message(FATAL_ERROR "juye minimum supported cxx standard is c++17")
endif()

file(MAKE_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/data)

option(JUYE_FORCE_RELEASE OFF)

set(BUILD_SHARED_LIBS OFF)
set(runtime_data ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/data/)
set(atexture_directory ${root}/data/textures/)

# configure_file(
#   "${CMAKE_CURRENT_SOURCE_DIR}/src/core/configuration/build_generation.h.in"
#   "${CMAKE_CURRENT_SOURCE_DIR}/src/core/configuration/build_generation.h"
# )

add_subdirectory(external)


add_subdirectory(samples)
add_subdirectory(src)

