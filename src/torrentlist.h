#ifndef TORRENT_LIST_H
#define TORRENT_LIST_H

#include <mutex>

#include <QAbstractListModel>
#include <QPointer>

#include <librt_torrent.h>

#include "torrent.h"

class Session;

class TorrentList: public QAbstractListModel
{
    Q_OBJECT
    Q_ENUMS(Sorting)
    Q_PROPERTY(int sorting READ sorting WRITE setSorting NOTIFY sortingChanged)

public:
    enum Sorting
    {
        SortByName,
        SortByQueuePosition
    };

public:
    TorrentList(Session &session, QObject *parent = Q_NULLPTR);

public:
    int sorting();
    void setSorting(int value);

signals:
    void sortingChanged();

public:
    int rowCount(const QModelIndex &parent) const Q_DECL_OVERRIDE;
    QVariant data(const QModelIndex &index, int role) const Q_DECL_OVERRIDE;
    QHash<int, QByteArray> roleNames() const Q_DECL_OVERRIDE;

public:
    void update(std::vector<librt::Torrent> &&torrents);

private slots:
    void onCachedTorrentsUpdated();

private:
    QPointer<Session> session_;
    std::vector<librt::Torrent> cached_;
    std::vector<Torrent> torrents_;
    std::mutex mutex_;
    Sorting sorting_;
};

#endif // TORRENT_LIST_H
