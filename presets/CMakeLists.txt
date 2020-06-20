#set(WHALE_PRESETS)

#foreach(compressedFolder ${WHALE_PRESETS})
#  message(STATUS "uncompress file ${compressedFolder}")
#  get_filename_component(presetsFolder ${compressedFolder} NAME)
#  string(REPLACE ".tar.xz" "" presetsFolder ${presetsFolder})
#  add_custom_command(OUTPUT ${presetsFolder}
#    COMMAND tar xvf ${WHALE_PRESETS_DIR}/${compressedFolder})
#  set(WHALE_PRESETS_OUTPUT ${WHALE_PRESETS_OUTPUT} ${CMAKE_BINARY_DIR}/presets/${presetsFolder})
#endforeach()

#add_custom_target(whale_presets ALL DEPENDS ${WHALE_PRESETS_OUTPUT})
#install(DIRECTORY ${WHALE_PRESETS_OUTPUT} DESTINATION ${CMAKE_INSTALL_FULL_DATADIR}/whaleSynth/presets)

