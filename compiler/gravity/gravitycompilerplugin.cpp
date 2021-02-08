#include "gravitycompilerplugin.h"

#include "gravitycompileoperation.h"

GravityCompilerPlugin::GravityCompilerPlugin(QObject* parent)
    : CompilerPlugin(parent)
{
}

GravityCompilerPlugin::~GravityCompilerPlugin()
{
}

Hemera::Operation* GravityCompilerPlugin::operationForClassName(const char* className, QObject* configurationObject,
                                                                const Hemera::Generators::CompilerConfiguration& compilerConfiguration)
{
    // Crawl back onto the superclasses.
    const QMetaObject *mO = configurationObject->metaObject();
    do {
        if (!qstrcmp(mO->className(), "Hemera::Qml::Settings::Appliance")) {
            return new GravityCompileOperation(compilerConfiguration, configurationObject, baseCompiler()->qmlEngine(), GravityCompileOperation::Type::Appliance, this);
        } else if (!qstrcmp(mO->className(), "Hemera::Qml::Settings::Sandbox")) {
            return new GravityCompileOperation(compilerConfiguration, configurationObject, baseCompiler()->qmlEngine(), GravityCompileOperation::Type::Sandbox, this);
        }

        mO = mO->superClass();
    } while (mO->superClass() != nullptr);

    // We can't handle this
    return nullptr;
}

