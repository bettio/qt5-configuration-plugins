#include "hemeraqmlsettingsplugin.h"

#include <QtQml/QQmlComponent>

// Components
#include "hemeraqmlappliance.h"
#include "hemeraqmlapplication.h"
#include "hemeraqmlapplicationprofile.h"
#include "hemeraqmldevice.h"
#include "hemeraqmldirectfbdisplay.h"
#include "hemeraqmldisplay.h"
#include "hemeraqmleglfsdisplay.h"
#include "hemeraqmlframebufferdisplay.h"
#include "hemeraqmlguiappliance.h"
#include "hemeraqmlhyperspaceqt5consumer.h"
#include "hemeraqmllanguagesupport.h"
#include "hemeraqmllinuxfbdisplay.h"
#include "hemeraqmlorbit.h"
#include "hemeraqmlproxiedapplication.h"
#include "hemeraqmlpythonapplication.h"
#include "hemeraqmlrestartonfailureapplicationprofile.h"
#include "hemeraqmlrootoperation.h"
#include "hemeraqmlsandbox.h"
#include "hemeraqmlsimplecppapplication.h"
#include "hemeraqmlsimpleqmlapplication.h"
#include "hemeraqmlsimpleqtquick1application.h"
#include "hemeraqmlupdatestrategy.h"
#include "hemeraqmlwatchdogapplicationprofile.h"
#include "hemeraqmlwaylanddisplay.h"
#include "hemeraqmlwaylandonxdisplay.h"
#include "hemeraqmlx11display.h"

namespace Hemera
{

namespace Qml
{

namespace Settings
{

void SettingsPlugin::registerTypes(const char* uri)
{
    int versionMajor = 1;
    int versionMinor = 0;

    qmlRegisterType<Appliance>(uri, versionMajor, versionMinor, "Appliance");
    qmlRegisterType<Application>(uri, versionMajor, versionMinor, "Application");
    qmlRegisterType<ApplicationProfile>(uri, versionMajor, versionMinor, "ApplicationProfile");
    qmlRegisterType<Device>(uri, versionMajor, versionMinor, "Device");
    qmlRegisterType<DirectFBDisplay>(uri, versionMajor, versionMinor, "DirectFBDisplay");
    qmlRegisterType<EGLFSDisplay>(uri, versionMajor, versionMinor, "EGLFSDisplay");
    qmlRegisterType<GUIAppliance>(uri, versionMajor, versionMinor, "GUIAppliance");
    qmlRegisterType<HyperspaceQt5Consumer>(uri, versionMajor, versionMinor, "HyperspaceQt5Consumer");
    qmlRegisterType<LinuxFBDisplay>(uri, versionMajor, versionMinor, "LinuxFBDisplay");
    qmlRegisterType<Orbit>(uri, versionMajor, versionMinor, "Orbit");
    qmlRegisterType<ProxiedApplication>(uri, versionMajor, versionMinor, "ProxiedApplication");
    qmlRegisterType<PythonApplication>(uri, versionMajor, versionMinor, "PythonApplication");
    qmlRegisterType<RestartOnFailureProfile>(uri, versionMajor, versionMinor, "RestartOnFailureProfile");
    qmlRegisterType<RootOperation>(uri, versionMajor, versionMinor, "RootOperation");
    qmlRegisterType<SimpleCppApplication>(uri, versionMajor, versionMinor, "SimpleCppApplication");
    qmlRegisterType<SimpleQmlApplication>(uri, versionMajor, versionMinor, "SimpleQmlApplication");
    qmlRegisterType<SimpleQtQuick1Application>(uri, versionMajor, versionMinor, "SimpleQtQuick1Application");
    qmlRegisterType<ImageStoreUpdateStrategy>(uri, versionMajor, versionMinor, "ImageStoreUpdateStrategy");
    qmlRegisterType<MassStorageUpdateStrategy>(uri, versionMajor, versionMinor, "MassStorageUpdateStrategy");
    qmlRegisterType<AstarteUpdateStrategy>(uri, versionMajor, versionMinor, "AstarteUpdateStrategy");
    qmlRegisterType<WatchdogApplicationProfile>(uri, versionMajor, versionMinor, "WatchdogApplicationProfile");
    qmlRegisterType<WaylandDisplay>(uri, versionMajor, versionMinor, "WaylandDisplay");
    qmlRegisterType<X11Display>(uri, versionMajor, versionMinor, "X11Display");
    qmlRegisterType<WaylandOnXDisplay>(uri, versionMajor, versionMinor, "WaylandOnXDisplay");

    // Enums
    qmlRegisterUncreatableType<HemeraModules>(uri, versionMajor, versionMinor, "HemeraModules", QStringLiteral("Can't create this"));
    qmlRegisterUncreatableType<OrbitFeatures>(uri, versionMajor, versionMinor, "Features", QStringLiteral("Can't create this"));
    qmlRegisterUncreatableType<PowerSavingStrategy>(uri, versionMajor, versionMinor, "PowerSavingStrategy", QStringLiteral("Can't create this"));
    qmlRegisterUncreatableType<QtModules>(uri, versionMajor, versionMinor, "QtModules", QStringLiteral("Can't create this"));
    qmlRegisterUncreatableType<HyperspaceModules>(uri, versionMajor, versionMinor, "HyperspaceModules", QStringLiteral("Can't create this"));

    // Interfaces
    qmlRegisterUncreatableType<Display>(uri, versionMajor, versionMinor, "Display",
                                        QStringLiteral("Display is an interface, and cannot be created directly."));
    qmlRegisterUncreatableType<FramebufferDisplay>(uri, versionMajor, versionMinor, "FramebufferDisplay",
                                                   QStringLiteral("FramebufferDisplay is an interface, and cannot be created directly."));
    qmlRegisterUncreatableType<LanguageSupport>(uri, versionMajor, versionMinor, "LanguageSupport",
                                                QStringLiteral("LanguageSupport is an interface, and cannot be created directly."));
    qmlRegisterUncreatableType<Sandbox>(uri, versionMajor, versionMinor, "Sandbox",
                                        QStringLiteral("Sandbox is an interface, and cannot be created directly."));
    qmlRegisterUncreatableType<UpdateStrategy>(uri, versionMajor, versionMinor, "UpdateStrategy",
                                        QStringLiteral("UpdateStrategy is an interface, and cannot be created directly."));
}

}

}

}
