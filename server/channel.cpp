#include "channel.h"
#include <QDebug>

Channel::Channel(quint32 id, const QByteArray &name, QObject *parent) :
    QObject(parent), m_id(id), m_name(name)
{
    qDebug() << "create channel" << id << name;
}

void Channel::add(AbsConnection *connection)
{
    m_connections << connection;
}

bool Channel::isEmpty()
{
    return m_connections.isEmpty();
}

bool Channel::contains(AbsConnection *who)
{
    QLinkedList<AbsConnection*>::iterator it;
    for (it=m_connections.begin();it!=m_connections.end();++it)
    {
        AbsConnection * conn = (*it);
        if (conn == who)
        {
            return true;
        }
    }
    return false;
}

void Channel::sendMessage(AbsConnection *who, const QByteArray &text)
{
    QLinkedList<AbsConnection*>::iterator it;
    for (it=m_connections.begin();it!=m_connections.end();++it)
    {
        AbsConnection * conn = (*it);
        conn->send(QVariantList() << "SM" << id() << who->name() << text);
    }
}

QByteArray Channel::getUserList()
{
    QByteArray users;
    QLinkedList<AbsConnection*>::iterator it;
    for (it=m_connections.begin();it!=m_connections.end();++it)
    {
        AbsConnection * conn = (*it);
        users += conn->name() + "\n";
    }
    users = users.trimmed();
    return users;
}

AbsConnection *Channel::getConnectionByName(const QByteArray &name)
{
    QLinkedList<AbsConnection*>::iterator it;
    for (it=m_connections.begin();it!=m_connections.end();++it)
    {
        AbsConnection * conn = (*it);
        if (conn->name() == name)
        {
            return conn;
        }
    }
    return NULL;
}

quint32 Channel::getUserCount()
{
    quint32 cnt = 0;
    QLinkedList<AbsConnection*>::iterator it;
    for (it=m_connections.begin();it!=m_connections.end();++it)
    {
        cnt++;
    }
    return cnt;
}

void Channel::remove(AbsConnection *connection)
{
    m_connections.removeOne(connection);
    qDebug() << "removed from channel" << connection;
}
