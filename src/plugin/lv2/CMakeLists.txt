pkg_check_modules(LV2 lv2)
if (NOT LV2_FOUND)
  message(FATAL_ERROR "can't find LV2 library")
endif(NOT LV2_FOUND)

include_directories(${GSYNTH_SYNTH_DIR})
include_directories(${GSYNTH_GUI_DIR})
include_directories(${REDKITE_INCLUDE_DIR})

set(GEONSYNTH_LV2_DSP_SOURCES ${GSYNTH_LV2_DIR}/geonsynth_lv2_dsp.cpp)
set(GEONSYNTH_LV2_DSP_TARGET_NAME "${PROJECT_NAME}_lv2_dsp")
set(GEONSYNTH_LV2_UI_SOURCES ${GSYNTH_LV2_DIR}/geonsynth_lv2_ui.cpp)
set(GEONSYNTH_LV2_UI_TARGET_NAME "${PROJECT_NAME}_lv2_ui")

add_library(${GEONSYNTH_LV2_DSP_TARGET_NAME} SHARED
            ${GEONSYNTH_LV2_DSP_SOURCES})
add_library(${GEONSYNTH_LV2_UI_TARGET_NAME} SHARED
            ${GEONSYNTH_LV2_UI_SOURCES})

target_link_libraries(${GEONSYNTH_LV2_DSP_TARGET_NAME} synth)
target_link_libraries(${GEONSYNTH_LV2_DSP_TARGET_NAME} "-lrt -lm -ldl -lpthread")

target_link_libraries(${GEONSYNTH_LV2_UI_TARGET_NAME} gui)
target_link_libraries(${GEONSYNTH_LV2_UI_TARGET_NAME} "-lredkite -lX11 -lrt -lm -ldl -lpthread -lcairo")

install(FILES ${GSYNTH_LV2_DIR}/manifest.ttl
                 ${GSYNTH_LV2_DIR}/${PROJECT_NAME}_dsp.ttl
                 ${GSYNTH_LV2_DIR}/${PROJECT_NAME}_dsp.ttl
                 ${GSYNTH_LV2_DIR}/${PROJECT_NAME}_ui.ttl
                 ${GSYNTH_LV2_DIR}/${PROJECT_NAME}_ui.ttl
              DESTINATION ${CMAKE_INSTALL_PREFIX}/lib/lv2/${PROJECT_NAME}.lv2)
install(TARGETS ${GEONSYNTH_LV2_DSP_TARGET_NAME} DESTINATION ${CMAKE_INSTALL_PREFIX}/lib/lv2/${PROJECT_NAME}.lv2)
install(TARGETS ${GEONSYNTH_LV2_UI_TARGET_NAME} DESTINATION ${CMAKE_INSTALL_PREFIX}/lib/lv2/${PROJECT_NAME}.lv2)
