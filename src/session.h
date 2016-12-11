#ifndef SESSION_H
#define SESSION_H

#include <QObject>
#include <QString>

#include <librt_session.h>

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

public:
    Session();

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

signals:
    void hostChanged();
    void pathChanged();
    void portChanged();
    void authenticationEnabledChanged();
    void usernameChanged();
    void passwordChanged();
    void timeoutChanged();
    void sslErrorHandlingEnabledChanged();

private:
    DISABLE_COPY(Session)
    DISABLE_MOVE(Session)

private:
    bool sslErrorHandlingEnabled_;
    librt::Session session_;

};

#endif // SESSION_H
