#ifndef TORRENT_LIST_H
#define TORRENT_LIST_H

#include <QAbstractListModel>
#include <QPointer>

class Session;

class TorrentList: public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(Session* session READ session WRITE setSession NOTIFY sessionChanged)
    Q_PROPERTY(int count        READ count                    NOTIFY countChanged)

public:
    TorrentList(QObject *parent = Q_NULLPTR);

public:
    Session *session();
    void setSession(Session *session);

    int count() const;

signals:
    void sessionChanged();
    void countChanged();

public:
    int rowCount(const QModelIndex &parent) const Q_DECL_OVERRIDE;
    QVariant data(const QModelIndex &index, int role) const Q_DECL_OVERRIDE;

private:
    QPointer<Session> session_;
    int count_;
};

#endif // TORRENT_LIST_H
