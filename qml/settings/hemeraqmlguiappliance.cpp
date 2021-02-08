#include "hemeraqmlguiappliance.h"

#include "hemeraqmldisplay.h"

namespace Hemera
{

namespace Qml
{

namespace Settings
{

GUIAppliance::GUIAppliance(QObject* parent)
    : Appliance(parent)
{
}

GUIAppliance::~GUIAppliance()
{
}

QQmlListProperty< Hemera::Qml::Settings::Display > GUIAppliance::displays()
{
    return QQmlListProperty<Hemera::Qml::Settings::Display>(this, 0,
                                     [] (QQmlListProperty<Hemera::Qml::Settings::Display> *list, Display *orbit) {
                                         GUIAppliance *ga = qobject_cast<GUIAppliance *>(list->object);
                                         if (orbit) {
                                             ga->m_displays.append(orbit);
                                         }
                                     },
                                     [] (QQmlListProperty<Hemera::Qml::Settings::Display> *list) -> int {
                                         return qobject_cast<GUIAppliance *>(list->object)->m_displays.count();
                                     },
                                     [] (QQmlListProperty<Hemera::Qml::Settings::Display> *list, int at) -> Display* {
                                         return qobject_cast<GUIAppliance *>(list->object)->m_displays.at(at);
                                     },
                                     [] (QQmlListProperty<Hemera::Qml::Settings::Display> *list) {
                                         GUIAppliance *ga = qobject_cast<GUIAppliance *>(list->object);
                                         qDeleteAll(ga->m_displays);
                                         ga->m_displays.clear();
                                     });
}

}

}

}
