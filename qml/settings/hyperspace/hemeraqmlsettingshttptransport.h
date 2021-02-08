#ifndef HEMERA_QML_SETTINGS_HTTPTRANSPORT_H
#define HEMERA_QML_SETTINGS_HTTPTRANSPORT_H

#include "hemeraqmlsettingshyperspacetransport.h"

#include <QtQml/QQmlListProperty>

namespace Hemera {
namespace Qml {
namespace Settings {

class NetworkListen;

class HTTPTransport : public HyperspaceTransport
{
    Q_OBJECT

    Q_PROPERTY(QQmlListProperty<Hemera::Qml::Settings::NetworkListen> networkListeners READ networkListeners)

    Q_CLASSINFO("DefaultProperty", "networkListeners")

public:
    explicit HTTPTransport(QObject *parent = 0);
    virtual ~HTTPTransport();

    QQmlListProperty<Hemera::Qml::Settings::NetworkListen> networkListeners();

private:
    QList< NetworkListen* > m_listeners;
};
}
}
}

#endif // HEMERA_QML_SETTINGS_HTTPTRANSPORT_H
