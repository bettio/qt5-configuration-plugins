#ifndef HEMERA_QML_SETTINGS_HYPERSPACETRANSPORT_H
#define HEMERA_QML_SETTINGS_HYPERSPACETRANSPORT_H

#include <QtCore/QObject>

namespace Hemera {
namespace Qml {
namespace Settings {

class HyperspaceTransport : public QObject
{
    Q_OBJECT

    Q_PROPERTY(bool discoverable MEMBER m_discoverable)

public:
    explicit HyperspaceTransport(QObject *parent = 0);
    virtual ~HyperspaceTransport();

private:
    bool m_discoverable;
};
}
}
}

#endif // HEMERA_QML_SETTINGS_HYPERSPACETRANSPORT_H
