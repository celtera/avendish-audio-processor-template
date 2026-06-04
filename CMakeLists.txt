cmake_minimum_required(VERSION 3.13 FATAL_ERROR)

project(MyProcessor CXX)

include(dependencies.cmake)

add_library(MyProcessor STATIC
  src/Model.cpp
  src/Model.hpp
  src/UI.hpp
  src/Processor.hpp
)

avnd_make_all(
  TARGET MyProcessor
  MAIN_FILE "${CMAKE_CURRENT_SOURCE_DIR}/src/Processor.hpp"
  MAIN_CLASS MyProcessor
  C_NAME my_processor
)

# Workaround: when godot-cpp is pulled in via FetchContent, its only targets are
# `godot-cpp` (+ alias `godot::cpp`) — there is no `godot-cpp::godot-cpp`. Avendish's
# avnd_make_godot doesn't always resolve the target, so the generated headers under
# <godot-cpp-build>/gen/include never make it onto the GDExtension's include path and
# compilation fails with "godot_cpp/classes/node.hpp not found". Re-linking the target
# here (at a scope where it is visible) propagates godot-cpp's PUBLIC include dirs.
foreach(godot_target MyProcessor_NODE_godot MyProcessor_AUDIO_EFFECT_godot)
  if(TARGET ${godot_target} AND TARGET godot-cpp)
    target_link_libraries(${godot_target} PRIVATE godot-cpp)
  endif()
endforeach()
