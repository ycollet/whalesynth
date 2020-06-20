pkg_check_modules(LV2 lv2)
if (NOT LV2_FOUND)
  message(FATAL_ERROR "can't find LV2 library")
endif(NOT LV2_FOUND)

include_directories(${WHALE_SYNTH_DIR})
include_directories(${WHALE_GUI_DIR})
include_directories(${REDKITE_INCLUDE_DIR})

set(WHALESYNTH_LV2_DSP_SOURCES
  ${WHALE_LV2_DIR}/whalesynth_lv2_dsp.cpp)
set(WHALESYNTH_LV2_DSP_TARGET_NAME "${PROJECT_NAME}_lv2_dsp")

set(WHALESYNTH_LV2_UI_HEADERS
  ${WHALE_LV2_DIR}/LV2SynthesizerModelProxy.h)
set(WHALESYNTH_LV2_UI_SOURCES
  ${WHALE_LV2_DIR}/LV2SynthesizerModelProxy.cpp
  ${WHALE_LV2_DIR}/whalesynth_lv2_ui.cpp)
set(WHALESYNTH_LV2_UI_TARGET_NAME "${PROJECT_NAME}_lv2_ui")

add_library(${WHALESYNTH_LV2_DSP_TARGET_NAME} SHARED
            ${WHALESYNTH_LV2_DSP_SOURCES})
add_library(${WHALESYNTH_LV2_UI_TARGET_NAME} SHARED
            ${WHALESYNTH_LV2_UI_HEADERS}
            ${WHALESYNTH_LV2_UI_SOURCES})

target_link_libraries(${WHALESYNTH_LV2_DSP_TARGET_NAME} synth)
target_link_libraries(${WHALESYNTH_LV2_DSP_TARGET_NAME} "-lrt -lm -ldl -lpthread")

target_link_libraries(${WHALESYNTH_LV2_UI_TARGET_NAME} gui)
target_link_libraries(${WHALESYNTH_LV2_UI_TARGET_NAME} "-lredkite -lX11 -lrt -lm -ldl -lpthread -lcairo")

install(FILES ${WHALE_LV2_DIR}/manifest.ttl
              ${WHALE_LV2_DIR}/${PROJECT_NAME}_dsp.ttl
              ${WHALE_LV2_DIR}/${PROJECT_NAME}_dsp.ttl
              ${WHALE_LV2_DIR}/${PROJECT_NAME}_ui.ttl
              ${WHALE_LV2_DIR}/${PROJECT_NAME}_ui.ttl
              DESTINATION ${CMAKE_INSTALL_PREFIX}/lib/lv2/${PROJECT_NAME}.lv2)
install(TARGETS ${WHALESYNTH_LV2_DSP_TARGET_NAME}
                DESTINATION ${CMAKE_INSTALL_PREFIX}/lib/lv2/${PROJECT_NAME}.lv2)
install(TARGETS ${WHALESYNTH_LV2_UI_TARGET_NAME}
                DESTINATION ${CMAKE_INSTALL_PREFIX}/lib/lv2/${PROJECT_NAME}.lv2)
