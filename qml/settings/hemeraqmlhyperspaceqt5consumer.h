#ifndef HEMERA_QML_SETTINGS_HYPERSPACEQT5CONSUMER_H
#define HEMERA_QML_SETTINGS_HYPERSPACEQT5CONSUMER_H

#include <QtCore/QObject>

namespace Hemera {
namespace Qml {
namespace Settings {

/**
 * @class HyperspaceQt5Consumer
 * @ingroup HemeraQmlSettings
 *
 * @brief HyperspaceQt5Consumer declares and generates an Hyperspace Consumer for Qt5 Applications.
 *
 * This item can be only used in combination with a @ref SimpleCppApplication or one of its subclasses,
 * to declare a Hyperspace Consumer which will be generated as a Qt5 class.
 *
 * @sa Hemera::Qml::Settings::SimpleCppApplication
 * @sa Hyperspace::ProducerConsumer::ConsumerAbstractAdaptor
 */
class HyperspaceQt5Consumer : public QObject
{
    Q_OBJECT
    Q_DISABLE_COPY(HyperspaceQt5Consumer)

    /**
     * The interface file - it should point to a json file representing the interface to be generated.
     */
    Q_PROPERTY(QString interfaceFile MEMBER m_interface)
    /// The header file of the class being adapted.
    Q_PROPERTY(QString header MEMBER m_header)
    /// The class name of the class being adapted.
    Q_PROPERTY(QString className MEMBER m_className)

public:
    explicit HyperspaceQt5Consumer(QObject *parent = 0);
    virtual ~HyperspaceQt5Consumer();

private:
    QString m_interface;
    QString m_header;
    QString m_className;
};
}
}
}

#endif // HEMERA_QML_SETTINGS_HYPERSPACEQT5CONSUMER_H
