include_directories(${CMAKE_SOURCE_DIR}/lib ${HyperspaceQt5Core_INCLUDE_DIRS})

set(HyperspaceCompilerPlugin_SRCS
    hyperspacecompilerplugin.cpp
    hyperspacecompileoperation.cpp
)

# final lib
add_library(HyperspaceCompilerPlugin SHARED ${HyperspaceCompilerPlugin_SRCS})

target_link_libraries(HyperspaceCompilerPlugin Qt5::Core Qt5::Qml HemeraQt5SDK::Core HemeraQt5SDK::Generators HemeraQmlSettings)

set_target_properties(HyperspaceCompilerPlugin PROPERTIES
                      SKIP_BUILD_RPATH FALSE
                      BUILD_WITH_INSTALL_RPATH TRUE
                      INSTALL_RPATH_USE_LINK_PATH TRUE
                      INSTALL_RPATH "${HEMERA_QML_SETTINGS_PLUGINS_DIR}/com/ispirata/Hemera/Settings")

# Install phase
install(TARGETS HyperspaceCompilerPlugin
        RUNTIME DESTINATION "${INSTALL_BIN_DIR}" COMPONENT bin
        LIBRARY DESTINATION "${HEMERA_COMPILER_PLUGINS_DIR}" COMPONENT shlib
        ARCHIVE DESTINATION "${HEMERA_COMPILER_PLUGINS_DIR}" COMPONENT shlib
        COMPONENT HyperspacePlugins)

set(HyperspaceCompilerPlugin_MISCFILES
    hyperdrive-transport-http.socket.in
    mosquitto-astarte-gateway.conf.in
)

# CMake modules
install(FILES ${HyperspaceCompilerPlugin_MISCFILES}
        DESTINATION ${HA_SDK_DATA_DIR}
        COMPONENT data)
