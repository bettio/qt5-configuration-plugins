#ifndef HEMERA_QML_SETTINGS_HEMERAQMLUPDATESTRATEGY_H
#define HEMERA_QML_SETTINGS_HEMERAQMLUPDATESTRATEGY_H

#include <QtCore/QObject>
#include <QtQml/QQmlListProperty>

namespace Hemera {
namespace Qml {
namespace Settings {

class UpdateStrategy : public QObject
{
    Q_OBJECT
    Q_DISABLE_COPY(UpdateStrategy)

public:
    explicit UpdateStrategy(QObject *parent = nullptr);
    virtual ~UpdateStrategy();

private:
};

// Specific update strategies
class ImageStoreUpdateStrategy : public UpdateStrategy
{
    Q_OBJECT
    Q_DISABLE_COPY(ImageStoreUpdateStrategy)

    Q_PROPERTY(QString endpoint MEMBER m_endpoint)
    Q_PROPERTY(QString apiKey MEMBER m_apiKey)

public:
    explicit ImageStoreUpdateStrategy(QObject *parent = nullptr);
    virtual ~ImageStoreUpdateStrategy();

private:
    QString m_endpoint;
    QString m_apiKey;
};

class MassStorageUpdateStrategy : public UpdateStrategy
{
    Q_OBJECT
    Q_DISABLE_COPY(MassStorageUpdateStrategy)

    Q_PROPERTY(bool automount MEMBER m_automount)

public:
    explicit MassStorageUpdateStrategy(QObject *parent = nullptr);
    virtual ~MassStorageUpdateStrategy();

private:
    bool m_automount;
};

class AstarteUpdateStrategy : public UpdateStrategy
{
    Q_OBJECT
    Q_DISABLE_COPY(AstarteUpdateStrategy)

public:
    explicit AstarteUpdateStrategy(QObject *parent = nullptr);
    virtual ~AstarteUpdateStrategy();
};

}
}
}

#endif // HEMERA_QML_SETTINGS_HEMERAQMLUPDATESTRATEGY_H
