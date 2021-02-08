/*
 *
 */

#ifndef HEMERA_QML_SETTINGS_SIMPLEQMLAPPLICATION_H
#define HEMERA_QML_SETTINGS_SIMPLEQMLAPPLICATION_H

#include "hemeraqmlsimplecppapplication.h"

namespace Hemera {
namespace Qml {
namespace Settings {

class SimpleQmlApplicationPrivate;
/**
 * @class SimpleQmlApplication
 * @ingroup HemeraQmlSettings
 *
 * @brief SimpleQmlApplication is used to create a QtQuick2 QML-only application, with no C++ logic.
 *
 * If your QtQuick2 application features no C++ logic, you can use SimpleQmlApplication to automatically
 * build and generate an Hemera application from your QML files. Specify them as resourceFiles, and
 * point to the mainQmlFile, the rest will be done automatically.
 */
class SimpleQmlApplication : public Hemera::Qml::Settings::SimpleCppApplication
{
    Q_OBJECT
    Q_DISABLE_COPY(SimpleQmlApplication)
    Q_DECLARE_PRIVATE(SimpleQmlApplication)

    /**
     * The main QML file. It needs to be installed with resourceFiles, and will be the QML file started
     * by the engine when the application is launched.
     */
    Q_PROPERTY(QString mainQmlFile READ mainQmlFile WRITE setMainQmlFile)

public:
    explicit SimpleQmlApplication(QObject *parent = nullptr);
    virtual ~SimpleQmlApplication();

    QString mainQmlFile() const;
    void setMainQmlFile(const QString &name);
};

}
}
}

#endif // HEMERA_QML_SETTINGS_SIMPLEQMLAPPLICATION_H
