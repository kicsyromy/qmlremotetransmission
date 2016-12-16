#include "torrentlist.h"

#include "session.h"

#include <QDebug>

namespace
{
    constexpr const auto TORRENT_ROLE { Qt::DisplayRole + 1 };
}

TorrentList::TorrentList(Session &session, QObject *parent) :
    QAbstractListModel(parent),
    session_(&session),
    cached_(),
    torrents_(),
    sorting_(SortByName)
{
}

int TorrentList::sorting()
{
    return sorting_;
}

void TorrentList::setSorting(int value)
{
    if (sorting_ != value)
    {
        sorting_ = static_cast<Sorting>(value);
        emit sortingChanged();
    }
}

int TorrentList::rowCount(const QModelIndex &) const
{
    return static_cast<int>(torrents_.size());
}

QVariant TorrentList::data(const QModelIndex &index, int role) const
{
    QVariant data;

    if (role == TORRENT_ROLE)
    {
        Torrent *t = const_cast<Torrent *>(&(torrents_.at(static_cast<std::size_t>(index.row()))));
        data = QVariant::fromValue(t);
    }

    return data;
}

QHash<int, QByteArray> TorrentList::roleNames() const
{
    return {{ TORRENT_ROLE, "torrent" }};
}

void TorrentList::update(std::vector<librt::Torrent> &&torrents)
{
    cached_ = std::move(torrents);
    QMetaObject::invokeMethod(this, "onCachedTorrentsUpdated", Qt::QueuedConnection);
}

void TorrentList::onCachedTorrentsUpdated()
{
    std::vector<librt::Torrent> tl;
    if (session_ == Q_NULLPTR)
    {
        cached_.clear();
        torrents_.clear();
        return;
    }
    tl = std::move(cached_);
    cached_.clear();
    session_->torrentUpdateLock_.unlock();

    if (tl.empty())
    {
        beginResetModel();
        torrents_.clear();
        endResetModel();

        return;
    }

    int currentTorrentCount = static_cast<int>(torrents_.size());
    for (int cit = 0; cit < currentTorrentCount; ++cit)
    {
        int newTorrentCount = static_cast<int>(tl.size());
        auto &currentTorrent = torrents_[cit];
        bool found = false;
        for (int nit = 0; nit < newTorrentCount; ++nit)
        {
            auto &newTorrent = tl[nit];

            if (currentTorrent.id() == newTorrent.id())
            {
                currentTorrent = std::move(newTorrent);
                tl.erase(tl.begin() + nit);
                found = true;
                break;
            }
        }
        if (!found)
        {
            beginRemoveRows(QModelIndex(), cit, cit);
            auto pos = torrents_.begin() + cit;
            torrents_.erase(pos);
            --cit;
            --currentTorrentCount;
            endRemoveRows();
        }
    }

    std::vector<librt::Torrent *> newTorrents;
    for (auto it = torrents_.begin(); it != torrents_.end(); ++it)
    {
        newTorrents.push_back(&(it->torrent_));
    }
    for (auto it = tl.begin(); it != tl.end(); ++it)
    {
        newTorrents.push_back(&(*it));
    }

    if (sorting_ == SortByName)
    {
        std::sort(newTorrents.begin(), newTorrents.end(), [&](const librt::Torrent *t1, const librt::Torrent *t2) {
            QString name1 = QString::fromUtf8(t1->name().c_str());
            QString name2 = QString::fromUtf8(t2->name().c_str());
            return (name1.compare(name2, Qt::CaseInsensitive) < 0);
        });
    }
    else if (sorting_ == SortByQueuePosition)
    {
        std::sort(newTorrents.begin(), newTorrents.end(), [&](const librt::Torrent *t1, const librt::Torrent *t2) {
            return (t1->queuePosition() < t2->queuePosition());
        });
    }

    if (torrents_.empty())
    {
        beginResetModel();
        for (auto &&t: newTorrents)
        {
            torrents_.emplace_back(std::move(*t));
        }
        endResetModel();
        return;
    }

    for (auto &&t: tl)
    {
        auto it = std::find(newTorrents.begin(), newTorrents.end(), &t);
        if (it != newTorrents.end())
        {
            int row = static_cast<int>(it - newTorrents.begin());
            beginInsertRows(QModelIndex(), row, row);
            torrents_.insert(torrents_.begin() + row, std::move(t));
            endInsertRows();
        }
    }
}
