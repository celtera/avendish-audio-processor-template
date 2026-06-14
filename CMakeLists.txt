cmake_minimum_required(VERSION 3.24 FATAL_ERROR)

project(MyProcessor CXX)

# Use the Avendish the host already provides (e.g. ossia score); otherwise fetch it.
# The same CMakeLists builds this as an ossia/score add-on or as a standalone object.
find_package(Avendish QUIET)
if(NOT Avendish_FOUND)
  include(FetchContent)
  FetchContent_Declare(
    Avendish
    GIT_REPOSITORY "https://github.com/celtera/avendish"
    GIT_TAG 39a9896686e613fd25729a438ccb79bf1dac3288)
  FetchContent_Populate(Avendish)
  list(APPEND CMAKE_PREFIX_PATH "${avendish_SOURCE_DIR}")
  find_package(Avendish REQUIRED)
endif()

avnd_addon_init(NAME MyProcessor)

# CATEGORY all: every back-end family -- audio plug-ins (VST3 / CLAP / VST2), the object
# bindings (Max / Pd / Python / TouchDesigner / Godot) and gstreamer. In a score build
# only the ossia process is produced. Back-ends whose SDK is absent are silently skipped.
avnd_addon_object(
  BASE MyProcessor
  C_NAME my_processor
  CLASS MyProcessor
  CATEGORY all
  MAIN_FILE "${CMAKE_CURRENT_SOURCE_DIR}/src/Processor.hpp"
  SOURCES
    src/Model.cpp
    src/Model.hpp
    src/UI.hpp
    src/Processor.hpp)

avnd_addon_finalize(NAME MyProcessor UUID a0ef0602-d1b2-46b7-9615-2049e1e97774 VERSION 1.0.0)
