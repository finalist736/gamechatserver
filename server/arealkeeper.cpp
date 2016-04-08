#include "arealkeeper.h"

ArealKeeper *ArealKeeper::instance()
{
    static ArealKeeper * g_areal = new ArealKeeper();
    return g_areal;
}

void ArealKeeper::dispatch(quint32 id, QList<QByteArray> list, AbsConnection *who)
{
    if (m_areals.contains(id))
    {
        Areal *areal = m_areals[id];
        areal->dispatch(list, who);
    }
}

Areal *ArealKeeper::add(quint32 id, AbsConnection *connection)
{
    Areal *areal;
    if (m_areals.contains(id))
    {
        areal = m_areals[id];
    }
    else
    {
        m_areals[id] = areal = new Areal(id, this);
    }
    areal->add(connection);
    qDebug() << "add connection to keeper" << id << connection;
    return areal;
}

void ArealKeeper::remove(quint32 id, AbsConnection *connection)
{
    qDebug() << "removing connection from keeper" << id << connection;
    if (m_areals.contains(id))
    {
        Areal *areal = m_areals[id];
        areal->remove(connection);
        if (areal->isEmpty())
        {
            qDebug() << "removing areal from keeper" << id;
            areal->deleteLater();
            m_areals.remove(id);
        }
    }
}

bool ArealKeeper::contains(const QByteArray &key)
{
    QHash<quint32, Areal*>::iterator it;
    for (it=m_areals.begin(); it!=m_areals.end(); ++it)
    {
        Areal *areal = it.value();
        if (areal->getArealData().key == key)
        {
            return true;
        }
    }
    return false;
}

quint32 ArealKeeper::getId(const QByteArray &key)
{
    QHash<quint32, Areal*>::iterator it;
    for (it=m_areals.begin(); it!=m_areals.end(); ++it)
    {
        Areal *areal = it.value();
        if (areal->getArealData().key == key)
        {
            return areal->id();
        }
    }
    return 0;
}

ArealKeeper::ArealKeeper(QObject *parent) :
    QObject(parent)
{
}
