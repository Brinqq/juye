set(src ${CMAKE_CURRENT_SOURCE_DIR})

set(osx_src ${src}/platform/macos)


if(APPLE)
    set(platform_src
      ${osx_src}/display.cpp
      gk/renderer/render_metal.cpp
      drivers/metal/layer_adapter.mm
      drivers/metal/futils.cpp
      drivers/metal/mtl_helpers.cpp
      # base/drivers/metal/metal_core.cpp
    )

    # The bridge uses __bridge casts, which require ARC. Scoped to the one
    # .mm file so the rest of the target is unaffected.
    set_source_files_properties(core/drivers/metal/layer_adapter.mm
      PROPERTIES COMPILE_OPTIONS "-fobjc-arc"
    )
endif()

add_library(juye
  #base
  base/entry.cpp
  base/escm.cpp
  base/utils.cpp

  fsystem/image.cpp

  gk/model/prefabs.cpp
  gk/renderer/render_frontend.cpp

  #platform
  ${platform_src}
)

target_include_directories(juye PUBLIC
  ../include
)


target_include_directories(juye PRIVATE
  ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(juye PRIVATE
  glfw
  yaml-cpp
  glm
  stb
  assimp
  metal-cpp
)

#tmp
target_compile_options(juye PRIVATE
  -g
)

if(APPLE)
  message(here)
  find_library(METAL_LIBRARY Metal REQUIRED)
  find_library(FOUNDATION_LIBRARY Foundation REQUIRED)
  find_library(QUARTZCORE_LIBRARY QuartzCore REQUIRED)
  find_library(APPKIT_LIBRARY AppKit REQUIRED) # NSWindow in layer_adapter.mm

  target_link_libraries(juye PRIVATE
    ${METAL_LIBRARY}
    ${FOUNDATION_LIBRARY}
    ${QUARTZCORE_LIBRARY}
    ${APPKIT_LIBRARY}
  )
endif()

target_link_libraries(juye PUBLIC
  bk
)

target_link_directories(juye PRIVATE
 ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}
 ${CMAKE_ARCHIVE_OUTPUT_DIRECTORY}
)
