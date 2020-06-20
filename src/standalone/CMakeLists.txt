if (WHALE_STANDALONE)
  pkg_check_modules(JACK jack)
  if (NOT JACK_FOUND)
    message(FATAL_ERROR "can't find jack library")
  endif(NOT JACK_FOUND)
  list(APPEND WHALE_API_FLAGS "-DWHALESYNTH_AUDIO_JACK")
endif()

set(WHALE_STANDALONE_HEADERS
  ${WHALE_STANDALONE_DIR}/Jack.cpp
  ${WHALE_STANDALONE_DIR}/main.cpp)

set(WHALE_STANDALONE_SOURCES
  ${WHALE_STANDALONE_DIR}/Jack.cpp
  ${WHALE_STANDALONE_DIR}/main.cpp)

include_directories(${WHALE_SYNTH_DIR})
include_directories(${REDKITE_INCLUDE_DIR})
include_directories(${WHALE_GUI_DIR})
include_directories(${WHALE_STANDALONE_DIR})

set(WHALE_TARGET_NAME "whalesynth")

add_executable(${WHALE_TARGET_NAME}
  ${WHALE_STANDALONE_HEADERS}
  ${WHALE_STANDALONE_SOURCES})

target_link_libraries(${WHALE_TARGET_NAME} synth)
target_link_libraries(${WHALE_TARGET_NAME} gui)
target_link_libraries(${WHALE_TARGET_NAME} "-lredkite -lX11 -ljack -lrt -lm -ldl -lpthread -lcairo")

install(TARGETS ${WHALE_TARGET_NAME} DESTINATION ${CMAKE_INSTALL_PREFIX}/bin)
