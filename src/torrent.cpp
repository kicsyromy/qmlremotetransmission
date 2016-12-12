#include "torrent.h"

Torrent::Torrent(librt::Torrent &&torrent, QObject *parent) :
    QObject(parent),
    torrent_(std::move(torrent))
{
}

Torrent::Torrent(Torrent &&other) :
    QObject(other.parent()),
    torrent_(std::move(other.torrent_))
{
}

Torrent &Torrent::operator =(Torrent &&other)
{
    setParent(other.parent());
    torrent_ = std::move(other.torrent_);

    return *this;
}

Torrent &Torrent::operator =(librt::Torrent &&torrent)
{
    torrent_ = std::move(torrent);

    return *this;
}

int Torrent::id() const
{
    return torrent_.id();
}

QString Torrent::name() const
{
    auto &&n = torrent_.name();
    return QString::fromUtf8(n.c_str(), n.size());
}

quint64 Torrent::bytesDownloaded() const
{
    return torrent_.bytesDownloaded();
}

quint64 Torrent::bytesUploaded() const
{
    return torrent_.bytesUploaded();
}

double Torrent::percentDone() const
{
    return torrent_.percentDone();
}

double Torrent::uploadRatio() const
{
    return torrent_.uploadRatio();
}

quint64 Torrent::downloadSpeed() const
{
    return torrent_.downloadSpeed();
}

quint64 Torrent::uploadSpeed() const
{
    return torrent_.uploadSpeed();
}

quint64 Torrent::size() const
{
    return torrent_.size();
}

int Torrent::eta() const
{
    return torrent_.eta();
}

int Torrent::status() const
{
    return static_cast<int>(torrent_.status());
}

int Torrent::queuePosition() const
{
    return torrent_.queuePosition();
}

QString Torrent::downloadDir() const
{
    auto &&dd = torrent_.downloadDir();
    return QString::fromUtf8(dd.c_str(), dd.size());
}
