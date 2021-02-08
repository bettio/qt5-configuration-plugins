########## QML Settings Plugin

set(hemeraqmlsettings_SRCS
    hemeraqmlappliance.cpp
    hemeraqmlapplication.cpp
    hemeraqmlapplicationprofile.cpp
    hemeraqmldevice.cpp
    hemeraqmldirectfbdisplay.cpp
    hemeraqmldisplay.cpp
    hemeraqmleglfsdisplay.cpp
    hemeraqmlframebufferdisplay.cpp
    hemeraqmlguiappliance.cpp
    hemeraqmlhyperspaceqt5consumer.cpp
    hemeraqmlorbit.cpp
    hemeraqmllanguagesupport.cpp
    hemeraqmllinuxfbdisplay.cpp
    hemeraqmlproxiedapplication.cpp
    hemeraqmlpythonapplication.cpp
    hemeraqmlsandbox.cpp
    hemeraqmlsettingsplugin.cpp
    hemeraqmlsimplecppapplication.cpp
    hemeraqmlsimpleqmlapplication.cpp
    hemeraqmlsimpleqtquick1application.cpp
    hemeraqmlrootoperation.cpp
    hemeraqmlupdatestrategy.cpp
    hemeraqmlwatchdogapplicationprofile.cpp
    hemeraqmlrestartonfailureapplicationprofile.cpp
    hemeraqmlwaylanddisplay.cpp
    hemeraqmlx11display.cpp
    hemeraqmlwaylandonxdisplay.cpp
)

set(hemeraqmlsettings_QMLFILES
    Hemera/Settings/qmldir
    Hemera/Settings/com.ispirata.Hemera.Settings.qmltypes
)

set(hemeraqmlsettings_PVT_HEADERS
    hemeraqmlappliance.h
    hemeraqmlapplication.h
    hemeraqmlapplication_p.h
    hemeraqmlapplicationprofile.h
    hemeraqmldevice.h
    hemeraqmldirectfbdisplay.h
    hemeraqmldisplay.h
    hemeraqmleglfsdisplay.h
    hemeraqmlframebufferdisplay.h
    hemeraqmlguiappliance.h
    hemeraqmlorbit.h
    hemeraqmllanguagesupport.h
    hemeraqmllinuxfbdisplay.h
    hemeraqmlproxiedapplication.h
    hemeraqmlpythonapplication.h
    hemeraqmlsandbox.h
    hemeraqmlsimplecppapplication.h
    hemeraqmlsimplecppapplication_p.h
    hemeraqmlsimpleqmlapplication.h
    hemeraqmlsimpleqtquick1application.h
    hemeraqmlrootoperation.h
    hemeraqmlupdatestrategy.h
    hemeraqmlwatchdogapplicationprofile.h
    hemeraqmlrestartonfailureapplicationprofile.h
    hemeraqmlwaylanddisplay.h
    hemeraqmlx11display.h
    hemeraqmlwaylandonxdisplay.h
)

file(COPY ${hemeraqmlsettings_PVT_HEADERS} DESTINATION ${CMAKE_BINARY_DIR}/include/private/HemeraQmlSettings/)

# QML Plugin
add_library(HemeraQmlSettings SHARED ${hemeraqmlsettings_SRCS})

target_link_libraries(HemeraQmlSettings Qt5::Core Qt5::Network Qt5::Qml)

# Install plugin
install(TARGETS HemeraQmlSettings
        RUNTIME DESTINATION "${HEMERA_QML_SETTINGS_PLUGINS_DIR}/com/ispirata/Hemera/Settings" COMPONENT HemeraQt5SDK
        LIBRARY DESTINATION "${HEMERA_QML_SETTINGS_PLUGINS_DIR}/com/ispirata/Hemera/Settings" COMPONENT HemeraQt5SDK
        ARCHIVE DESTINATION "${HEMERA_QML_SETTINGS_PLUGINS_DIR}/com/ispirata/Hemera/Settings" COMPONENT HemeraQt5SDK
        COMPONENT HemeraQt5SDK)

# Install qmldir files
install(FILES ${hemeraqmlsettings_QMLFILES}
        DESTINATION "${HEMERA_QML_SETTINGS_PLUGINS_DIR}/com/ispirata/Hemera/Settings"
        COMPONENT HemeraQt5SDK)

add_subdirectory(hyperspace)
