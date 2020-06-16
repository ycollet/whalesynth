set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/modules/")
include(FindRedkite)

set(GSYNTH_GUI_HEADERS
  ${GSYNTH_GUI_DIR}/OperatorView.h
  ${GSYNTH_GUI_DIR}/MainWindow.h)

set(GSYNTH_GUI_SOURCES
  ${GSYNTH_GUI_DIR}/OperatorView.cpp
  ${GSYNTH_GUI_DIR}/MainWindow.cpp)

include_directories(${REDKITE_INCLUDE_DIR})
include_directories(${GSYNTH_SYNTH_DIR})
link_directories(${REDKITE_LIBRARY})

include(${GSYNTH_GUI_DIR}/resources/resources.rc)

foreach(pngFile ${gsynth_rc_png_files})
  message(STATUS "convert file ${pngFile}")
  get_filename_component(filename ${GSYNTH_RC_DIR}/${pngFile} NAME)
  string(REPLACE "." "_" filename ${filename})
  string(REPLACE " " "_" filename ${filename})
  add_custom_command(OUTPUT ${CMAKE_BINARY_DIR}/resources/${filename}.c
    COMMAND mkdir -p ${CMAKE_BINARY_DIR}/resources
    && rkpng2c "${GSYNTH_RC_DIR}/${pngFile}" "${CMAKE_BINARY_DIR}/resources/${filename}.c" "rk__${filename}")
    set(GSYNTH_RC_OUTPUT ${GSYNTH_RC_OUTPUT} ${CMAKE_BINARY_DIR}/resources/${filename}.c)
endforeach()

add_custom_target(gsynth_resources ALL DEPENDS ${GSYNTH_RC_OUTPUT})

add_library(gui STATIC
  ${GSYNTH_GUI_HEADERS}
  ${GSYNTH_GUI_SOURCES}
  ${GSYNTH_RC_OUTPUT})
