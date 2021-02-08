########## QML Settings Hyperspace Plugin
set(hemeraqmlsettingshyperspace_SRCS
    hemeraqmlsettingsastartegateway.cpp
    hemeraqmlsettingsastartetransport.cpp
    hemeraqmlsettingshttptransport.cpp
    hemeraqmlsettingshyperspace.cpp
    hemeraqmlsettingshyperspaceplugin.cpp
    hemeraqmlsettingshyperspacetransport.cpp
    hemeraqmlsettingsmqtttransport.cpp
    hemeraqmlsettingsnetworklisten.cpp
)

set(hemeraqmlsettingshyperspace_QMLFILES
    Hemera/Settings/Hyperspace/qmldir
    Hemera/Settings/Hyperspace/com.ispirata.Hemera.Settings.Hyperspace.qmltypes
)

set(hemeraqmlsettingshyperspace_PVT_HEADERS
)

# QML Plugin
if (ENABLE_HEMERA_WEB_SDK_COVERAGE)
    add_library(HemeraQmlSettingsHyperspace STATIC ${hemeraqmlsettingshyperspace_SRCS})
    target_link_libraries(HemeraQmlSettingsHyperspace gcov)
else ()
    add_library(HemeraQmlSettingsHyperspace SHARED ${hemeraqmlsettingshyperspace_SRCS})
endif ()

target_link_libraries(HemeraQmlSettingsHyperspace Qt5::Core Qt5::Network Qt5::DBus Qt5::Qml HemeraQt5SDK::Core)

# Install plugin
install(TARGETS HemeraQmlSettingsHyperspace
        RUNTIME DESTINATION "${HEMERA_QML_SETTINGS_PLUGINS_DIR}/com/ispirata/Hemera/Settings/Hyperspace" COMPONENT bin
        LIBRARY DESTINATION "${HEMERA_QML_SETTINGS_PLUGINS_DIR}/com/ispirata/Hemera/Settings/Hyperspace" COMPONENT shlib
        ARCHIVE DESTINATION "${HEMERA_QML_SETTINGS_PLUGINS_DIR}/com/ispirata/Hemera/Settings/Hyperspace" COMPONENT shlib
        COMPONENT HemeraHyperspaceSDK)

# Install qmldir files
install(FILES ${hemeraqmlsettingshyperspace_QMLFILES}
        DESTINATION "${HEMERA_QML_SETTINGS_PLUGINS_DIR}/com/ispirata/Hemera/Settings/Hyperspace"
        COMPONENT HemeraHyperspaceSDK)

# Install private headers, if requested
if (HEMERA_QT5_SDK_PRIVATE_COMPONENTS)
    install(FILES ${hemeraqmlsettingshyperspace_PVT_HEADERS}
            DESTINATION "${INSTALL_INCLUDE_DIR}/HemeraHyperspaceSDK/private/HemeraQmlSettingsHyperspace"
            COMPONENT private)
endif ()
