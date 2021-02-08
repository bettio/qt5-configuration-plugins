set(Qt5ApplicationsConfigurePlugin_SRCS
    qt5applicationsconfigureplugin.cpp
)

# final lib
add_library(Qt5ApplicationsConfigurePlugin SHARED ${Qt5ApplicationsConfigurePlugin_SRCS})

target_link_libraries(Qt5ApplicationsConfigurePlugin HemeraQt5SDK::Core HemeraQt5SDK::Generators HemeraQmlSettings Qt5::Core Qt5::Qml)

# RPath handling for plugin linking
set_target_properties(Qt5ApplicationsConfigurePlugin PROPERTIES
                      SKIP_BUILD_RPATH FALSE
                      BUILD_WITH_INSTALL_RPATH TRUE
                      INSTALL_RPATH_USE_LINK_PATH TRUE
                      INSTALL_RPATH "${HEMERA_QML_SETTINGS_PLUGINS_DIR}/com/ispirata/Hemera/Settings")

# Install phase
install(TARGETS Qt5ApplicationsConfigurePlugin
        RUNTIME DESTINATION "${HEMERA_CONFIGURE_PLUGINS_DIR}" COMPONENT bin
        LIBRARY DESTINATION "${HEMERA_CONFIGURE_PLUGINS_DIR}" COMPONENT shlib
        ARCHIVE DESTINATION "${HEMERA_CONFIGURE_PLUGINS_DIR}" COMPONENT shlib
        COMPONENT ConfigurePlugins)

# Configure files
install(FILES CMakeLists.txt.in RootOperation.cmake.in main-headless.cpp.in main-quick2.cpp.in main-widgets.cpp.in
        DESTINATION ${HA_SDK_DATA_DIR}/configure/qt5-applications
        COMPONENT data)
