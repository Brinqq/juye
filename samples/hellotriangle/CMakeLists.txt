add_executable(hello-triangle hello_triangle.cpp)

set_target_properties(hello-triangle PROPERTIES
  RUNTIME_OUTPUT_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/samples
)
