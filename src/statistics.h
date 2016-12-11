#ifndef STATISTICS_H
#define STATISTICS_H

#include <atomic>

#include <QObject>

#include <librt_global.h>

class Statistics: public QObject
{
    Q_OBJECT
    Q_PROPERTY(int totalTorrentCount  READ totalTorrentCount  NOTIFY totalTorrentCountChanged)
    Q_PROPERTY(int activeTorrentCount READ activeTorrentCount NOTIFY activeTorrentCountChanged)
    Q_PROPERTY(int pausedTorrentCount READ pausedTorrentCount NOTIFY pausedTorrentCountChanged)
    Q_PROPERTY(int downloadSpeed      READ downloadSpeed      NOTIFY downloadSpeedChanged)
    Q_PROPERTY(int uploadSpeed        READ uploadSpeed        NOTIFY uploadSpeedChanged)

public:
    explicit Statistics(QObject *parent = Q_NULLPTR);

public:
    int totalTorrentCount() const;
    void setTotalTorrentCount(int value);

    int activeTorrentCount() const;
    void setActiveTorrentCount(int value);

    int pausedTorrentCount() const;
    void setPausedTorrentCount(int value);

    int downloadSpeed() const;
    void setDownloadSpeed(int value);

    int uploadSpeed() const;
    void setUploadSpeed(int value);

signals:
    void totalTorrentCountChanged();
    void activeTorrentCountChanged();
    void pausedTorrentCountChanged();
    void downloadSpeedChanged();
    void uploadSpeedChanged();

private:
    std::atomic_int totalTorrentCount_;
    std::atomic_int activeTorrentCount_;
    std::atomic_int pausedTorrentCount_;
    std::atomic_int downloadSpeed_;
    std::atomic_int uploadSpeed_;

private:
    DISABLE_COPY(Statistics)
    DISABLE_MOVE(Statistics)
};

#endif // STATISTICS_H
