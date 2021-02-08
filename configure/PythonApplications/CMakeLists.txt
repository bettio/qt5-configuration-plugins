set(PythonApplicationsConfigurePlugin_SRCS
    pythonapplicationsconfigureplugin.cpp
)

# final lib
add_library(PythonApplicationsConfigurePlugin SHARED ${PythonApplicationsConfigurePlugin_SRCS})

target_link_libraries(PythonApplicationsConfigurePlugin HemeraQt5SDK::Core HemeraQt5SDK::Generators HemeraQmlSettings Qt5::Core Qt5::Qml)

# RPath handling for plugin linking
set_target_properties(PythonApplicationsConfigurePlugin PROPERTIES
                      SKIP_BUILD_RPATH FALSE
                      BUILD_WITH_INSTALL_RPATH TRUE
                      INSTALL_RPATH_USE_LINK_PATH TRUE
                      INSTALL_RPATH "${HEMERA_QML_SETTINGS_PLUGINS_DIR}/com/ispirata/Hemera/Settings")

# Install phase
install(TARGETS PythonApplicationsConfigurePlugin
        RUNTIME DESTINATION "${HEMERA_CONFIGURE_PLUGINS_DIR}" COMPONENT bin
        LIBRARY DESTINATION "${HEMERA_CONFIGURE_PLUGINS_DIR}" COMPONENT shlib
        ARCHIVE DESTINATION "${HEMERA_CONFIGURE_PLUGINS_DIR}" COMPONENT shlib
        COMPONENT ConfigurePlugins)

# Configure files
install(FILES CMakeLists.txt.in
        DESTINATION ${HA_SDK_DATA_DIR}/configure/python-applications
        COMPONENT data)
