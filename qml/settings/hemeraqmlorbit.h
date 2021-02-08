#ifndef HEMERA_QML_SETTINGS_HEMERAQMLORBIT_H
#define HEMERA_QML_SETTINGS_HEMERAQMLORBIT_H

#include "hemeraqmlsandbox.h"

namespace Hemera {

namespace Qml {

namespace Settings {

/**
 * @class Orbit
 * @ingroup HemeraQmlSettings
 *
 * @brief Orbit is a specialization of Sandbox which can contain any number of applications.
 *
 * It should be used in those cases where Orbital Applications are unsuitable.
 *
 * @note Several concepts in Orbit are better explained in @ref BaseHemeraConcepts "Base Hemera Concepts"
 *
 * @sa Hemera::Qml::Settings::Sandbox
 */
class Orbit : public Sandbox
{
    Q_OBJECT
    Q_DISABLE_COPY(Orbit)

    /// A list of application IDs which should run in this Orbit
    Q_PROPERTY(QStringList applications MEMBER m_applications)

    Q_CLASSINFO("DefaultProperty", "languageSupport")

public:
    explicit Orbit(QObject *parent = 0);
    virtual ~Orbit();

private:
    QStringList m_applications;

    friend class Gravity;
};

}

}

}

#endif // HEMERA_QML_SETTINGS_HEMERAQMLORBIT_H
