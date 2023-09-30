# CMakeList.txt : CMake project for electric_fields, include source and define
# project specific logic here.
#


# Add source to this project's executable.
add_executable (electric_fields "electric_fields.cpp" "electric_fields.h"  "field.h"  "particle.h")

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET electric_fields PROPERTY CXX_STANDARD 20)
endif()


# TODO: Add tests and install targets if needed.

find_package(VTK REQUIRED)
#  COMPONENTS 
#   CommonColor
#   CommonCore
#   CommonDataModel
#   CommonTransforms
#   FiltersGeneral
#   FiltersSources
#   InteractionStyle
#   RenderingContextOpenGL2
#   RenderingCore
#   RenderingFreeType
#   RenderingGL2PSOpenGL2
#   RenderingOpenGL2

include(${VTK_USE_FILE})


if (NOT VTK_FOUND)
  message(FATAL_ERROR "electric_fields: Unable to find the VTK build folder.")
endif()


# Prevent a "command line is too long" failure in Windows.
set(CMAKE_NINJA_FORCE_RESPONSE_FILE "ON" CACHE BOOL "Force Ninja to use response files.")

target_link_libraries(electric_fields PRIVATE ${VTK_LIBRARIES})

# IMPORTANT: 
# vtk_module_autoinit is needed
vtk_module_autoinit(
  TARGETS electric_fields
  MODULES ${VTK_LIBRARIES}
)
