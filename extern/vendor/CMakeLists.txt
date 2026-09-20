if(NOT TARGET stb)
  add_library(stb INTERFACE)
  target_include_directories(stb INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/stb)
endif()

if(NOT TARGET metal-cpp)
add_library(metal-cpp INTERFACE)
target_include_directories(metal-cpp INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/apple/metal-cpp)
endif()

if(NOT TARGET vulkan)
add_library(vulkan INTERFACE)
target_include_directories(vulkan INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/vulkan/include)
endif()
