juce_add_gui_app(audioBladeDesktop
    PRODUCT_NAME "audioBlade"
    BUNDLE_ID "dev.audioblade"
    VERSION "${PROJECT_VERSION}"
)

target_sources(audioBladeDesktop
    PRIVATE
        main.cpp
)

target_link_libraries(audioBladeDesktop
    PRIVATE
        juce::juce_gui_extra
        juce::juce_audio_devices
)

set_target_properties(audioBladeDesktop PROPERTIES
    OUTPUT_NAME "audioBlade"
)
