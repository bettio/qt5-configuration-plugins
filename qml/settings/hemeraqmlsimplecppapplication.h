/*
 *
 */

#ifndef HEMERA_QML_SETTINGS_SIMPLECPPAPPLICATION_H
#define HEMERA_QML_SETTINGS_SIMPLECPPAPPLICATION_H

#include "hemeraqmlapplication.h"

#include "hemeraqmlhyperspaceqt5consumer.h"

namespace Hemera {
namespace Qml {
namespace Settings {

/**
 * @class HyperspaceModules
 * @ingroup HemeraQmlSettings
 *
 * @brief Container for HyperspaceModules enum.
 */
class HyperspaceModules : public QObject
{
    Q_OBJECT
    Q_DISABLE_COPY(HyperspaceModules)

public:
    enum Module : quint32 {
        NoModules = 0,
        Core = 1 << 0,
        ProducerConsumer = 1 << 1
    };
    Q_DECLARE_FLAGS(Modules, Module)
    Q_FLAGS(Modules)
    Q_ENUM(Module)
};

/**
 * @class HemeraModules
 * @ingroup HemeraQmlSettings
 *
 * @brief Container for HemeraModules enum.
 */
class HemeraModules : public QObject
{
    Q_OBJECT
    Q_DISABLE_COPY(HemeraModules)

public:
    enum Module {
        NoModules = 0,
        Core = 1 << 0,
        Gui = 1 << 1,
        Multimedia = 1 << 2,
        SoftwareManagement = 1 << 3,
        Test = 1 << 4,
        Qml = 1 << 5,
        Widgets = 1 << 6,
        WebClient = 1 << 7
    };
    Q_DECLARE_FLAGS(Modules, Module)
    Q_FLAGS(Modules)
    Q_ENUM(Module)
};

/**
 * @class QtModules
 * @ingroup HemeraQmlSettings
 *
 * @brief Container for QtModules enum.
 */
class QtModules : public QObject
{
    Q_OBJECT
    Q_DISABLE_COPY(QtModules)

public:
    enum Module : quint32 {
        NoModules = 0,
        Core = 1 << 0,
        Gui = 1 << 1,
        Multimedia = 1 << 2,
        Test = 1 << 3,
        Widgets = 1 << 4,
        Declarative = 1 << 5,
        SerialPort = 1 << 6,
        Quick = 1 << 7,
        Qml = 1 << 8,
        DBus = 1 << 9,
        Network = 1 << 10,
        WebKit = 1 << 11,
        SystemInfo = 1 << 12,
        Compositor = 1  << 13,
        Sql = 1 << 14
    };
    Q_DECLARE_FLAGS(Modules, Module)
    Q_FLAGS(Modules)
    Q_ENUM(Module)
};

class SimpleCppApplicationPrivate;
/**
 * @class SimpleCppApplication
 * @ingroup HemeraQmlSettings
 *
 * @brief SimpleCppApplication is the base object for application manifest files (ha) which should act as a buildsystem for C++ applications.
 *
 * SimpleCppApplication extends Application with additional parameters used for building a C++ application from sources.
 * When invoking configure, a CMake buildsystem is automatically generated from the ha and can be used to build the application
 * and create its package or package descriptor straight away.
 *
 * It is possible to specify the list of Qt5 modules and Hemera Qt5 SDK modules to be used in the application, which automatically
 * adjusts include paths and linked libraries. In the same fashion, it is possible to specify a list of pkgconfig modules, which will
 * adjust linked libraries and include paths in the same fashion. In case none of those options can be used, it is possible to manually
 * specify which additional libraries to link to and which additional include paths should be used.
 *
 * @note Several concepts in SimpleCppApplication are better explained in @ref BaseHemeraConcepts "Base Hemera Concepts"
 *
 * @sa Hemera::Qml::Settings::Application
 */
class SimpleCppApplication : public Hemera::Qml::Settings::Application
{
    Q_OBJECT
    Q_DISABLE_COPY(SimpleCppApplication)
    Q_DECLARE_PRIVATE(SimpleCppApplication)

    /// A list of paths, relative to the ha file, of source files to be compiled
    Q_PROPERTY(QStringList sourceFiles READ sourceFiles WRITE setSourceFiles)
    /// A list of library names to link to. Usually not advised, use pkgconfig or modules if you can.
    Q_PROPERTY(QStringList libraries READ libraries WRITE setLibraries)
    /// A list of additional include paths.
    Q_PROPERTY(QStringList includePaths READ includePaths WRITE setIncludePaths)
    /// A list of additional pkgconfig modules to include
    Q_PROPERTY(QStringList pkgconfig MEMBER m_pkgconfig)
    /// List of HemeraQt5SDK modules to use
    Q_PROPERTY(Hemera::Qml::Settings::HemeraModules::Modules hemeraModules READ hemeraModules WRITE setHemeraModules)
    /// List of HyperspaceQt5 modules to use
    Q_PROPERTY(Hemera::Qml::Settings::HyperspaceModules::Modules hyperspaceModules READ hyperspaceModules WRITE setHyperspaceModules)
    /// List of Qt5 modules to use
    Q_PROPERTY(Hemera::Qml::Settings::QtModules::Modules qtModules READ qtModules WRITE setQtModules)
    /// A list of additional pkgconfig modules to include
    Q_PROPERTY(QStringList hyperspaceProducers MEMBER m_hyperspaceProducers)
    /// A list of additional pkgconfig modules to include
    Q_PROPERTY(QQmlListProperty<Hemera::Qml::Settings::HyperspaceQt5Consumer> hyperspaceConsumers READ hyperspaceConsumers)

public:
    explicit SimpleCppApplication(QObject *parent = 0);
    virtual ~SimpleCppApplication();

    QStringList sourceFiles() const;
    void setSourceFiles(const QStringList &sourceFiles);

    QStringList libraries() const;
    void setLibraries(const QStringList &libraries);

    QStringList includePaths() const;
    void setIncludePaths(const QStringList &includePaths);

    Hemera::Qml::Settings::HemeraModules::Modules hemeraModules() const;
    void setHemeraModules(Hemera::Qml::Settings::HemeraModules::Modules modules);

    Hemera::Qml::Settings::HyperspaceModules::Modules hyperspaceModules() const;
    void setHyperspaceModules(Hemera::Qml::Settings::HyperspaceModules::Modules modules);

    Hemera::Qml::Settings::QtModules::Modules qtModules() const;
    void setQtModules(Hemera::Qml::Settings::QtModules::Modules modules);

    QQmlListProperty<Hemera::Qml::Settings::HyperspaceQt5Consumer> hyperspaceConsumers();

protected:
    explicit SimpleCppApplication(SimpleCppApplicationPrivate &dd, QObject *parent);

private:
    QStringList m_pkgconfig;
    QStringList m_hyperspaceProducers;
    QList<Hemera::Qml::Settings::HyperspaceQt5Consumer*> m_hyperspaceConsumers;
};

}
}
}

Q_DECLARE_OPERATORS_FOR_FLAGS(Hemera::Qml::Settings::QtModules::Modules)
Q_DECLARE_OPERATORS_FOR_FLAGS(Hemera::Qml::Settings::HemeraModules::Modules)
Q_DECLARE_OPERATORS_FOR_FLAGS(Hemera::Qml::Settings::HyperspaceModules::Modules)

#endif // HEMERA_QML_SETTINGS_SIMPLECPPAPPLICATION_H
