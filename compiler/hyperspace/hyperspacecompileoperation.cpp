#include "hyperspacecompileoperation.h"

#include "hyperspacecompilerplugin.h"

#include <qml/settings/hyperspace/hemeraqmlsettingsmqtttransport.h>

#include <HemeraCore/Literals>

#include <QtCore/QSettings>
#include <QtNetwork/QHostAddress>
#include <QtQml/QQmlEngine>
#include <QtQml/QQmlListReference>

#include <pluginsconfig.h>

// PATHS
#define SYSTEMD_SYSTEM_PATH QStringLiteral("/lib/systemd/system/")
#define SYSTEMD_USER_PATH QStringLiteral("/usr/lib/systemd/user/")

HyperspaceCompileOperation::HyperspaceCompileOperation(const Hemera::Generators::CompilerConfiguration& configuration, QObject* object,
                                                       QQmlEngine* engine, HyperspaceCompilerPlugin* parent)
    : Operation(object)
    , m_configuration(configuration)
    , m_object(object)
    , m_qmlEngine(engine)
    , m_plugin(parent)
{
}

HyperspaceCompileOperation::~HyperspaceCompileOperation()
{
}

void HyperspaceCompileOperation::startImpl()
{
    // Let's do this.
    // Configure transports one by one.
    QQmlListReference transports(m_object, "transports", m_qmlEngine);

    // TODO: Maybe, in the future, we want to allow pure IPC Hyperspace with no transports. For now, this is still not the case.
    if (transports.count() <= 0) {
        setFinishedWithError(QStringLiteral("Attempting to configure Hyperspace with no transports!"), QString());
        return;
    }

    for (int i = 0; i < transports.count(); ++i) {
        QObject *transport = transports.at(i);

        // Check the type
        if (!qstrcmp(transport->metaObject()->className(), "Hemera::Qml::Settings::HTTPTransport")) {
            // HTTP Transport
            if (!compileHTTPTransport(transport)) {
                return;
            }
        } else if (!qstrcmp(transport->metaObject()->className(), "Hemera::Qml::Settings::MQTTTransport")) {
            // MQTT Transport
            if (!compileMQTTTransport(transport)) {
                return;
            }
        } else if (!qstrcmp(transport->metaObject()->className(), "Hemera::Qml::Settings::AstarteTransport")) {
            // MQTT Transport
            if (!compileAstarteTransport(transport)) {
                return;
            }
        } else if (!qstrcmp(transport->metaObject()->className(), "Hemera::Qml::Settings::AstarteGateway")) {
            // MQTT Transport
            if (!compileAstarteGateway(transport)) {
                return;
            }
        } else {
            setFinishedWithError(Hemera::Literals::literal(Hemera::Literals::Errors::interfaceNotAvailable()),
                                 QStringLiteral("Transport %1 is not known to this compiler version!").arg(QLatin1String(transport->metaObject()->className())));
            return;
        }
    }

    // Done!
    setFinished();
}

