if (GSYNTH_STANDALONE)
  pkg_check_modules(JACK jack)
  if (NOT JACK_FOUND)
    message(FATAL_ERROR "can't find jack library")
  endif(NOT JACK_FOUND)
  list(APPEND GSYNTH_API_FLAGS "-DGEONSYNTH_AUDIO_JACK")
endif()

set(GSYNTH_STANDALONE_SOURCES
  ${GSYNTH_STANDALONE_DIR}/Jack.cpp
  ${GSYNTH_STANDALONE_DIR}/main.cpp)

set(GSYNTH_TARGET_NAME "geonsynth");

add_executable(geonkick
  ${GSYNTH_COMMON_HEADERS}
  ${GSYNTH_COMMON_RESOURCES}
  ${GSYNTH_STANDALONE_SOURCES})

add_dependencies(GSYNTH_TARGET_NAME api_standalone)
target_link_libraries(GSYNTH_TARGET_NAME  geonkick_common api_standalone)
target_link_libraries(GSYNTH_TARGET_NAME "-lstdc++fs")
target_link_libraries(GSYNTH_TARGET_NAME "-lredkite -lX11 -ljack -lsndfile -lrt -lm -ldl -lpthread -lcairo")

install(TARGETS GSYNTH_TARGET_NAME DESTINATION ${CMAKE_INSTALL_PREFIX}/bin)
