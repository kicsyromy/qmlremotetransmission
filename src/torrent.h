#ifndef TORRENT_H
#define TORRENT_H

#include <QObject>

#include <librt_torrent.h>

class Torrent : public QObject
{
    Q_OBJECT
    Q_ENUMS(Status)

public:
    enum Status
    {
        Stopped        = static_cast<int>(librt::Torrent::Status::Stopped),
        CheckWait      = static_cast<int>(librt::Torrent::Status::CheckWait),
        Check          = static_cast<int>(librt::Torrent::Status::Check),
        DownloadWait   = static_cast<int>(librt::Torrent::Status::DownloadWait),
        Download       = static_cast<int>(librt::Torrent::Status::Download),
        SeedWait       = static_cast<int>(librt::Torrent::Status::SeedWait),
        Seed           = static_cast<int>(librt::Torrent::Status::Seed)
    };

    Q_PROPERTY(int     id              READ id              NOTIFY idChanged)
    Q_PROPERTY(QString name            READ name            NOTIFY nameChanged)
    Q_PROPERTY(quint64 bytesDownloaded READ bytesDownloaded NOTIFY bytesDownloadedChanged)
    Q_PROPERTY(quint64 bytesUploaded   READ bytesUploaded   NOTIFY bytesUploadedChanged)
    Q_PROPERTY(double  percentDone     READ percentDone     NOTIFY percentDoneChanged)
    Q_PROPERTY(double  uploadRatio     READ uploadRatio     NOTIFY uploadRatioChanged)
    Q_PROPERTY(quint64 downloadSpeed   READ downloadSpeed   NOTIFY downloadSpeedChanged)
    Q_PROPERTY(quint64 uploadSpeed     READ uploadSpeed     NOTIFY uploadSpeedChanged)
    Q_PROPERTY(quint64 size            READ size            NOTIFY sizeChanged)
    Q_PROPERTY(int     eta             READ eta             NOTIFY etaChanged)
    Q_PROPERTY(int     status          READ status          NOTIFY statusChanged)
    Q_PROPERTY(int     queuePosition   READ queuePosition   NOTIFY queuePositionChanged)
    Q_PROPERTY(QString downloadDir     READ downloadDir     NOTIFY downloadDirChanged)

public:
    Torrent(librt::Torrent &&torrent, QObject *parent = Q_NULLPTR);
    Torrent(Torrent &&other);
    Torrent &operator =(Torrent &&other);
    Torrent &operator =(librt::Torrent &&torrent);

public:
    int id() const;
    QString name() const;
    quint64 bytesDownloaded() const;
    quint64 bytesUploaded() const;
    double percentDone() const;
    double uploadRatio() const;
    quint64 downloadSpeed() const;
    quint64 uploadSpeed() const;
    quint64 size() const;
    int eta() const;
    int status() const;
    int queuePosition() const;
    QString downloadDir() const;

signals:
    void idChanged();
    void nameChanged();
    void bytesDownloadedChanged();
    void bytesUploadedChanged();
    void percentDoneChanged();
    void uploadRatioChanged();
    void downloadSpeedChanged();
    void uploadSpeedChanged();
    void sizeChanged();
    void etaChanged();
    void statusChanged();
    void queuePositionChanged();
    void downloadDirChanged();

private:
//    quint64 bytesDownloaded_;
//    quint64 bytesUploaded_;
//    double percentDone_;
//    double uploadRatio_;
//    quint64 downloadSpeed_;
//    quint64 uploadSpeed_;
//    quint64 size_;
//    int eta_;
//    int status_;
//    int queuePosition_;
//    QString downloadDir_;
    librt::Torrent torrent_;

private:
    friend class TorrentList;

private:
    DISABLE_COPY(Torrent)
//    DISABLE_MOVE(Torrent)
};

#endif // TORRENT_H
