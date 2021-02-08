/*
 *
 */

#ifndef HEMERA_QML_SETTINGS_LANGUAGESUPPORT_H
#define HEMERA_QML_SETTINGS_LANGUAGESUPPORT_H

#include <QtCore/QObject>

namespace Hemera {
namespace Qml {
namespace Settings {

class LanguageSupport : public QObject
{
    Q_OBJECT
    Q_DISABLE_COPY(LanguageSupport)

public:
    explicit LanguageSupport(QObject* parent = 0);
    virtual ~LanguageSupport();
};
}
}
}

#endif // HEMERA_QML_SETTINGS_LANGUAGESUPPORT_H
