pkg_check_modules(LV2 lv2)
if (NOT LV2_FOUND)
  message(FATAL_ERROR "can't find LV2 library")
endif(NOT LV2_FOUND)

include_directories(${GSYNTH_SYNTH_DIR})

set(GEONSYNTH_LV2_SOURCES ${GSYNTH_LV2_DIR}/geonsynth_lv2.cpp)
set(GEONSYNTH_LV2_TARGET_NAME "${PROJECT_NAME}_lv2")

add_library(${GEONSYNTH_LV2_TARGET_NAME} SHARED
            ${GEONSYNTH_LV2_SOURCES})

target_link_libraries(${GEONSYNTH_LV2_TARGET_NAME}  synth)
target_link_libraries(${GEONSYNTH_LV2_TARGET_NAME} "-lrt -lm -ldl -lpthread")

install(FILES ${GSYNTH_LV2_DIR}/manifest.ttl
              ${GSYNTH_LV2_DIR}/${PROJECT_NAME}.ttl
              DESTINATION ${CMAKE_INSTALL_PREFIX}/lib/lv2/${PROJECT_NAME}.lv2)
install(TARGETS ${GEONSYNTH_LV2_TARGET_NAME} DESTINATION ${CMAKE_INSTALL_PREFIX}/lib/lv2/${PROJECT_NAME}.lv2)
