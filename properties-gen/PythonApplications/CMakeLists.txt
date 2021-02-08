set(PythonApplicationsPropertiesPlugin_SRCS
    pythonapplicationspropertiesplugin.cpp
)

# final lib
add_library(PythonApplicationsPropertiesPlugin SHARED ${PythonApplicationsPropertiesPlugin_SRCS})

target_link_libraries(PythonApplicationsPropertiesPlugin HemeraQt5SDK::Core HemeraQt5SDK::Generators HemeraQmlSettings Qt5::Core Qt5::Qml)

# RPath handling for plugin linking
set_target_properties(PythonApplicationsPropertiesPlugin PROPERTIES
                      SKIP_BUILD_RPATH FALSE
                      BUILD_WITH_INSTALL_RPATH TRUE
                      INSTALL_RPATH_USE_LINK_PATH TRUE
                      INSTALL_RPATH "${HEMERA_QML_SETTINGS_PLUGINS_DIR}/com/ispirata/Hemera/Settings")

# Install phase
install(TARGETS PythonApplicationsPropertiesPlugin
        RUNTIME DESTINATION "${INSTALL_BIN_DIR}" COMPONENT bin
        LIBRARY DESTINATION "${HEMERA_PROPERTIES_PLUGINS_DIR}" COMPONENT shlib
        ARCHIVE DESTINATION "${HEMERA_PROPERTIES_PLUGINS_DIR}" COMPONENT shlib
        COMPONENT PropertiesPlugins)

# Data files
install(FILES properties_template.py.in
        DESTINATION ${HA_SDK_DATA_DIR}/properties-gen/python-applications
        COMPONENT data)
