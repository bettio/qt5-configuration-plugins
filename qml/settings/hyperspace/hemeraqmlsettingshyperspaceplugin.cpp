#include "hemeraqmlsettingshyperspaceplugin.h"

#include <QtQml/QQmlComponent>

// Components
#include "hemeraqmlsettingsastartegateway.h"
#include "hemeraqmlsettingsastartetransport.h"
#include "hemeraqmlsettingshttptransport.h"
#include "hemeraqmlsettingshyperspace.h"
#include "hemeraqmlsettingshyperspacetransport.h"
#include "hemeraqmlsettingsmqtttransport.h"
#include "hemeraqmlsettingsnetworklisten.h"

namespace Hemera {
namespace Qml {
namespace Settings {

void HyperspacePlugin::registerTypes(const char* uri)
{
    int versionMajor = 1;
    int versionMinor = 0;

    qmlRegisterType<AstarteGateway>(uri, versionMajor, versionMinor, "AstarteGateway");
    qmlRegisterType<AstarteTransport>(uri, versionMajor, versionMinor, "AstarteTransport");
    qmlRegisterType<Hyperspace>(uri, versionMajor, versionMinor, "Hyperspace");
    qmlRegisterType<HyperspaceTransport>(uri, versionMajor, versionMinor, "HyperspaceTransport");
    qmlRegisterType<HTTPTransport>(uri, versionMajor, versionMinor, "HTTPTransport");
    qmlRegisterType<MQTTTransport>(uri, versionMajor, versionMinor, "MQTTTransport");
    qmlRegisterType<NetworkListen>(uri, versionMajor, versionMinor, "NetworkListen");

    // Enums
    qmlRegisterUncreatableType<MQTTQoS>(uri, versionMajor, versionMinor, "MQTTQoS", QStringLiteral("Can't create this"));
    qmlRegisterUncreatableType<MQTTProtocol>(uri, versionMajor, versionMinor, "MQTTProtocol", QStringLiteral("Can't create this"));
}

}

}

}
