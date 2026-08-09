cmake_minimum_required(VERSION 4.0.0)
project(juye)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_CXX_STANDARD 20)


set(CMAKE_CXX_STANDARD 20)

# if(${CMAKE_CXX_STANDARD} LESS 17)
#   message(FATAL_ERROR "juye minimum supported cxx standard is c++17")
# endif()

file(MAKE_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/data)
option(JUYE_FORCE_RELEASE OFF)


if(APPLE)
  # Both required at top-level scope. OBJCXX alone makes CMake feed .m files
  # (GLFW's cocoa backend) to the ObjC++ compiler, where C++ pointer rules
  # break them. With OBJC enabled, .m -> OBJC and .mm -> OBJCXX as expected.
  enable_language(OBJC)
  enable_language(OBJCXX) # layer_adapter.mm
endif()


set(BUILD_SHARED_LIBS OFF)
set(runtime_data ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/data/)
set(atexture_directory ${root}/data/textures/)

# configure_file(
#   "${CMAKE_CURRENT_SOURCE_DIR}/src/core/configuration/build_generation.h.in"
#   "${CMAKE_CURRENT_SOURCE_DIR}/src/core/configuration/build_generation.h"
# )

#shader compilation
set(SLANGC ${slang_SOURCE_DIR}/bin/slangc${_exe})

add_subdirectory(extern)
add_subdirectory(dev)
add_subdirectory(samples)
add_subdirectory(src)

