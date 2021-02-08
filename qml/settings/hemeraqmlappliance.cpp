#include "hemeraqmlappliance.h"

#include "hemeraqmlupdatestrategy.h"

namespace Hemera
{

namespace Qml
{

namespace Settings
{

Appliance::Appliance(QObject* parent)
    : QObject(parent)
{
}

Appliance::~Appliance()
{
}

QQmlListProperty< Hemera::Qml::Settings::UpdateStrategy > Appliance::updateStrategies()
{
    return QQmlListProperty<Hemera::Qml::Settings::UpdateStrategy>(this, 0,
                                     [] (QQmlListProperty<Hemera::Qml::Settings::UpdateStrategy> *list, UpdateStrategy *orbit) {
                                         Appliance *a = qobject_cast<Appliance *>(list->object);
                                         if (orbit) {
                                             a->m_updateStrategies.append(orbit);
                                         }
                                     },
                                     [] (QQmlListProperty<Hemera::Qml::Settings::UpdateStrategy> *list) -> int {
                                         return qobject_cast<Appliance *>(list->object)->m_updateStrategies.count();
                                     },
                                     [] (QQmlListProperty<Hemera::Qml::Settings::UpdateStrategy> *list, int at) -> UpdateStrategy* {
                                         return qobject_cast<Appliance *>(list->object)->m_updateStrategies.at(at);
                                     },
                                     [] (QQmlListProperty<Hemera::Qml::Settings::UpdateStrategy> *list) {
                                         Appliance *a = qobject_cast<Appliance *>(list->object);
                                         qDeleteAll(a->m_updateStrategies);
                                         a->m_updateStrategies.clear();
                                     });
}

}

}

}
