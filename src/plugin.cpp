#include "plugin.h"

#include <QQmlEngine>

#include "session.h"
#include "statistics.h"
#include "torrent.h"
#include "torrentlist.h"

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
    qmlRegisterType<TorrentList>(uri, URI_V_MAJOR, URI_V_MINOR, "TorrentList");
    qmlRegisterUncreatableType<Statistics>(uri, URI_V_MAJOR, URI_V_MINOR, "Statistics", "Statistics are obtained from a Session");
    qmlRegisterUncreatableType<Torrent>(uri, URI_V_MAJOR, URI_V_MINOR, "Torrent", "Torrents are obtained from a TorrentList");
}

void QMLRemoteTransmission::initializeEngine(QQmlEngine *engine, const char *uri)
{
    Q_ASSERT(uri == QLatin1String(URI_STR));
    Q_UNUSED(engine)
}
