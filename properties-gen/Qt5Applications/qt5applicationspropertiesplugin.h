/*
 *
 */

#ifndef QT5APPLICATIONSPROPERTIESPLUGIN_H
#define QT5APPLICATIONSPROPERTIESPLUGIN_H

#include <HemeraGenerators/PropertiesPlugin>

class Qt5ApplicationsPropertiesPlugin : public Hemera::Generators::PropertiesPlugin
{
    Q_OBJECT
    Q_DISABLE_COPY(Qt5ApplicationsPropertiesPlugin)
    Q_PLUGIN_METADATA(IID "com.ispirata.Hemera.Qt5SDK.Generators.PropertiesPlugin")
    Q_INTERFACES(Hemera::Generators::PropertiesPlugin)

public:
    Qt5ApplicationsPropertiesPlugin(QObject *parent = nullptr);
    virtual ~Qt5ApplicationsPropertiesPlugin();

    virtual bool propertyGeneration(const char* className) override final;
};

#endif // PROXIEDAPPLICATIONSPROPERTIESPLUGIN_H
