set(GravityCompilerPlugin_SRCS
    gravitycompilerplugin.cpp
    gravitycompileoperation.cpp
)

# final lib
add_library(GravityCompilerPlugin SHARED ${GravityCompilerPlugin_SRCS})

target_link_libraries(GravityCompilerPlugin Qt5::Core Qt5::Qml HemeraQt5SDK::Core HemeraQt5SDK::Generators HemeraQmlSettings)

set_target_properties(GravityCompilerPlugin PROPERTIES
                      SKIP_BUILD_RPATH FALSE
                      BUILD_WITH_INSTALL_RPATH TRUE
                      INSTALL_RPATH_USE_LINK_PATH TRUE
                      INSTALL_RPATH "${HEMERA_QML_SETTINGS_PLUGINS_DIR}/com/ispirata/Hemera/Settings")

# Install phase
install(TARGETS GravityCompilerPlugin
        RUNTIME DESTINATION "${INSTALL_BIN_DIR}" COMPONENT bin
        LIBRARY DESTINATION "${HEMERA_COMPILER_PLUGINS_DIR}" COMPONENT shlib
        ARCHIVE DESTINATION "${HEMERA_COMPILER_PLUGINS_DIR}" COMPONENT shlib
        COMPONENT GravityPlugins)

set(GravityCompilerPlugin_MISCFILES
    hemera-star-template.service.in
    hemera-orbit-template.service.in
    hemera-star-template.target.in
    hemera-orbit-template.target.in
    weston-star-template.service.in
    allowance-star-template.conf.in
    star-bus-policy-orbit-template.conf.in
    star-bus-policy-star-template.conf.in
)

# CMake modules
install(FILES ${GravityCompilerPlugin_MISCFILES}
        DESTINATION ${HA_SDK_DATA_DIR}
        COMPONENT data)
