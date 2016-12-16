#ifndef SESSION_H
#define SESSION_H


#include <QObject>
#include <QTimer>
#include <QString>
#include <QPointer>
#include <QMutex>
#include <QThreadPool>

#include <librt_session.h>

#include "statistics.h"
#include "torrentlist.h"

struct StatisticsUpdate: QRunnable
{
    StatisticsUpdate(librt::Session &session, Statistics &stats) :
        session_(session), stats_(stats)
    {setAutoDelete(false);}
    void run() Q_DECL_OVERRIDE
    {
        auto r = session_.statistics();
        if (!r.error)
        {
            stats_.setTotalTorrentCount(r.value.totalTorrentCount);
            stats_.setActiveTorrentCount(r.value.activeTorrentCount);
            stats_.setPausedTorrentCount(r.value.pausedTorrentCount);
            stats_.setDownloadSpeed(r.value.downloadSpeed);
            stats_.setUploadSpeed(r.value.uploadSpeed);
        }
    }

private:
    librt::Session &session_;
    Statistics &stats_;
};

struct TorrentListUpdate: QRunnable
{
    TorrentListUpdate(librt::Session &session, TorrentList &torrentList, QMutex &lock) :
        session_(session), torrentList_(torrentList), lock_(lock)
    {setAutoDelete(false);}
    void run() Q_DECL_OVERRIDE
    {
        auto r = session_.torrents();
        if (!r.error)
        {
            if (lock_.tryLock())
            {
                torrentList_.update(std::move(r.value));
            }
            else
            {
                return;
            }
        }
    }

private:
    librt::Session &session_;
    TorrentList &torrentList_;
    QMutex &lock_;
};

class Session: public QObject
{
    Q_OBJECT

    Q_PROPERTY(QString host                    READ host                    WRITE setHost                    NOTIFY hostChanged)
    Q_PROPERTY(QString path                    READ path                    WRITE setPath                    NOTIFY pathChanged)
    Q_PROPERTY(int     port                    READ port                    WRITE setPort                    NOTIFY portChanged)
    Q_PROPERTY(bool    authenticationEnabled   READ authenticationEnabled   WRITE setAuthenticationEnabled   NOTIFY authenticationEnabledChanged)
    Q_PROPERTY(QString username                READ username                WRITE setUsername                NOTIFY usernameChanged)
    Q_PROPERTY(QString password                READ password                WRITE setPassword                NOTIFY passwordChanged)
    Q_PROPERTY(int     timeout                 READ timeout                 WRITE setTimeout                 NOTIFY timeoutChanged)
    Q_PROPERTY(bool    sslErrorHandlingEnabled READ sslErrorHandlingEnabled WRITE setSslErrorHandlingEnabled NOTIFY sslErrorHandlingEnabledChanged)
    Q_PROPERTY(int     refreshInterval         READ refreshInterval         WRITE setRefreshInterval         NOTIFY refreshIntervalChanged)
    Q_PROPERTY(bool    active                  READ active                  WRITE setActive                  NOTIFY activeChanged)

    Q_PROPERTY(Statistics*  stats    READ stats    NOTIFY statsChanged)
    Q_PROPERTY(TorrentList* torrents READ torrents NOTIFY torrentsChanged)

public:
    explicit Session(QObject *parent = Q_NULLPTR);
    ~Session();

public:
    QString host() const;
    void setHost(const QString &hostname);

    QString path() const;
    void setPath(const QString &path);

    int port() const;
    void setPort(int port);

    bool authenticationEnabled() const;
    void setAuthenticationEnabled(bool value);

    QString username() const;
    void setUsername(const QString &username);

    QString password() const;
    void setPassword(const QString &password);

    int timeout() const;
    void setTimeout(int value);

    bool sslErrorHandlingEnabled() const;
    void setSslErrorHandlingEnabled(bool value);

    int refreshInterval() const;
    void setRefreshInterval(int msecs);

    bool active() const;
    void setActive(bool value);

public:
    Statistics *stats();
    TorrentList *torrents();

signals:
    void hostChanged();
    void pathChanged();
    void portChanged();
    void authenticationEnabledChanged();
    void usernameChanged();
    void passwordChanged();
    void timeoutChanged();
    void sslErrorHandlingEnabledChanged();
    void refreshIntervalChanged();
    void activeChanged();

signals:
    void statsChanged();
    void torrentsChanged();

private slots:
    void update();

private:
    DISABLE_COPY(Session)
    DISABLE_MOVE(Session)

private:
    friend class TorrentList;

private:
    bool sslErrorHandlingEnabled_;
    QTimer timer_;
    bool active_;
    librt::Session session_;
    Statistics stats_;
    StatisticsUpdate statsUpdate_;
    TorrentList torrentList_;
    QMutex torrentUpdateLock_;
    TorrentListUpdate torrentListUpdate_;
    QThreadPool workers_;
};

#endif // SESSION_H
