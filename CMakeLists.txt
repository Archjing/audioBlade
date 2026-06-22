cmake_minimum_required(VERSION 3.28)

project(audioBlade
    VERSION 0.1.0
    DESCRIPTION "Persistent desktop audio routing foundation"
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(AUDIOBLADE_APP_NAME "audioBlade")
set(AUDIOBLADE_APP_IDENTIFIER "dev.audioblade")
set(AUDIOBLADE_DESKTOP_TARGET "audioBladeDesktop")

option(AUDIOBLADE_ENABLE_JUCE_DESKTOP "Build the JUCE desktop app when extern/JUCE is available" ON)

if(AUDIOBLADE_ENABLE_JUCE_DESKTOP AND EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/extern/JUCE/CMakeLists.txt")
    add_subdirectory(extern/JUCE EXCLUDE_FROM_ALL)
    add_subdirectory(apps/desktop)
else()
    add_custom_target(audioBladeDesktop ALL
        COMMAND ${CMAKE_COMMAND} -E echo "audioBladeDesktop placeholder: provide extern/JUCE to build the JUCE app."
        VERBATIM
    )
    message(STATUS "JUCE desktop target is a placeholder. Provide extern/JUCE to build the desktop app.")
endif()
