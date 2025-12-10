cmake_minimum_required(VERSION 3.24)
project(PimpleJuice VERSION 1.0.0)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# JUCE
set(JUCE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/JUCE")
if(NOT EXISTS "${JUCE_PATH}/CMakeLists.txt")
    message(FATAL_ERROR "JUCE not found at: ${JUCE_PATH}. Please clone JUCE: git clone https://github.com/juce-framework/JUCE.git")
endif()
add_subdirectory("${JUCE_PATH}" "${CMAKE_BINARY_DIR}/JUCE_build")

# Plugin target
juce_add_plugin(PimpleJuice
    COMPANY_NAME "GodOfEcht"
    PLUGIN_MANUFACTURER_CODE "GoEc"
    PLUGIN_CODE "PmJc"
    FORMATS VST3 AU Standalone
    PRODUCT_NAME "PimpleJuice"
    IS_SYNTH FALSE
    NEEDS_MIDI_INPUT FALSE
    NEEDS_MIDI_OUTPUT FALSE
    IS_MIDI_EFFECT FALSE
    EDITOR_WANTS_KEYBOARD_FOCUS FALSE
)

juce_generate_juce_header(PimpleJuice)

target_sources(PimpleJuice PRIVATE
    Source/PluginProcessor.cpp
    Source/PluginEditor.cpp
)

target_include_directories(PimpleJuice PRIVATE Source)

target_compile_definitions(PimpleJuice PRIVATE
    JUCE_WEB_BROWSER=0
    JUCE_USE_CURL=0
    JUCE_VST3_CAN_REPLACE_VST2=0
    JUCE_DISPLAY_SPLASH_SCREEN=0
)

target_link_libraries(PimpleJuice PRIVATE
    juce::juce_audio_utils
    juce::juce_audio_plugin_client
    juce::juce_audio_processors
    juce::juce_audio_basics
    juce::juce_gui_basics
    juce::juce_dsp
)

if(APPLE)
    set(CMAKE_OSX_ARCHITECTURES "x86_64;arm64")
endif()

message(STATUS "✅ Building PimpleJuice - A Better Sampler")
