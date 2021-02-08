#include "hemeraqmlsettingshyperspace.h"

#include "hemeraqmlsettingshyperspacetransport.h"

namespace Hemera {
namespace Qml {
namespace Settings {

Hyperspace::Hyperspace(QObject* parent)
    : QObject(parent)
{
}

Hyperspace::~Hyperspace()
{
}

QQmlListProperty< HyperspaceTransport > Hyperspace::transports()
{
    return QQmlListProperty<Hemera::Qml::Settings::HyperspaceTransport>(this, 0,
                                     [] (QQmlListProperty<Hemera::Qml::Settings::HyperspaceTransport> *list, HyperspaceTransport *transport) {
                                         Hyperspace *hyp = qobject_cast<Hyperspace *>(list->object);
                                         if (transport) {
                                             hyp->m_transports.append(transport);
                                         }
                                     },
                                     [] (QQmlListProperty<Hemera::Qml::Settings::HyperspaceTransport> *list) -> int {
                                         return qobject_cast<Hyperspace *>(list->object)->m_transports.count();
                                     },
                                     [] (QQmlListProperty<Hemera::Qml::Settings::HyperspaceTransport> *list, int at) -> HyperspaceTransport* {
                                         return qobject_cast<Hyperspace *>(list->object)->m_transports.at(at);
                                     },
                                     [] (QQmlListProperty<Hemera::Qml::Settings::HyperspaceTransport> *list) {
                                         Hyperspace *hyp = qobject_cast<Hyperspace *>(list->object);
                                         qDeleteAll(hyp->m_transports);
                                         hyp->m_transports.clear();
                                     });
}

}
}
}
