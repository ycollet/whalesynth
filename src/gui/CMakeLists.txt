set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/modules/")
include(FindRedkite)

set(WHALE_GUI_HEADERS
  ${WHALE_GUI_DIR}/SynthesizerModel.h
  ${WHALE_GUI_DIR}/WhaleSynthWidget.h
  ${WHALE_GUI_DIR}/TopMenu.h
  ${WHALE_GUI_DIR}/OperatorView.h
  ${WHALE_GUI_DIR}/MainWindow.h)

set(WHALE_GUI_SOURCES
  ${WHALE_GUI_DIR}/SynthesizerModel.cpp
  ${WHALE_GUI_DIR}/WhaleSynthWidget.cpp
  ${WHALE_GUI_DIR}/OperatorView.cpp
  ${WHALE_GUI_DIR}/TopMenu.cpp
  ${WHALE_GUI_DIR}/MainWindow.cpp)

include_directories(${REDKITE_INCLUDE_DIR})
include_directories(${WHALE_SYNTH_DIR})
link_directories(${REDKITE_LIBRARY})

include(${WHALE_GUI_DIR}/resources/resources.rc)

foreach(pngFile ${whale_rc_png_files})
  message(STATUS "convert file ${pngFile}")
  get_filename_component(filename ${WHALE_RC_DIR}/${pngFile} NAME)
  string(REPLACE "." "_" filename ${filename})
  string(REPLACE " " "_" filename ${filename})
  add_custom_command(OUTPUT ${CMAKE_BINARY_DIR}/resources/${filename}.c
    COMMAND mkdir -p ${CMAKE_BINARY_DIR}/resources
    && rkpng2c "${WHALE_RC_DIR}/${pngFile}" "${CMAKE_BINARY_DIR}/resources/${filename}.c" "rk__${filename}")
    set(WHALE_RC_OUTPUT ${WHALE_RC_OUTPUT} ${CMAKE_BINARY_DIR}/resources/${filename}.c)
endforeach()

add_custom_target(whale_resources ALL DEPENDS ${WHALE_RC_OUTPUT})

add_library(gui STATIC
  ${WHALE_GUI_HEADERS}
  ${WHALE_GUI_SOURCES}
  ${WHALE_RC_OUTPUT})
