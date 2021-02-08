/*
 *
 */

#ifndef HEMERA_QML_SETTINGS_SIMPLEQTQUICK1APPLICATION_H
#define HEMERA_QML_SETTINGS_SIMPLEQTQUICK1APPLICATION_H

#include "hemeraqmlsimplecppapplication.h"

namespace Hemera {
namespace Qml {
namespace Settings {

class SimpleQtQuick1ApplicationPrivate;
/**
 * @class SimpleQtQuick1Application
 * @ingroup HemeraQmlSettings
 *
 * @brief SimpleQtQuick1Application is used to create a QtQuick1 QML-only application, with no C++ logic.
 *
 * If your QtQuick1 application features no C++ logic, you can use SimpleQmlApplication to automatically
 * build and generate an Hemera application from your QML files. Specify them as resourceFiles, and
 * point to the mainQmlFile, the rest will be done automatically.
 */
class SimpleQtQuick1Application : public Hemera::Qml::Settings::SimpleCppApplication
{
    Q_OBJECT
    Q_DISABLE_COPY(SimpleQtQuick1Application)
    Q_DECLARE_PRIVATE(SimpleQtQuick1Application)

    /**
     * The main QML file. It needs to be installed with resourceFiles, and will be the QML file started
     * by the engine when the application is launched.
     */
    Q_PROPERTY(QString mainQmlFile READ mainQmlFile WRITE setMainQmlFile)

public:
    explicit SimpleQtQuick1Application(QObject *parent = nullptr);
    virtual ~SimpleQtQuick1Application();

    QString mainQmlFile() const;
    void setMainQmlFile(const QString &name);
};

}
}
}

#endif // HEMERA_QML_SETTINGS_SIMPLEQTQUICK1APPLICATION_H
