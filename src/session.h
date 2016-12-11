#ifndef SESSION_H
#define SESSION_H

#include <QObject>
#include <QTimer>
#include <QString>
#include <QPointer>

#include <librt_session.h>

#include "statistics.h"

class TorrentList;

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

    Q_PROPERTY(Statistics* stats READ stats NOTIFY statsChanged)

public:
    explicit Session(QObject *parent = Q_NULLPTR);

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

public:
    void addTorrentList(TorrentList *torrentList);
    void removeTorrentList(TorrentList *torrentList);

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

private slots:
    void update();

private:
    DISABLE_COPY(Session)
    DISABLE_MOVE(Session)

private:
    bool sslErrorHandlingEnabled_;
    QTimer timer_;
    bool active_;
    librt::Session session_;
    Statistics stats_;
    std::vector<QPointer<TorrentList>> torrentLists_;
};

#endif // SESSION_H
