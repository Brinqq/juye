include(FetchContent)

set(GLFW_BUILD_DOCS OFF)
set(GLFW_BUILD_TESTS OFF)
set(GLFW_BUILD_EXAMPLES OFF)
set(GLFW_INSTALL OFF)

FetchContent_Declare(
  glfw
  GIT_REPOSITORY https://github.com/glfw/glfw.git
  GIT_TAG 3.4
)

set(YAML_CPP_BUILD_TESTS OFF)
set(YAML_CPP_BUILD_TOOLS OFF)
set(YAML_CPP_INSTALL OFF)
FetchContent_Declare(
  yaml-cpp
  GIT_REPOSITORY https://github.com/jbeder/yaml-cpp.git
  GIT_TAG yaml-cpp-0.9.0
)

set(GLM_BUILD_TESTS OFF)
set(GLM_BUILD_INSTALL OFF)

FetchContent_Declare(
  glm
  GIT_REPOSITORY https://github.com/g-truc/glm.git
  GIT_TAG 1.0.3
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
)

FetchContent_MakeAvailable(glfw yaml-cpp glm assimp)
