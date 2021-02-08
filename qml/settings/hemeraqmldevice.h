#ifndef HEMERA_QML_SETTINGS_DEVICE_H
#define HEMERA_QML_SETTINGS_DEVICE_H

#include <QtCore/QObject>

namespace Hemera {

namespace Qml {

namespace Settings {

/**
 * @class Device
 * @ingroup HemeraQmlSettings
 *
 * @brief Represents a Linux device. Used to configure access to devices in Sandbox.
 *
 * @sa Sandbox
 */
class Device : public QObject
{
    Q_OBJECT
    Q_DISABLE_COPY(Device)

    /// Path to the device. Usually, /dev/someDevice0
    Q_PROPERTY(QString path READ path WRITE setPath)
    /// Whether the Sandbox can read from the device
    Q_PROPERTY(bool canRead READ canRead WRITE setCanRead)
    /// Whether the Sandbox can write to the device
    Q_PROPERTY(bool canWrite READ canWrite WRITE setCanWrite)
    /// Whether the Sandbox can create the device
    Q_PROPERTY(bool canCreate READ canCreate WRITE setCanCreate)

public:
    explicit Device(QObject *parent = nullptr);
    virtual ~Device();

    QString path() const;
    void setPath(const QString &path);

    bool canRead() const;
    void setCanRead(bool canRead);

    bool canWrite() const;
    void setCanWrite(bool canWrite);

    bool canCreate() const;
    void setCanCreate(bool canCreate);

private:
    class Private;
    Private * const d;
};

}

}

}

#endif // HEMERA_QML_SETTINGS_DEVICE_H
