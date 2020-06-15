include_directories(${GSYNTH_SYNTH_DIR})

set(GSYNTH_SYNTH_HEADERS
  ${GSYNTH_SYNTH_DIR}/GeonSynth.h
  ${GSYNTH_SYNTH_DIR}/WaveGenerator.h
  ${GSYNTH_SYNTH_DIR}/Operator.h
  ${GSYNTH_SYNTH_DIR}/Note.h
  ${GSYNTH_SYNTH_DIR}/SynthesizerVoice.h
  ${GSYNTH_SYNTH_DIR}/Synthesizer.h)

set(GSYNTH_SYNTH_HEADERS
  ${GSYNTH_SYNTH_DIR}/GeonSynth.h
  ${GSYNTH_SYNTH_DIR}/WaveGenerator.cpp
  ${GSYNTH_SYNTH_DIR}/Operator.cpp
  ${GSYNTH_SYNTH_DIR}/Synthesizer.cpp
  ${GSYNTH_SYNTH_DIR}/SynthesizerVoice.cpp)

add_library(synth STATIC
  ${GSYNTH_SYNTH_HEADERS}
  ${GSYNTH_SYNTH_SOURCES})

