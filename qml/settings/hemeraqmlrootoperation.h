#ifndef HEMERA_QML_SETTINGS_ROOTOPERATION_H
#define HEMERA_QML_SETTINGS_ROOTOPERATION_H

#include <QtCore/QObject>

#include "hemeraqmlsimplecppapplication.h"

namespace Hemera {
namespace Qml {
namespace Settings {

/*
 * Internal note: There's a reason why this class does not inherit Application, Sandbox or the likes.
 * Although it is true the mechanics for compilation are identical, there are a few bottlenecks:
 *
 *  1. This is an internal, Socket+Systemd activated process. It has nothing to do with the concept of
 *     Hemera Sandboxes or Applications. On the other hand, it is actually related to a single application.
 *  2. You don't want to compile accidentally a RootOperation as a Sandbox
 *  3. Consistency.
 *
 * Deal with it.
 */

/**
 * @class RootOperation
 * @ingroup HemeraQmlSettings
 *
 * @brief RootOperation is the base object for declaring root operations inside a Hemera Application.
 *
 * Hemera Applications are never run as root for a number of reasons, among which the fact that it
 * would pose a security threat. However, at times it might be necessary to perform some operations as root.
 *
 * This class allows to declare a RootOperation to be built and registered with its associated application.
 * When declared inside a buildsystem ha, it also packs in the logic for building the helper.
 *
 * @sa Hemera::Qml::Settings::Application
 * @sa Hemera::RootOperation
 * @sa Hemera::RootOperationClient
 */
class RootOperation : public QObject
{
    Q_OBJECT
    Q_DISABLE_COPY(RootOperation)

    /**
     * The operation ID. It is compulsory, and it has to be an unique identifier in
     * [reverse domain name notation](http://en.wikipedia.org/wiki/Reverse_domain_name_notation).
     */
    Q_PROPERTY(QString operationId MEMBER m_operationId)
    /// A list of paths, relative to the ha file, of source files to be compiled
    Q_PROPERTY(QStringList sourceFiles MEMBER m_sourceFiles)
    /// A list of library names to link to. Usually not advised, use pkgconfig or modules if you can.
    Q_PROPERTY(QStringList libraries MEMBER m_libraries)
    /// A list of additional include paths.
    Q_PROPERTY(QStringList includePaths MEMBER m_includePaths)
    /// A list of additional pkgconfig modules to include
    Q_PROPERTY(QStringList pkgconfig MEMBER m_pkgconfig)
    /// List of HemeraQt5SDK modules to use
    Q_PROPERTY(Hemera::Qml::Settings::HemeraModules::Modules hemeraModules MEMBER m_hemeraModules)
    /// List of Qt5 modules to use
    Q_PROPERTY(Hemera::Qml::Settings::QtModules::Modules qtModules MEMBER m_qtModules)

public:
    explicit RootOperation(QObject *parent = 0);
    virtual ~RootOperation();

private:
    QString m_operationId;
    QStringList m_sourceFiles;
    QStringList m_includePaths;
    QStringList m_libraries;
    QStringList m_pkgconfig;

    QtModules::Modules m_qtModules;
    HemeraModules::Modules m_hemeraModules;
};
}
}
}

#endif // HEMERA_QML_SETTINGS_ROOTOPERATION_H
