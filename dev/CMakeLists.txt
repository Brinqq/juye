add_library(dev SHARED main.cpp)
target_include_directories(dev PRIVATE ../include)

target_link_directories(dev PRIVATE 
  ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}
)

target_link_libraries(dev PRIVATE
  common
)

set_target_properties(dev PROPERTIES CXX_VISIBILITY_PRESET hidden)

