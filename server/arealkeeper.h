#ifndef AREALKEEPER_H
#define AREALKEEPER_H

#include <QObject>
#include <QHash>
#include "areal.h"

class ArealKeeper : public QObject
{
    Q_OBJECT
    QHash<quint32, Areal*> m_areals;

    explicit ArealKeeper(QObject *parent = 0);
public:
    static ArealKeeper *instance();

    void dispatch(quint32 id, QList<QByteArray> list, AbsConnection *who);
    Areal *add(quint32 id, AbsConnection *connection);
    void remove(quint32 id, AbsConnection *connection);

    bool contains(const QByteArray &key);
    quint32 getId(const QByteArray &key);

signals:

public slots:

};

#endif // AREALKEEPER_H
