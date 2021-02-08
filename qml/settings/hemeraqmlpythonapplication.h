#ifndef HEMERA_QML_SETTINGS_PYTHONAPPLICATION_H
#define HEMERA_QML_SETTINGS_PYTHONAPPLICATION_H

#include "hemeraqmlapplication.h"

namespace Hemera {
namespace Qml {
namespace Settings {

class PythonApplicationPrivate;
class PythonApplication : public Hemera::Qml::Settings::Application
{
    Q_OBJECT
    Q_DISABLE_COPY(PythonApplication)
    Q_DECLARE_PRIVATE(PythonApplication)

    Q_PROPERTY(QString mainFile MEMBER m_mainFile)
    Q_PROPERTY(bool generatePropertyClass MEMBER m_generatePropertyClass)
    Q_PROPERTY(QString propertyClassInstallationPath MEMBER m_propertyClassInstallationPath)

public:
    explicit PythonApplication(QObject *parent = nullptr);
    virtual ~PythonApplication();

private:
    QString m_mainFile;
    bool m_generatePropertyClass;
    QString m_propertyClassInstallationPath;
};

}
}
}

#endif // HEMERA_QML_SETTINGS_PYTHONAPPLICATION_H
