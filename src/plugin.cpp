#include "plugin.h"

#include <QQmlEngine>

#include "session.h"

namespace
{
    constexpr const auto URI_STR     { URI };
    constexpr const auto URI_V_MAJOR { VERSION_MAJOR };
    constexpr const auto URI_V_MINOR { VERSION_MINOR };
}

QMLRemoteTransmission::QMLRemoteTransmission()
{
}

void QMLRemoteTransmission::registerTypes(const char *uri)
{
    Q_ASSERT(uri == QLatin1String(URI_STR));
    qmlRegisterType<Session>(uri, URI_V_MAJOR, URI_V_MINOR, "Session");
}

void QMLRemoteTransmission::initializeEngine(QQmlEngine *engine, const char *uri)
{
    Q_ASSERT(uri == QLatin1String(URI_STR));
    Q_UNUSED(engine)
}
