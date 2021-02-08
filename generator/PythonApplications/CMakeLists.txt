set(PythonApplicationsGeneratorPlugin_SRCS
    pythonapplicationsgeneratorplugin.cpp
)

# final lib
add_library(PythonApplicationsGeneratorPlugin SHARED ${PythonApplicationsGeneratorPlugin_SRCS})

target_link_libraries(PythonApplicationsGeneratorPlugin HemeraQt5SDK::Core HemeraQt5SDK::Generators HemeraQmlSettings Qt5::Core Qt5::Qml)

# RPath handling for plugin linking
set_target_properties(PythonApplicationsGeneratorPlugin PROPERTIES
                      SKIP_BUILD_RPATH FALSE
                      BUILD_WITH_INSTALL_RPATH TRUE
                      INSTALL_RPATH_USE_LINK_PATH TRUE
                      INSTALL_RPATH "${HEMERA_QML_SETTINGS_PLUGINS_DIR}/com/ispirata/Hemera/Settings")

# Install phase
install(TARGETS PythonApplicationsGeneratorPlugin
        RUNTIME DESTINATION "${INSTALL_BIN_DIR}" COMPONENT bin
        LIBRARY DESTINATION "${HEMERA_GENERATOR_PLUGINS_DIR}" COMPONENT shlib
        ARCHIVE DESTINATION "${HEMERA_GENERATOR_PLUGINS_DIR}" COMPONENT shlib
        COMPONENT GeneratorPlugins)
