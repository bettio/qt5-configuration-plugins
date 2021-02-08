#include "hemeraqmlsandbox.h"

#include "hemeraqmldevice.h"

namespace Hemera
{

namespace Qml
{

namespace Settings
{

Sandbox::Sandbox(QObject *parent)
    : QObject(parent)
    , m_keepAlive(false)
    , m_features(OrbitFeatures::NoFeatures)
    , m_environment(QString())
    , m_allowUpdates(false)
    , m_runAsRoot(false)
    , m_IUnderstandItIsASecurityRisk(false)
{
}

Sandbox::~Sandbox()
{
}

QQmlListProperty<Device> Sandbox::devices()
{
    return QQmlListProperty<Hemera::Qml::Settings::Device>(this, 0,
                                                        [] (QQmlListProperty<Hemera::Qml::Settings::Device> *list, Device *device) {
                                                            Sandbox *sandbox = qobject_cast<Sandbox*>(list->object);
                                                            if (device) {
                                                                sandbox->m_devices.append(device);
                                                            }
                                                        },
                                                        [] (QQmlListProperty<Hemera::Qml::Settings::Device> *list) -> int {
                                                            return qobject_cast<Sandbox*>(list->object)->m_devices.count();
                                                        },
                                                        [] (QQmlListProperty<Hemera::Qml::Settings::Device> *list, int at) -> Device* {
                                                            return qobject_cast<Sandbox*>(list->object)->m_devices.at(at);
                                                        },
                                                        [] (QQmlListProperty<Hemera::Qml::Settings::Device> *list) {
                                                            Sandbox *sandbox = qobject_cast<Sandbox*>(list->object);
                                                            qDeleteAll(sandbox->m_devices);
                                                            sandbox->m_devices.clear();
                                                        });
}

QQmlListProperty< LanguageSupport > Sandbox::languageSupport()
{
    return QQmlListProperty<Hemera::Qml::Settings::LanguageSupport>(this, 0,
                                     [] (QQmlListProperty<Hemera::Qml::Settings::LanguageSupport> *list, LanguageSupport *ls) {
                                         Sandbox *sandbox = qobject_cast<Sandbox *>(list->object);
                                         if (ls) {
                                             sandbox->m_languageSupport.append(ls);
                                         }
                                     },
                                     [] (QQmlListProperty<Hemera::Qml::Settings::LanguageSupport> *list) -> int {
                                         return qobject_cast<Sandbox *>(list->object)->m_languageSupport.count();
                                     },
                                     [] (QQmlListProperty<Hemera::Qml::Settings::LanguageSupport> *list, int at) -> LanguageSupport* {
                                         return qobject_cast<Sandbox *>(list->object)->m_languageSupport.at(at);
                                     },
                                     [] (QQmlListProperty<Hemera::Qml::Settings::LanguageSupport> *list) {
                                         Sandbox *sandbox = qobject_cast<Sandbox *>(list->object);
                                         qDeleteAll(sandbox->m_languageSupport);
                                         sandbox->m_languageSupport.clear();
                                     });
}

}

}

}
