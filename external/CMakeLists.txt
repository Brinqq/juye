
include(FetchContent)

set(GLFW_BUILD_DOCS OFF)
set(GLFW_BUILD_TESTS OFF)
set(GLFW_BUILD_EXAMPLES OFF)
set(GLFW_INSTALL OFF)

FetchContent_Declare(
  glfw
  GIT_REPOSITORY https://github.com/glfw/glfw.git
  GIT_TAG 3.4
  SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/glfw
)

FetchContent_Declare(
  stb
  GIT_REPOSITORY https://github.com/nothings/stb.git
  GIT_TAG 31c1ad37456438565541f4919958214b6e762fb4
  SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/stb
)

FetchContent_Declare(
  metal-cpp
  GIT_REPOSITORY https://github.com/apple/metal-cpp.git
  GIT_TAG 27c4382b7151d55a51692cdcb27aaa98752240de
  SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/metal-cpp
)

set(YAML_CPP_BUILD_TESTS OFF)
set(YAML_CPP_BUILD_TOOLS OFF)
set(YAML_CPP_INSTALL OFF)

FetchContent_Declare(
  yaml-cpp
  GIT_REPOSITORY https://github.com/jbeder/yaml-cpp.git
  GIT_TAG yaml-cpp-0.9.0
  SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/yaml-cpp
)

set(GLM_BUILD_TESTS OFF)
set(GLM_BUILD_INSTALL OFF)

FetchContent_Declare(
  glm
  GIT_REPOSITORY https://github.com/g-truc/glm.git
  GIT_TAG 1.0.3
  SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/glm
)

set(ASSIMP_WARNINGS_AS_ERRORS OFF CACHE BOOL "" FORCE)
set(ASSIMP_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(ASSIMP_BUILD_ASSIMP_TOOLS OFF CACHE BOOL "" FORCE)
set(ASSIMP_INSTALL OFF CACHE BOOL "" FORCE)
# Vendored zlib breaks under new Xcode clang (predefined TARGET_OS_MAC trips
# a classic-MacOS fdopen hack in contrib/zlib/zutil.h). macOS ships zlib in
# the SDK, so only build assimp's copy where there is no system one.
if(WIN32)
  set(ASSIMP_BUILD_ZLIB ON CACHE BOOL "" FORCE)
else()
  set(ASSIMP_BUILD_ZLIB OFF CACHE BOOL "" FORCE)
endif()

FetchContent_Declare(
  assimp
  GIT_REPOSITORY https://github.com/assimp/assimp.git
  GIT_TAG v6.0.5
  SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/assimp
)


if(APPLE)
FetchContent_Declare(
  slang
  URL https://github.com/shader-slang/slang/releases/download/v2026.14/slang-2026.14-macos-aarch64.zip
  URL_HASH SHA256=b8e2e277abd5cbe4591168ab4350ee46bd5631f705410208cb505c795fbab4b9
)
endif()

FetchContent_MakeAvailable(glfw stb yaml-cpp glm assimp metal-cpp slang)

add_subdirectory(bk)

if(NOT TARGET stb)
  add_library(stb INTERFACE)
  target_include_directories(stb INTERFACE ${stb_SOURCE_DIR})
endif()

if(NOT TARGET metal-cpp)
add_library(metal-cpp INTERFACE)
target_include_directories(metal-cpp INTERFACE ${metal-cpp_SOURCE_DIR})

endif()

