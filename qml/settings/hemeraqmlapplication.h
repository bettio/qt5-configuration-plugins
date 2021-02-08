#ifndef HEMERA_QML_HEMERAQMLAPPLICATION_H
#define HEMERA_QML_HEMERAQMLAPPLICATION_H

#include <QtCore/QObject>
#include <QtCore/QStringList>
#include <QtQml/QQmlListProperty>

#include "hemeraqmlsandbox.h"

namespace Hemera {

namespace Qml {

namespace Settings {

class ApplicationProfile;
class WatchdogApplicationProfile;
class RootOperation;

class ApplicationPrivate;
/**
 * @class Application
 * @ingroup HemeraQmlSettings
 *
 * @brief Application is the base object for application manifest files (ha). Each ha file is an Application object.
 *
 * Application is the simplest form of an ha file, and should be used in those cases where you are handling the buildsystem
 * yourself. Use one of Application's subclasses if you want to use ha as a buildsystem.
 *
 * Application is a Sandbox, and you can use all of Sandbox's parameters to configure the Orbital Application which will
 * be generated, unless _generateOrbit_ is false. The most common use case is specifying _features_ to configure the
 * Orbit.
 *
 * @note Several concepts in Application are better explained in @ref BaseHemeraConcepts "Base Hemera Concepts"
 *
 * @sa SimpleCppApplication
 */
class Application : public Sandbox
{
    Q_OBJECT
    Q_DISABLE_COPY(Application)
    Q_DECLARE_PRIVATE(Application)

    // TBD
    Q_PROPERTY(QQmlListProperty<Hemera::Qml::Settings::ApplicationProfile> applicationProfiles READ applicationProfiles)
    /**
     * The application ID. It is compulsory, and it has to be an unique identifier in
     * [reverse domain name notation](http://en.wikipedia.org/wiki/Reverse_domain_name_notation). Also, the
     * ha file must be named after this exact ID. This is done to prevent potential typos.
     */
    Q_PROPERTY(QString applicationId READ applicationId WRITE setApplicationId)
    /// The version of the application
    Q_PROPERTY(QString version READ version WRITE setVersion)
    /// The icon of the application. It should be a relative path to the ha, pointing to an image file.
    Q_PROPERTY(QString icon READ icon WRITE setIcon)
    /// The vendor, or owner, of the application.
    Q_PROPERTY(QString organization READ organization WRITE setOrganization)
    /// A short description of the application
    Q_PROPERTY(QString description READ description WRITE setDescription)
    /**
     * Resource files of the application. This setting is meaningful only if ha is being used as a buildsystem.
     * A list of relative paths to files which will be installed along the application.
     * From within the application, these files will be available under resource:///, retaining the very same relative
     * path.
     *
     * An example is:
     *
     * @code
     * resourceFiles: [ "images/myimg.png", "data/stuff/myData.xml" ]
     * @endcode
     *
     * Will install those files, which can be accessed by the application at resource:///images/myimg.png and
     * resource:///data/stuff/myData.xml.
     */
    Q_PROPERTY(QStringList resourceFiles READ resourceFiles WRITE setResourceFiles)
    /**
     * A list of RootOperations to be compiled and associated with this application.
     *
     * @sa Hemera::Qml::Settings::RootOperation
     */
    Q_PROPERTY(QQmlListProperty<Hemera::Qml::Settings::RootOperation> rootOperations READ rootOperations)
    Q_PROPERTY(QString workingDirectory READ workingDirectory WRITE setWorkingDirectory)
    /// Whether to generate or not an Orbital Application. Defaults to true.
    Q_PROPERTY(bool generateOrbit MEMBER m_generateOrbit)
    Q_PROPERTY(QStringList handledMimeTypes MEMBER m_handledMimeTypes)
    /// When using this ha to create a spec file automatically, add additional (non-autodetected) package Requires here.
    Q_PROPERTY(QStringList additionalPackageRequires MEMBER m_additionalPackageRequires)
    /// When using this ha to create a spec file automatically, add additional (non-autodetected) package BuildRequires here.
    Q_PROPERTY(QStringList additionalPackageBuildRequires MEMBER m_additionalPackageBuildRequires)

    Q_CLASSINFO("DefaultProperty", "applicationProfiles")

public:
    explicit Application(QObject *parent = 0);
    virtual ~Application();

    QString applicationId() const;
    void setApplicationId(const QString &id);

    QQmlListProperty<Hemera::Qml::Settings::ApplicationProfile> applicationProfiles();
    QQmlListProperty<Hemera::Qml::Settings::RootOperation> rootOperations();

    QString description() const;
    void setDescription(const QString &description);

    QString version() const;
    void setVersion(const QString &version);

    QString icon() const;
    void setIcon(const QString &icon);

    QString organization() const;
    void setOrganization(const QString &organization);

    QStringList resourceFiles() const;
    void setResourceFiles(const QStringList &resourceFiles);

    QString workingDirectory() const;
    void setWorkingDirectory(const QString &workingDirectory);

protected:
    explicit Application(ApplicationPrivate &dd, QObject *parent);

    ApplicationPrivate * const d_ptr;

private:
    bool m_generateOrbit;
    QStringList m_handledMimeTypes;
    QStringList m_additionalPackageRequires;
    QStringList m_additionalPackageBuildRequires;
};

}

}

}

#endif // HEMERA_QML_HEMERAQMLAPPLICATION_H