bool HyperspaceCompileOperation::compileHTTPTransport(QObject* transport)
{
    // TODO: Configure security

    // Configure a systemd socket for each listener. Then our internal magic will do the rest.
    // Get the payload
    QQmlListReference listeners(transport, "networkListeners", m_qmlEngine);

    // This cannot be.
    if (listeners.count() <= 0) {
        setFinishedWithError(QStringLiteral("Attempting to configure an HTTPTransport with no listeners!"), QString());
        return false;
    }

    QDir dataDir = QDir(StaticConfig::hemeraDataDir());

    // Settings
    QDir confDir;
    if (!confDir.exists(StaticConfig::hyperspaceConfigurationDir())) {
        confDir.mkpath(StaticConfig::hyperspaceConfigurationDir());
    }
    QSettings settings(QStringLiteral("%1%2/transport-http.conf").arg(m_configuration.prefix(), StaticConfig::hyperspaceConfigurationDir()), QSettings::IniFormat);
    QList<QUrl> templateUrls;
    QList<uint> urlFeatures;

    for (int i = 0; i < listeners.count(); ++i) {
        QObject *listener = listeners.at(i);

        // Let's get the payload first
        QByteArray sourceData = m_plugin->baseCompiler()->payload(dataDir.absoluteFilePath(QStringLiteral("hyperdrive-transport-http.socket.in")));

        // Do the replacement dance now
        // First of all, we want to check our IP configuration
        QHostAddress address(listener->property("ip").toString());

        if (address.protocol() == QAbstractSocket::UnknownNetworkLayerProtocol || address.isNull()) {
            setFinishedWithError(Hemera::Literals::literal(Hemera::Literals::Errors::badRequest()), QStringLiteral("The ip address provided is not valid!"));
            return false;
        }

        // First, do we bind to a specific IP?
        if (address != QHostAddress::AnyIPv4 && address != QHostAddress::AnyIPv6 && address != QHostAddress::Any) {
            sourceData.replace("@IP@", QStringLiteral("%1:").arg(address.toString()).toLatin1());
        } else {
            sourceData.replace("@IP@", "");
        }

        // Specific interface?
        if (!listener->property("interface").toString().isEmpty() && listener->property("interface").toString() != QStringLiteral("*")) {
            sourceData.replace("@BIND_TO_DEVICE@", QStringLiteral("BindToDevice=%1").arg(listener->property("interface").toString()).toLatin1());
        } else {
            sourceData.replace("@BIND_TO_DEVICE@", "");
        }

        // Ports
        sourceData.replace("@PORT@", QString::number(listener->property("port").toInt()).toLatin1());

        // IPv6 only?
        sourceData.replace("@IPV6_ONLY@", listener->property("ipv6Only").toBool() ? "true" : "false");

        // SSL?
        bool withSsl = listener->property("secure").toBool();
        sourceData.replace("@REQUIRES_SSL@", withSsl ? "Requires=hyperdrive-transport-http-ssl-cert.service" : "");

        // We are done. Let's write it down.
        QString socketFilename = QStringLiteral("%1%2hyperdrive-transport-http-%3.socket").arg(m_configuration.prefix(), SYSTEMD_SYSTEM_PATH).arg(i);
        m_plugin->baseCompiler()->writeFile(socketFilename, sourceData);

        // Symlink
        if (!QFile::link(QStringLiteral("../hyperdrive-transport-http-%1.socket").arg(i),
                         QString::fromLatin1("%1%2sockets.target.wants/hyperdrive-transport-http-%3.socket").arg(m_configuration.prefix(), SYSTEMD_SYSTEM_PATH).arg(i)) &&
            !m_configuration.allowMultilink()) {
            setFinishedWithError(Hemera::Literals::literal(Hemera::Literals::Errors::failedRequest()), QStringLiteral("Could not create symlink!"));
            return false;
        }

        // Populate settings
        settings.beginGroup(QString::number(i)); {
            settings.setValue(QStringLiteral("address"), address.toString());
            settings.setValue(QStringLiteral("port"), listener->property("port").toInt());
            settings.setValue(QStringLiteral("secure"), listener->property("secure").toBool());
        } settings.endGroup();

        // Create template template URL
        QUrl templateUrl;
        templateUrl.setScheme(listener->property("secure").toBool() ? QStringLiteral("https") : QStringLiteral("http"));
        templateUrl.setHost(QStringLiteral("host"));
        templateUrl.setPort(listener->property("port").toInt());
        templateUrls.append(templateUrl);
        Features features = Feature::Reliable;
        if (listener->property("secure").toBool()) {
            features |= Feature::Secure;
        } else {
            features |= Feature::Insecure;
        }
        urlFeatures.append(features);
    }

    // Save global settings for transport discovery and authorization.
    QSettings hyperdriveSettings(QStringLiteral("%1%2/transports.d/HTTP.conf").arg(m_configuration.prefix(),
                                                                                   StaticConfig::hyperspaceConfigurationDir()),
                                 QSettings::IniFormat);
    hyperdriveSettings.beginGroup(QStringLiteral("HTTP")); {
        hyperdriveSettings.beginWriteArray(QStringLiteral("templateUrls")); {
            for (int i = 0; i < templateUrls.size(); ++i) {
                hyperdriveSettings.setArrayIndex(i);
                hyperdriveSettings.setValue(QStringLiteral("url"), templateUrls.at(i));
                hyperdriveSettings.setValue(QStringLiteral("features"), urlFeatures.at(i));
            }
        } hyperdriveSettings.endArray();
    } hyperdriveSettings.endGroup();


    // Yeah.
    return true;
}

