#ifndef HEMERA_QML_APPLICATIONPROFILE_H
#define HEMERA_QML_APPLICATIONPROFILE_H

#include <QtCore/QObject>

namespace Hemera {

namespace Qml {

namespace Settings {

class ApplicationProfile : public QObject
{
    Q_OBJECT
    Q_DISABLE_COPY(ApplicationProfile)

public:
    explicit ApplicationProfile(QObject *parent = 0);
    virtual ~ApplicationProfile();

private:
    class Private;
    Private * const d;
};

};

};

};

#endif // HEMERA_QML_APPLICATIONPROFILE_H
