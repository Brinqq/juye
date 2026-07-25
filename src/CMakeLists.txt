
add_library(juye
  core/juye.cpp
)

target_include_directories(juye PUBLIC
  ../include
  ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(juye PRIVATE

)

target_link_directories(juye PRIVATE
 ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}
)