bool HyperspaceCompileOperation::compileMQTTTransport(QObject* transport)
{
    // Basic data
    QString host = transport->property("host").toString();
    int port = transport->property("port").toInt();
    Hemera::Qml::Settings::MQTTProtocol::Protocol protocol = static_cast< Hemera::Qml::Settings::MQTTProtocol::Protocol >(transport->property("protocol").toInt());
    QString protocolString;

    switch (protocol) {
        case Hemera::Qml::Settings::MQTTProtocol::TCP:
            protocolString = QStringLiteral("tcp");
            break;
        case Hemera::Qml::Settings::MQTTProtocol::SSL:
            protocolString = QStringLiteral("ssl");
            break;
        case Hemera::Qml::Settings::MQTTProtocol::UDP:
            protocolString = QStringLiteral("udp");
            break;
        default:
            protocolString = QStringLiteral("tcp");
            break;
    }

    if (port == 0) {
        // Set the default port
        if (protocol == Hemera::Qml::Settings::MQTTProtocol::SSL) {
            port = 8883;
        } else {
            port = 1883;
        }
    }

    QUrl hostUrl;
    hostUrl.setScheme(protocolString);
    hostUrl.setHost(host);
    hostUrl.setPort(port);

    // Settings
    QDir confDir;
    if (!confDir.exists(StaticConfig::hyperspaceConfigurationDir())) {
        confDir.mkpath(StaticConfig::hyperspaceConfigurationDir());
    }
    QSettings settings(QStringLiteral("%1%2/transport-mqtt-%3.conf").arg(m_configuration.prefix(),
                                                                         StaticConfig::hyperspaceConfigurationDir(),
                                                                         hostUrl.toString(QUrl::RemoveScheme).remove(QLatin1Char('/')).remove(QLatin1Char(':'))),
                       QSettings::IniFormat);

    // Populate settings
    settings.beginGroup(QStringLiteral("MQTTTransport")); {
        settings.setValue(QStringLiteral("hostUrl"), hostUrl);
        settings.setValue(QStringLiteral("publishQoS"), transport->property("publishQoS").toInt());
        settings.setValue(QStringLiteral("subscribeQoS"), transport->property("subscribeQoS").toInt());
        settings.setValue(QStringLiteral("cleanSession"), transport->property("cleanSession").toBool());
        settings.setValue(QStringLiteral("keepAlive"), transport->property("keepAlive").toInt());

        settings.setValue(QStringLiteral("trustStore"), transport->property("trustStore").toString());
        settings.setValue(QStringLiteral("keyStore"), transport->property("keyStore").toString());
        settings.setValue(QStringLiteral("privateKey"), transport->property("privateKey").toString());
        settings.setValue(QStringLiteral("privateKeyPassword"), transport->property("privateKeyPassword").toString());
        settings.setValue(QStringLiteral("serverCertificateVerification"), transport->property("serverCertificateVerification").toBool());
    } settings.endGroup();

    // Yeah.
    return true;
}

