#include "hemeraqmlsettingshttptransport.h"

#include "hemeraqmlsettingsnetworklisten.h"

namespace Hemera {
namespace Qml {
namespace Settings {

HTTPTransport::HTTPTransport(QObject* parent)
    : HyperspaceTransport(parent)
{

}

HTTPTransport::~HTTPTransport()
{

}

QQmlListProperty< NetworkListen > HTTPTransport::networkListeners()
{
    return QQmlListProperty<Hemera::Qml::Settings::NetworkListen>(this, 0,
                                     [] (QQmlListProperty<Hemera::Qml::Settings::NetworkListen> *list, NetworkListen *transport) {
                                         HTTPTransport *http = qobject_cast<HTTPTransport *>(list->object);
                                         if (transport) {
                                             http->m_listeners.append(transport);
                                         }
                                     },
                                     [] (QQmlListProperty<Hemera::Qml::Settings::NetworkListen> *list) -> int {
                                         return qobject_cast<HTTPTransport *>(list->object)->m_listeners.count();
                                     },
                                     [] (QQmlListProperty<Hemera::Qml::Settings::NetworkListen> *list, int at) -> NetworkListen* {
                                         return qobject_cast<HTTPTransport *>(list->object)->m_listeners.at(at);
                                     },
                                     [] (QQmlListProperty<Hemera::Qml::Settings::NetworkListen> *list) {
                                         HTTPTransport *http = qobject_cast<HTTPTransport *>(list->object);
                                         qDeleteAll(http->m_listeners);
                                         http->m_listeners.clear();
                                     });
}

}
}
}
