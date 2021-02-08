#ifndef HEMERA_QML_SETTINGS_HYPERSPACE_H
#define HEMERA_QML_SETTINGS_HYPERSPACE_H

#include <QtCore/QObject>

#include <QtQml/QQmlListProperty>

namespace Hemera {
namespace Qml {
namespace Settings {

class HyperspaceTransport;

class Hyperspace : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QQmlListProperty<Hemera::Qml::Settings::HyperspaceTransport> transports READ transports)

    Q_CLASSINFO("DefaultProperty", "transports")

public:
    explicit Hyperspace(QObject *parent = 0);
    virtual ~Hyperspace();

    QQmlListProperty<Hemera::Qml::Settings::HyperspaceTransport> transports();

private:
    QList< HyperspaceTransport* > m_transports;
};

}
}
}

#endif // HEMERA_QML_SETTINGS_HYPERSPACE_H
