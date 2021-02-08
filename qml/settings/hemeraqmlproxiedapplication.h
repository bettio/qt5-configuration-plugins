#ifndef HEMERA_QML_SETTINGS_PROXIEDAPPLICATION_H
#define HEMERA_QML_SETTINGS_PROXIEDAPPLICATION_H

#include "hemeraqmlapplication.h"

namespace Hemera {
namespace Qml {
namespace Settings {

class ProxiedApplicationPrivate;
/**
 * @class ProxiedApplication
 * @ingroup HemeraQmlSettings
 *
 * @brief ProxiedApplication allows to run any executable as if it was a proper Hemera application.
 *
 * The use of ProxiedApplication is generally unadvised: it shall be used only in those cases where there is
 * no SDK available for the language/application of choice, or where there is an explicit urgency of not making
 * the application depend on Hemera SDK.
 *
 * ProxiedApplication can be used as a buildsystem for the application package only through resource files: a common
 * use case for Java, for example, is to deploy all the class/jar files as resourceFiles, and let ha create the package
 * from there. The buildsystem will also take care of generating the correct proxy.
 *
 * @note Several concepts in ProxiedApplication are better explained in @ref BaseHemeraConcepts "Base Hemera Concepts"
 *
 * @sa Hemera::Qml::Settings::Application
 */
class ProxiedApplication : public Hemera::Qml::Settings::Application
{
    Q_OBJECT
    Q_DISABLE_COPY(ProxiedApplication)
    Q_DECLARE_PRIVATE(ProxiedApplication)

    /**
     * The command which should be executed for starting the application. You can use the token @RESOURCE_DIR@,
     * which will be replaced with the root of the resource directory. It is equivalent to resource:///.
     */
    Q_PROPERTY(QString command READ command WRITE setCommand)

public:
    explicit ProxiedApplication(QObject *parent = 0);
    virtual ~ProxiedApplication();

    QString command() const;
    void setCommand(const QString &command);

protected:
    explicit ProxiedApplication(ProxiedApplicationPrivate &dd, QObject *parent);
};

}
}
}

#endif // HEMERA_QML_SETTINGS_PROXIEDAPPLICATION_H
