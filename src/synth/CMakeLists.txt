include_directories(${WHALE_SYNTH_DIR})

set(WHALE_SYNTH_HEADERS
  ${WHALE_SYNTH_DIR}/WhaleSynth.h
  ${WHALE_SYNTH_DIR}/WaveGenerator.h
  ${WHALE_SYNTH_DIR}/Operator.h
  ${WHALE_SYNTH_DIR}/Note.h
  ${WHALE_SYNTH_DIR}/SynthesizerVoice.h
  ${WHALE_SYNTH_DIR}/Synthesizer.h
  ${WHALE_SYNTH_DIR}/SynthesizerProxy.h)

set(WHALE_SYNTH_HEADERS
  ${WHALE_SYNTH_DIR}/WhaleSynth.h
  ${WHALE_SYNTH_DIR}/WaveGenerator.cpp
  ${WHALE_SYNTH_DIR}/Operator.cpp
  ${WHALE_SYNTH_DIR}/Synthesizer.cpp
  ${WHALE_SYNTH_DIR}/SynthesizerVoice.cpp)

add_library(synth STATIC
  ${WHALE_SYNTH_HEADERS}
  ${WHALE_SYNTH_SOURCES})

