#include "session.h"

#include <QtConcurrent/QtConcurrent>

#include "torrentlist.h"

Session::Session(QObject *parent) :
    QObject(parent),
    sslErrorHandlingEnabled_(true),
    timer_(),
    active_(false),
    session_(),
    stats_(),
    torrentList_(*this)
{
    timer_.setInterval(5000);
    timer_.setSingleShot(false);
    QObject::connect(&timer_, &QTimer::timeout, this, &Session::update);
}

QString Session::host() const
{
    auto &&u = session_.url();
    return QString::fromUtf8(u.c_str(), u.size());
}

void Session::setHost(const QString &hostname)
{
    auto &&h = hostname.toUtf8();
    std::size_t size = static_cast<std::size_t>(h.size());
    if ((session_.url().size() != size) && (strncmp(h.data(), session_.url().c_str(), size) != 0))
    {
        session_.setUrl( { h.data(), size } );
        emit hostChanged();
    }
}

QString Session::path() const
{
    auto &&p = session_.path();
    return QString::fromUtf8(p.c_str(), p.size());
}

void Session::setPath(const QString &path)
{
    auto &&p = path.toUtf8();
    std::size_t size = static_cast<std::size_t>(p.size());
    if ((session_.path().size() != size) && (strncmp(p.data(), session_.path().c_str(), size) != 0))
    {
        session_.setPath( { p.data(), size });
        emit pathChanged();
    }
}

int Session::port() const
{
    return session_.port();
}

void Session::setPort(int port)
{
    if (session_.port() != port)
    {
        session_.setPort(port);
        emit portChanged();
    }
}

bool Session::authenticationEnabled() const
{
    return session_.authenticationRequired();
}

void Session::setAuthenticationEnabled(bool value)
{
    if (value != session_.authenticationRequired())
    {
        session_.setAuthentication(
            value ? librt::Session::Authentication::Required :
                    librt::Session::Authentication::None
        );
        emit authenticationEnabledChanged();
    }
}

QString Session::username() const
{
    auto &&u = session_.username();
    return QString::fromUtf8(u.c_str(), u.size());
}

void Session::setUsername(const QString &username)
{
    auto &&u = username.toUtf8();
    std::size_t size = static_cast<std::size_t>(u.size());
    if ((session_.username().size() != size) && (strncmp(u.data(), session_.username().c_str(), size) != 0))
    {
        session_.setUsername( { u.data(), size } );
        emit usernameChanged();
    }
}

QString Session::password() const
{
    auto &&p = session_.password();
    return QString::fromUtf8(p.c_str(), p.size());
}

void Session::setPassword(const QString &password)
{
    auto &&p = password.toUtf8();
    std::size_t size = static_cast<std::size_t>(p.size());
    if ((session_.password().size() != size) && (strncmp(p.data(), session_.password().c_str(), size) != 0))
    {
        session_.setPassword( { p.data(), size } );
        emit passwordChanged();
    }
}

int Session::timeout() const
{
    return session_.timeout();
}

void Session::setTimeout(int value)
{
    if (value != session_.timeout())
    {
        session_.setTimeout(value);
        emit timeoutChanged();
    }
}

bool Session::sslErrorHandlingEnabled() const
{
    return sslErrorHandlingEnabled_;
}

void Session::setSslErrorHandlingEnabled(bool value)
{
    if (value != sslErrorHandlingEnabled_)
    {
        sslErrorHandlingEnabled_ = value;
        session_.setSSLErrorHandling(
            value ? librt::Session::SSLErrorHandling::Acknowledge :
                    librt::Session::SSLErrorHandling::Ignore
        );
        emit sslErrorHandlingEnabledChanged();
    }
}

int Session::refreshInterval() const
{
    return timer_.interval();
}

void Session::setRefreshInterval(int msecs)
{
    timer_.setInterval(msecs);
}

bool Session::active() const
{
    return active_;
}

void Session::setActive(bool value)
{
    if (value != active_)
    {
        if (value)
        {
            timer_.start();
        }
        else
        {
            timer_.stop();
        }

        active_ = value;
        emit activeChanged();
    }
}

Statistics *Session::stats()
{
    return &stats_;
}

TorrentList *Session::torrents()
{
    return &torrentList_;
}

void Session::update()
{
    QtConcurrent::run(QThreadPool::globalInstance(), [this]() {
        auto r = session_.statistics();
        if (!r.error)
        {
            stats_.setTotalTorrentCount(r.value.totalTorrentCount);
            stats_.setActiveTorrentCount(r.value.activeTorrentCount);
            stats_.setPausedTorrentCount(r.value.pausedTorrentCount);
            stats_.setDownloadSpeed(r.value.downloadSpeed);
            stats_.setUploadSpeed(r.value.uploadSpeed);
        }
    });
    QtConcurrent::run(QThreadPool::globalInstance(), [this]() {
        auto r = session_.torrents();
        if (!r.error)
        {
            torrentList_.update(std::move(r.value));
        }
    });
}
