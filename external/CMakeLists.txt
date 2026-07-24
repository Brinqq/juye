set(juye_extern_include_dirs)
set(juye_extern_library_dirs ${CMAKE_LIBRARY_OUTPUT_DIRECTORY})
set(juye_extern_libs)

if(APPLE)
  set(juye_extern_static_ext "a")
  set(juye_extern_shared_ext "dylib")
endif()

if(UNIX)
  set(juye_extern_static_ext "a")
  set(juye_extern_shared_ext "so")
endif()

if(WIN32)
  set(juye_extern_static_ext "lib")
  set(juye_extern_shared_ext "dll")
endif()

#yaml-cpp
set(YAML_BUILD_SHARED_LIBS OFF)
add_subdirectory(yaml-cpp)
list(APPEND juye_extern_include_dirs yaml-cpp/include)
list(APPEND juye_extern_libs yaml-cpp)

#glm
add_subdirectory(glm)
list(APPEND juye_extern_include_dirs glm)
list(APPEND juye_extern_libs glm)

#bk
add_subdirectory(bcl/src)
list(APPEND juye_extern_include_dirs bcl/src/include)
list(APPEND juye_extern_libs bk)

#stb
# list(APPEND juye_extern_include_dirs stb/) #its in the root wtf
list(APPEND juye_extern_include_dirs ${CMAKE_CURRENT_SOURCE_DIR})


#assimp
# add_subdirectory(assimp)
# list(APPEND juye_extern_include_dirs assimp/include) 
# list(APPEND juye_extern_libs assimpd)

#user dependant externs
#vulkan
if(NOT DEFINED ENV{VULKAN_SDK})
  message(FATAL_ERROR "Unable to find vulkan sdk path, set ENV:VULKAN_SDK=<path>")
endif()

list(APPEND juye_extern_include_dirs $ENV{VULKAN_SDK}/Include)
list(APPEND juye_extern_library_dirs $ENV{VULKAN_SDK}/lib)
list(APPEND juye_extern_libs vulkan-1)


if(APPLE OR UNIX)
  #glfw
  add_subdirectory(glfw)
  list(APPEND juye_extern_include_dirs glfw/include)
  list(APPEND juye_extern_libs glfw3)

  #frameworks
  list(APPEND ${juye_extern_libs} "-framework Metal" "-framework IOKit" "-framework Cocoa" "-framework QuartzCore")
endif()

target_link_directories(${PROJECT_NAME} PUBLIC ${juye_extern_library_dirs})
target_include_directories(${PROJECT_NAME} PUBLIC ${juye_extern_include_dirs})
  target_link_libraries(${PROJECT_NAME} PUBLIC ${juye_extern_libs})
