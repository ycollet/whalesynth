if (GSYNTH_STANDALONE)
  pkg_check_modules(JACK jack)
  if (NOT JACK_FOUND)
    message(FATAL_ERROR "can't find jack library")
  endif(NOT JACK_FOUND)
  list(APPEND GSYNTH_API_FLAGS "-DGEONSYNTH_AUDIO_JACK")
endif()

set(GSYNTH_STANDALONE_HEADERS
  ${GSYNTH_STANDALONE_DIR}/Jack.cpp
  ${GSYNTH_STANDALONE_DIR}/main.cpp)

set(GSYNTH_STANDALONE_SOURCES
  ${GSYNTH_STANDALONE_DIR}/Jack.cpp
  ${GSYNTH_STANDALONE_DIR}/main.cpp)

set(GSYNTH_TARGET_NAME "geonsynth")

add_executable(${GSYNTH_TARGET_NAME}
  ${GSYNTH_STANDALONE_HEADERS}
  ${GSYNTH_STANDALONE_SOURCES})

add_dependencies(${GSYNTH_TARGET_NAME} synth)
target_link_libraries(${GSYNTH_TARGET_NAME} "-ljack -lrt -lm -ldl -lpthread")

install(TARGETS ${GSYNTH_TARGET_NAME} DESTINATION ${CMAKE_INSTALL_PREFIX}/bin)