bool HyperspaceCompileOperation::compileAstarteTransport(QObject* transport)
{
    // Settings
    QDir confDir;
    if (!confDir.exists(StaticConfig::hyperspaceConfigurationDir())) {
        confDir.mkpath(StaticConfig::hyperspaceConfigurationDir());
    }

    // There can be only one Astarte endpoint on the system.
    QSettings settings(QStringLiteral("%1%2/transport-astarte.conf").arg(m_configuration.prefix(),
                                                                         StaticConfig::hyperspaceConfigurationDir()),
                       QSettings::IniFormat);

    // Populate settings
    settings.beginGroup(QStringLiteral("AstarteTransport")); {
        // Check endpoint/gateway
        if (!transport->property("endpoint").toUrl().isValid() && !transport->property("gateway").toUrl().isValid()) {
            setFinishedWithError(QStringLiteral("Astarte can be configured either with an endpoint or a gateway, not both!"), QString());
            return false;
        }

        if (transport->property("endpoint").toUrl().isValid()) {
            settings.setValue(QStringLiteral("endpoint"), transport->property("endpoint").toUrl().toString());
        }
        if (transport->property("gateway").toUrl().isValid()) {
            settings.setValue(QStringLiteral("gateway"), transport->property("gateway").toUrl().toString());
        }
        if (!transport->property("agentKey").toString().isEmpty()) {
            settings.setValue(QStringLiteral("agentKey"), transport->property("agentKey").toString());
        }
        if (!transport->property("signingKey").toString().isEmpty()) {
            settings.setValue(QStringLiteral("signingKey"), transport->property("signingKey").toString());
        }
        if (transport->property("rebootWhenConnectionFails").toBool()) {
            settings.setValue(QStringLiteral("rebootWhenConnectionFails"), transport->property("rebootWhenConnectionFails").toBool());
            if (transport->property("rebootDelayMinutes").toInt() > 0) {
                settings.setValue(QStringLiteral("rebootDelayMinutes"), transport->property("rebootDelayMinutes").toInt());
            }
        }
    } settings.endGroup();

    // Yeah.
    return true;
}

bool HyperspaceCompileOperation::compileAstarteGateway(QObject* transport)
{
    // Settings
    QDir confDir;
    if (!confDir.exists(StaticConfig::hyperspaceConfigurationDir())) {
        confDir.mkpath(StaticConfig::hyperspaceConfigurationDir());
    }

    // There can be only one Astarte endpoint on the system.
    QSettings settings(QStringLiteral("%1%2/transport-astarte.conf").arg(m_configuration.prefix(),
                                                                         StaticConfig::hyperspaceConfigurationDir()),
                       QSettings::IniFormat);

    // Easy things first: populate local transport settings
    settings.beginGroup(QStringLiteral("AstarteTransport")); {
        settings.setValue(QStringLiteral("gateway"), QStringLiteral("127.0.0.1"));
    } settings.endGroup();

    // Let's populate the gateway settings now. They will be needed by our pairing helper.
    settings.beginGroup(QStringLiteral("AstarteGateway")); {
        settings.setValue(QStringLiteral("endpoint"), transport->property("endpoint"));
        settings.setValue(QStringLiteral("astarteBrokerHost"), transport->property("astarteBrokerHost"));
        settings.setValue(QStringLiteral("astarteBrokerPort"), transport->property("astarteBrokerPort"));
    } settings.endGroup();

    // Let's configure Mosquitto now.
    // Let's get the payload first
    QDir dataDir = QDir(StaticConfig::hemeraDataDir());
    QByteArray sourceData = m_plugin->baseCompiler()->payload(dataDir.absoluteFilePath(QStringLiteral("mosquitto-astarte-gateway.conf.in")));

    // Do the replacement dance now

    // Specific bind interface?
    if (!transport->property("bindHost").toString().isEmpty()) {
        sourceData.replace("@BIND_ADDRESS@", QStringLiteral("bind_address %1").arg(transport->property("bindHost").toString()).toLatin1());
    } else {
        sourceData.replace("@BIND_ADDRESS@", "");
    }

    // Broker settings
    sourceData.replace("@ASTARTE_BROKER_HOST@", transport->property("astarteBrokerHost").toString().toLatin1());
    sourceData.replace("@ASTARTE_BROKER_PORT@", QString::number(transport->property("astarteBrokerPort").toInt()).toLatin1());
    sourceData.replace("@TOPIC_ACCESS_RULES@", transport->property("topicMatchingRules").toStringList().join(QLatin1Char('\n')).toLatin1());

    // We are done. Let's write it down.
    QString confFilename = QStringLiteral("%1%2/mosquitto.conf.default").arg(m_configuration.prefix(),
                                                                             StaticConfig::hyperspaceConfigurationDir());
    m_plugin->baseCompiler()->writeFile(confFilename, sourceData);

    // Yeah.
    return true;
}
