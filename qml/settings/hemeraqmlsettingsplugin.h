#ifndef HEMERA_QML_HEMERAQMLSETTINGSPLUGIN_H
#define HEMERA_QML_HEMERAQMLSETTINGSPLUGIN_H

#include <QtQml/QQmlExtensionPlugin>

namespace Hemera {

namespace Qml {

namespace Settings
{

class SettingsPlugin : public QQmlExtensionPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "org.qt-project.Qt.QQmlExtensionInterface")

public:
    void registerTypes(const char *uri);
};

}

}

}

#endif // HEMERA_QML_HEMERAQMLSETTINGSPLUGIN_H
