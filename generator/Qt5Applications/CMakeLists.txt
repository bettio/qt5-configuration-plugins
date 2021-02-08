set(Qt5ApplicationsGeneratorPlugin_SRCS
    qt5applicationsgeneratorplugin.cpp
)

# final lib
add_library(Qt5ApplicationsGeneratorPlugin SHARED ${Qt5ApplicationsGeneratorPlugin_SRCS})

target_link_libraries(Qt5ApplicationsGeneratorPlugin HemeraQt5SDK::Core HemeraQt5SDK::Generators HemeraQmlSettings Qt5::Core Qt5::Qml)

# RPath handling for plugin linking
set_target_properties(Qt5ApplicationsGeneratorPlugin PROPERTIES
                      SKIP_BUILD_RPATH FALSE
                      BUILD_WITH_INSTALL_RPATH TRUE
                      INSTALL_RPATH_USE_LINK_PATH TRUE
                      INSTALL_RPATH "${HEMERA_QML_SETTINGS_PLUGINS_DIR}/com/ispirata/Hemera/Settings")

# Install phase
install(TARGETS Qt5ApplicationsGeneratorPlugin
        RUNTIME DESTINATION "${INSTALL_BIN_DIR}" COMPONENT bin
        LIBRARY DESTINATION "${HEMERA_GENERATOR_PLUGINS_DIR}" COMPONENT shlib
        ARCHIVE DESTINATION "${HEMERA_GENERATOR_PLUGINS_DIR}" COMPONENT shlib
        COMPONENT GeneratorPlugins)
