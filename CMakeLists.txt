cmake_minimum_required (VERSION 3.7)

project(geonsynth VERSION 1.0.0)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/modules/")
include(FindRedkite)

option(GSYNTH_STANDALONE "Enable build for standalone" ON)
option(GSYNTH_PLUGIN "Enable build for plugin" OFF)
if (GSYNTH_PLUGIN)
  option(GSYNTH_PLUGIN_LV2 "Enable build LV2 plugin" ON)
  if (GSYNTH_VST_SDK_PATH)
    option(GSYNTH_PLUGIN_VST "Enable build VST plugin" ON)
  else(GSYNTH_VST_SDK_PATH)
    option(GSYNTH_PLUGIN_VST "Enable build VST plugin" OFF)
  endif (GSYNTH_VST_SDK_PATH)
endif (GSYNTH_PLUGIN)

set(GSYNTH_SYNTH_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src/synth)
set(GSYNTH_GUI_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src/gui)
set(GSYNTH_STANDALONE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src/standalone)
set(GSYNTH_PLUGIN_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src/plugin)
set(GSYNTH_DATA_DIR ${CMAKE_CURRENT_SOURCE_DIR}/data)
set(GSYNTH_PRESETS_DIR ${CMAKE_CURRENT_SOURCE_DIR}/presets)
set(GSYNTH_DOC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/doc)
if (GSYNTH_PLUGIN)
  if (GSYNTH_PLUGIN_VST)
    set(GSYNTH_VST_DIR ${GSYNTH_PLUGIN_DIR}/vst)
  endif (GSYNTH_PLUGIN_VST)
  if (GSYNTH_PLUGIN_LV2)
    set(GSYNTH_LV2_DIR ${GSYNTH_PLUGIN_DIR}/lv2)
  endif (GSYNTH_PLUGIN_LV2)
endif(GSYNTH_PLUGIN)

if (NOT CMAKE_BUILD_TYPE)
  message(STATUS "no build type selected, set default to Release")
  set(CMAKE_BUILD_TYPE Release CACHE STRING "Build type Release" FORCE)
endif()

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "-Wall ${CMAKE_CXX_FLAGS} -fvisibility=hidden")
set(CMAKE_C_FLAGS "-Wall ${CMAKE_C_FLAGS} -fvisibility=hidden")

if (CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
  set (CMAKE_INSTALL_PREFIX "/usr/local" CACHE PATH "default install path" FORCE)
endif()

if (NOT CMAKE_INSTALL_FULL_DATADIR)
  message(STATUS "CMAKE_INSTALL_FULL_DATADIR is not set, use default ${CMAKE_INSTALL_PREFIX}/share")
  set(CMAKE_INSTALL_FULL_DATADIR "${CMAKE_INSTALL_PREFIX}/share" CACHE PATH "default data dir path" FORCE)
endif()

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DGEONSYNTH_DATA_DIR=\\\"${CMAKE_INSTALL_FULL_DATADIR}\\\"")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DGEONSYNTH_DATA_DIR=\\\"${CMAKE_INSTALL_FULL_DATADIR}\\\"")

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
  message(STATUS "Enable application logging")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DGEONSYNTH_DEBUG")
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DGEONSYNTH_DEBUG")
endif()

find_package(PkgConfig REQUIRED)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
add_definitions(-fPIC)

message(STATUS "------------ Summary ---------")
if (GSYNTH_STANDALONE)
  message(STATUS "Standalone: yes" )
else(GSYNTH_STANDALONE)
  message(STATUS "Standalone: no")
endif(GSYNTH_STANDALONE)

if (GSYNTH_PLUGIN_LV2)
  message(STATUS "LV2 plugin: yes" )
else(GSYNTH_PLUGIN_LV2)
  message(STATUS "LV2 plugin: no")
endif(GSYNTH_PLUGIN_LV2)

if (GSYNTH_PLUGIN_VST)
  message(STATUS "VST plugin: yes" )
else(GSYNTH_PLUGIN_VST)
  message(STATUS "VST plugin: no")
endif(GSYNTH_PLUGIN_VST)

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
  message(STATUS "Debug enabled: yes" )
else()
  message(STATUS "Debug enabled: no" )
endif()
message(STATUS "------------------------------")

add_subdirectory(${GSYNTH_SYNTH_DIR})

if (GSYNTH_STANDALONE)
  add_subdirectory(${GSYNTH_STANDALONE_DIR})
endif (GSYNTH_STANDALONE)

if (GSYNTH_PLUGIN)
  add_subdirectory(${GSYNTH_PLUGIN_DIR})
endif (GSYNTH_PLUGIN)

add_subdirectory(data)
add_subdirectory(presets)
