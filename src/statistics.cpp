#include "statistics.h"

Statistics::Statistics(QObject *parent) :
    QObject(parent),
    totalTorrentCount_(0),
    activeTorrentCount_(0),
    pausedTorrentCount_(0),
    downloadSpeed_(0),
    uploadSpeed_(0)
{
}

int Statistics::totalTorrentCount() const
{
    return totalTorrentCount_.load();
}

void Statistics::setTotalTorrentCount(int value)
{
    if (value != totalTorrentCount_.load())
    {
        totalTorrentCount_.store(value);
        emit totalTorrentCountChanged();
    }
}

int Statistics::activeTorrentCount() const
{
    return activeTorrentCount_.load();
}

void Statistics::setActiveTorrentCount(int value)
{
    if (value != activeTorrentCount_.load())
    {
        activeTorrentCount_.store(value);
        emit activeTorrentCountChanged();
    }
}

int Statistics::pausedTorrentCount() const
{
    return pausedTorrentCount_.load();
}

void Statistics::setPausedTorrentCount(int value)
{
    if (value != pausedTorrentCount_.load())
    {
        pausedTorrentCount_.store(value);
        emit pausedTorrentCountChanged();
    }
}

int Statistics::downloadSpeed() const
{
    return downloadSpeed_.load();
}

void Statistics::setDownloadSpeed(int value)
{
    if (value != downloadSpeed_.load())
    {
        downloadSpeed_.store(value);
        emit downloadSpeedChanged();
    }
}

int Statistics::uploadSpeed() const
{
    return uploadSpeed_.load();
}

void Statistics::setUploadSpeed(int value)
{
    if (value != uploadSpeed_.load())
    {
        uploadSpeed_.store(value);
        emit uploadSpeedChanged();
    }
}
