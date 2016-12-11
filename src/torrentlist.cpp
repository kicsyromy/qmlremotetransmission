#include "torrentlist.h"

#include "session.h"

TorrentList::TorrentList(QObject *parent) :
    QAbstractListModel(parent)
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
    return count_;
}

int TorrentList::rowCount(const QModelIndex &) const
{
    return count_;
}

QVariant TorrentList::data(const QModelIndex &, int role) const
{
}
