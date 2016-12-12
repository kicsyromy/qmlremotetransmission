#include "torrentlist.h"

#include "session.h"

namespace
{
    constexpr const auto TORRENT_ROLE { Qt::DisplayRole + 1 };
}

TorrentList::TorrentList(QObject *parent) :
    QAbstractListModel(parent),
    cached_(),
    torrents_(),
    mutex_(),
    sorting_(SortByName)
{
}

Session *TorrentList::session()
{
    return session_;
}

void TorrentList::setSession(Session *session)
{
    if (session_ != session)
    {
        if (!session_.isNull())
        {
            session_->removeTorrentList(this);
        }

        session_ = session;

        if (!session_.isNull())
        {
            session_->addTorrentList(this);
        }

        emit sessionChanged();
    }
}

int TorrentList::count() const
{
    return static_cast<int>(torrents_.size());
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

void TorrentList::update(std::vector<librt::Torrent> &&torrents)
{
    std::lock_guard<std::mutex> lock(mutex_);
    {
        cached_ = std::move(torrents);
    }
    QMetaObject::invokeMethod(this, "onCachedTorrentsUpdated", Qt::QueuedConnection);
}

void TorrentList::onCachedTorrentsUpdated()
{
    std::vector<librt::Torrent> tl;

    std::lock_guard<std::mutex> lock(mutex_);
    {
        if (session_ == Q_NULLPTR)
        {
            cached_.clear();
            torrents_.clear();
            return;
        }

        tl.reserve(cached_.size());
        for (auto &&torrent: cached_)
        {
            tl.push_back(std::move(torrent));
        }
    }

    if (tl.empty())
    {
        beginResetModel();
        torrents_.clear();
        endResetModel();

        return;
    }

    int currentTorrentCount = static_cast<int>(torrents_.size());
    int newTorrentCount = static_cast<int>(tl.size());
    for (int cit = 0; cit < currentTorrentCount; ++cit)
    {
        auto &currentTorrent = torrents_[cit];
        bool found = false;
        for (int nit = 0; nit < newTorrentCount; ++nit)
        {
            auto &newTorrent = tl[nit];

            if (currentTorrent.id() == newTorrent.id())
            {
                currentTorrent = std::move(newTorrent);
                tl.erase(tl.begin() + nit);
                --newTorrentCount;
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

    for (auto &t: tl)
    {
        auto it = std::find(newTorrents.begin(), newTorrents.end(), &t);
        if (it != newTorrents.end())
        {
            int row = static_cast<int>(it - newTorrents.begin());
            beginInsertRows(QModelIndex(), row, row);
            torrents_.emplace(torrents_.begin() + row, std::move(t));
            endInsertRows();
        }
    }

    emit countChanged();
}
