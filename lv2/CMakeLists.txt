#CMake file for LV2 Versions of Rakarrack-plus Effects

project (rakarrack-plus-lv2)

# config libraries

add_library(rakarrack-plus-lv2 SHARED
  rkrlv2.C
  ../src/EQ.C
  ../src/AnalogFilter.C
  ../src/Compressor.C
  ../src/Limiter.C
  ../src/Distorsion.C
  ../src/Overdrive.C
  ../src/Waveshaper.C
  ../src/Resample.C
  ../src/FPreset.C
  ../src/Echo.C
  ../src/delayline.C
  ../src/Chorus.C
  ../src/Flanger.C
  ../src/EffectLFO.C
  ../src/APhaser.C
  ../src/Harmonizer.C
  ../src/RecognizeNote.C
  ../src/RecChord.C
  ../src/Sustainer.C
  ../src/smbPitchShift.C
  ../src/Exciter.C
  ../src/HarmonicEnhancer.C
  ../src/Pan.C
  ../src/Alienwah.C
  ../src/Reverb.C
  ../src/Cabinet.C
  ../src/MusicDelay.C
  ../src/WahWah.C
  ../src/Filter.C
  ../src/FilterParams.C
  ../src/SVFilter.C
  ../src/FormantFilter.C
  ../src/Derelict.C
  ../src/Valve.C
  ../src/Dual_Flange.C
  ../src/Ring.C
  ../src/DistBand.C
  ../src/Arpie.C
  ../src/Expander.C
  ../src/Shuffle.C
  ../src/Synthfilter.C
  ../src/VaryBand.C
  ../src/RBFilter.C
  ../src/MuTroMojo.C
  ../src/Echoverse.C
  ../src/CoilCrafter.C
  ../src/ShelfBoost.C
  ../src/Vocoder.C
  ../src/Sequence.C
  ../src/beattracker.C
  ../src/Shifter.C
  ../src/StompBox.C
  ../src/Reverbtron.C
  ../src/Echotron.C
  ../src/StereoHarm.C
  ../src/CompBand.C
  ../src/Opticaltrem.C
  ../src/Vibe.C
  ../src/Infinity.C
  ../src/Phaser.C
  ../src/Gate.C
  ../src/mayer_fft.C
  ../src/Convolotron.C
  ../src/ParametricEQ.C
)

set_source_files_properties(../src/MIDIConverter.C PROPERTIES COMPILE_FLAGS -std=c++98)

target_sources(rakarrack-plus-lv2 PRIVATE
  ../src/MIDIConverter.C
)

target_compile_definitions(rakarrack-plus-lv2 PRIVATE LV2_SUPPORT=1)

set_target_properties(rakarrack-plus-lv2 PROPERTIES PREFIX "")

set_target_properties(rakarrack-plus-lv2 PROPERTIES BUILD_WITH_INSTALL_RPATH TRUE)

target_include_directories (rakarrack-plus-lv2 PRIVATE ${LV2_INCLUDE_DIRS} ${SAMPLERATE_INCLUDE_DIRS} ${FFTW3_INCLUDE_DIRS} ${SNDFILE_INCLUDE_DIRS} ../src)
target_link_libraries(rakarrack-plus-lv2 PRIVATE ${LV2_LIBRARIES} ${SAMPLERATE_LIBRARIES} ${FFTW3_LIBRARIES} ${SNDFILE_LIBRARIES} )

# config install
install(TARGETS rakarrack-plus-lv2
  LIBRARY
  DESTINATION ${LV2_INSTALL_DIR}
)

install (DIRECTORY ttl/
  DESTINATION ${LV2_INSTALL_DIR}
)

#Data files needed for default settings and presets. They must be included in the lv2 bundle folder.
#We copy all the data files here, but only those indicated by the presets & defaults are actually needed.
#Rakarrack has a pop up 'preset' selector that gives access to all the data files. This feature is
#not implemented in the lv2s. Instead the lv2s must use the user file lookup.
install (DIRECTORY ${CMAKE_BINARY_DIR}/lv2data/
  DESTINATION ${LV2_INSTALL_DIR}
)

#This is a convenience for the user to easily access the data files from the user file lookup
#which are also be copied to the lv2 bundle folder above. 
install (DIRECTORY ${CMAKE_BINARY_DIR}/lv2data/
  DESTINATION share/RakarrackPlus.lv2
)

if(BuildCarlaPresets)
    install (DIRECTORY presets/
        DESTINATION share/RakarrackPlus.lv2
    )
endif(BuildCarlaPresets)
