#ifndef PLUGIN_H
#define PLUGIN_H

#include <QQmlExtensionPlugin>

class QMLRemoteTransmission: public QQmlExtensionPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "org.kicsyromy.RemoteTransmission")

public:
    QMLRemoteTransmission();

    void registerTypes(const char *uri);
    void initializeEngine(QQmlEngine *engine, const char *uri);
};

#endif // PLUGIN_H
