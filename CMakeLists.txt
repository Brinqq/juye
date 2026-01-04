cmake_minimum_required(VERSION 4.0.0)
project(juye)

if(${CMAKE_CXX_STANDARD} LESS 14)
  message(FATAL_ERROR "juye minimum supported cxx standard is c++14")
endif()

file(MAKE_DIRECTORY ${CMAKE_BINARY_OUTPUT_DIRECTORY}/data)

option(JUYE_FORCE_RELEASE OFF)

set(BUILD_SHARED_LIBS OFF)
set(root ${CMAKE_CURRENT_SOURCE_DIR})
set(runtime_data ${CMAKE_BINARY_OUTPUT_DIRECTORY}/data/)
set(atexture_directory ${root}/data/textures/)

configure_file(
  "${CMAKE_CURRENT_SOURCE_DIR}/src/core/configuration/build_generation.h.in"
  "${CMAKE_CURRENT_SOURCE_DIR}/src/core/configuration/build_generation.h"
)

set(force_includes 
  # "-includecore/pch/assertions.h"
  # "-includecore/pch/error.h"
  # "-includecore/global.h"
)

set(files
  # systems

  # core
  "${CMAKE_CURRENT_SOURCE_DIR}/src/core/juye.cpp"
  "${CMAKE_CURRENT_SOURCE_DIR}/src/core/runtime_layer.cpp"
  "${CMAKE_CURRENT_SOURCE_DIR}/src/core/drivers/display_win32.cpp"
  #
  "${CMAKE_CURRENT_SOURCE_DIR}/src/core/drivers/device.cpp"
  "${CMAKE_CURRENT_SOURCE_DIR}/src/core/drivers/VK/vk_core.cpp"
  "${CMAKE_CURRENT_SOURCE_DIR}/src/core/drivers/VK/vk_helper.cpp"
  "${CMAKE_CURRENT_SOURCE_DIR}/src/core/drivers/VK/vkinternal.cpp"
  "${CMAKE_CURRENT_SOURCE_DIR}/src/core/drivers/VK/vk_wrappers.cpp"
  "${CMAKE_CURRENT_SOURCE_DIR}/src/core/drivers/VK/MemoryVK/MemoryVK.cpp"
  "${CMAKE_CURRENT_SOURCE_DIR}/src/core/drivers/VK/vkshader.cpp"
  "${CMAKE_CURRENT_SOURCE_DIR}/src/core/drivers/VK/vkresource.cpp"
  "${CMAKE_CURRENT_SOURCE_DIR}/src/core/drivers/VK/tmp.cpp"
  "${CMAKE_CURRENT_SOURCE_DIR}/src/core/drivers/VK/vkplatform.cpp"

  # "${CMAKE_CURRENT_SOURCE_DIR}/src/gk/gk.cpp"
  # "${CMAKE_CURRENT_SOURCE_DIR}/src/gk/actions.cpp"
  # "${CMAKE_CURRENT_SOURCE_DIR}/src/gk/model/prefabs.cpp"
  # "${CMAKE_CURRENT_SOURCE_DIR}/src/gk/assets/loader.cpp"
  # "${CMAKE_CURRENT_SOURCE_DIR}/src/gk/assets/registry.cpp"
  # "${CMAKE_CURRENT_SOURCE_DIR}/src/core/fsystem/image.cpp"
)


set(juye_internal_include_dirs
  ${root}/include
  ${root}/src
  ${root}/src/core/pch
)

set(juye_internal_libs)

if(WIN32)
  list(APPEND juye_internal_libs dwmapi)
endif()

add_library(${PROJECT_NAME} STATIC ${files})
target_include_directories(${PROJECT_NAME} PUBLIC ${juye_internal_include_dirs})
target_link_libraries(${PROJECT_NAME} PUBLIC ${juye_internal_libs})

if(NOT CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
  target_compile_options(${PROJECT_NAME} PRIVATE -include/core/pch/all.h)
else()
  target_compile_options(${PROJECT_NAME} PRIVATE /FI "${CMAKE_CURRENT_SOURCE_DIR}/src/core/pch/fi.h")
endif()

add_subdirectory(external)
