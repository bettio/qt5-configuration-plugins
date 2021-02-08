#ifndef HYPERSPACECOMPILEOPERATION_H
#define HYPERSPACECOMPILEOPERATION_H

#include <HemeraCore/Operation>

#include <HemeraGenerators/BaseCompiler>

class HyperspaceCompilerPlugin;
class QQmlEngine;
class HyperspaceCompileOperation : public Hemera::Operation
{
    Q_OBJECT
    Q_DISABLE_COPY(HyperspaceCompileOperation)

public:
    // FIXME: This one is imported from Hyperdrive. How do we handle this?
    enum class Feature {
        None = 0,
        Reliable = 1 << 0,
        Unreliable = 1 << 1,
        Secure = 1 << 2,
        Insecure = 1 << 3,
    };
    Q_ENUMS(Feature)
    Q_DECLARE_FLAGS(Features, Feature)

    explicit HyperspaceCompileOperation(const Hemera::Generators::CompilerConfiguration &configuration, QObject *object,
                                        QQmlEngine *engine, HyperspaceCompilerPlugin *parent);
    virtual ~HyperspaceCompileOperation();

protected:
    virtual void startImpl() override final;

private Q_SLOTS:
    bool compileAstarteGateway(QObject *transport);
    bool compileAstarteTransport(QObject *transport);
    bool compileHTTPTransport(QObject *transport);
    bool compileMQTTTransport(QObject *transport);

private:
    Hemera::Generators::CompilerConfiguration m_configuration;
    QObject *m_object;
    QQmlEngine *m_qmlEngine;
    HyperspaceCompilerPlugin *m_plugin;
};

#endif // HYPERSPACECOMPILEOPERATION_H
