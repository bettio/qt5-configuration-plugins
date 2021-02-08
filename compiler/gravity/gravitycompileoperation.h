/*
 *
 */

#ifndef COMPILEOPERATION_H
#define COMPILEOPERATION_H

#include <HemeraCore/Operation>

#include <QtCore/QStringList>

#include <qml/settings/hemeraqmlsandbox.h>

#include <HemeraGenerators/BaseCompiler>

class GravityCompilerPlugin;
class QQmlComponent;
class QQmlEngine;
class GravityCompileOperation : public Hemera::Operation
{
    Q_OBJECT
    Q_DISABLE_COPY(GravityCompileOperation)

public:
    enum class Type {
        Sandbox,
        Appliance
    };
    explicit GravityCompileOperation(const Hemera::Generators::CompilerConfiguration &configuration, QObject *object,
                                     QQmlEngine *engine, Type type, GravityCompilerPlugin *parent);
    virtual ~GravityCompileOperation();

protected:
    virtual void startImpl();

private Q_SLOTS:
    void cleanup();
    void compileAppliance();
    void compileSandbox();

private:
    QByteArray payload(const QString &file);

    QStringList featuresToGroups(Hemera::Qml::Settings::OrbitFeatures::Features features);

    void writeBaseApplianceConf(bool hasGui);
    void compileHeadlessAppliance();
    void compileGUIAppliance();
    bool compileStar(QObject *star, const QString &starName, int tty);

    void finalizeUsers();

    GravityCompilerPlugin *m_plugin;
    Hemera::Generators::CompilerConfiguration m_settings;
    QObject *m_object;
    QHash<QString, QStringList> m_userList;
    Type m_type;

    QString m_name;

    QQmlEngine *m_qmlEngine;
};

#endif // COMPILEOPERATION_H
