#include "areal.h"
#include <QDateTime>

AbsConnection *Areal::getConnectionByName(const QByteArray &name)
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

void Areal::ban(const QByteArray &text, AbsConnection *who)
{
    QList<QByteArray> list = text.split(' ');
    if (list.length() < 2) return;
    QByteArray reason = "";
    quint32 minutes = 5;

    if (list.length() > 3)
    {
        for (QList<QByteArray>::iterator it = list.begin() + 3;it!=list.end();++it){
            QByteArray tmp = *it;
            qDebug() << tmp;
            reason += tmp + " ";
        }
        reason = reason.trimmed();
    }

    if (list.length() > 2)
    {
        minutes = list.at(2).toInt();
        if (minutes <= 0)
        {
            minutes = 5;
        }
    }
    QByteArray name = list.at(1);
    AbsConnection * banned = getConnectionByName(name);
    if (banned != NULL)
    {
        quint32 now = QDateTime::currentDateTime().toTime_t();
        m_banlist[name] = now + (minutes * 60);
        // нужно оповестить пользователей что кто-то забанен, пока оповестим только забаненного
        QByteArray result_reason = getArealData().banned_message;
        result_reason = result_reason.replace("%m", QByteArray::number(minutes));
        result_reason = result_reason.replace("%r", reason);
        banned->send(QVariantList() << "PM" << getArealData().name << result_reason);
        who->send(QVariantList() << "PM" << getArealData().name << "/ban ok");
        return;
    }
    who->send(QVariantList() << "PM" << getArealData().name << "/ban " + name + " not found");
}

void Areal::unban(const QByteArray &text, AbsConnection *who)
{
    QList<QByteArray> list = text.split(' ');
    if (list.length() < 2) return;
    QByteArray name = list.at(1);
    if (m_banlist.contains(name))
    {
        m_banlist.remove(name);
        AbsConnection * banned = getConnectionByName(name);
        if (banned != NULL)
        {
            banned->send(QVariantList() << "PM" << getArealData().name << getArealData().unbanned_message);
            who->send(QVariantList() << "PM" << getArealData().name << "/unban ok");
        }
        return;
    }
    who->send(QVariantList() << "PM" << getArealData().name << "/unban " + name + " not found");
}

void Areal::removeFromBanlist()
{
    QList<QByteArray> toRemove;
    QHash<QByteArray, quint32>::iterator it;
    for (it=m_banlist.begin(); it!=m_banlist.end();++it)
    {
        quint32 now = QDateTime::currentDateTime().toTime_t();
        if (it.value() >= now)
        {
            toRemove << it.key();
        }
    }
    QList<QByteArray>::iterator il;
    for (il=toRemove.begin(); il!=toRemove.end();++il)
    {
        m_banlist.remove(*il);
    }
}

Areal::Areal(quint32 id, QObject *parent) :
    QObject(parent), m_id(id), m_next_channel(0)
{
    qDebug() << "Areal::Areal()" << id;
}

void Areal::dispatch(QList<QByteArray> list, AbsConnection *who)
{
    removeFromBanlist();
    if (list.length() == 0)
    {
        return;
    }
    QByteArray cmd = list.at(0);

    if (cmd == "CC")
    {
        if (list.length() == 2)
        {
            bool exists = false;
            QByteArray name = list.at(1);
            QHash<quint32, Channel*>::iterator it;
            for (it=m_channels.begin();it!=m_channels.end();++it)
            {
                if (name == it.value()->name())
                {
                    exists = true;
                    it.value()->add(who);
                    who->send(QVariantList() << "CC" << name << it.value()->id());
                    break;
                }
            }
            if (!exists)
            {
                ++m_next_channel;
                Channel * channel = m_channels[m_next_channel] = new Channel(m_next_channel, name, this);
                channel->add(who);
                who->send(QVariantList() << "CC" << name << channel->id());
            }
        }
    }
    else if (cmd == "LC")
    {
        if (list.length() == 2)
        {
            QByteArray name = list.at(1);
            QHash<quint32, Channel*>::iterator it;
            for (it=m_channels.begin();it!=m_channels.end();++it)
            {
                Channel * channel = it.value();
                if (name == channel->name())
                {
                    channel->remove(who);
                    if (channel->isEmpty())
                    {
                        channel->deleteLater();
                        m_channels.remove(it.key());
                    }
                    who->send(QVariantList() << "LC" << "OK");
                    break;
                }
            }
        }
    }
    else if (cmd == "SM")
    {
        if (list.length() == 3)
        {
            quint32 id = list.at(1).toInt();
            if (m_channels.contains(id))
            {
                QByteArray text = list.at(2);
                Channel * channel = m_channels.value(id);

                if (text.left(1) == "/")
                {
                    if (text.left(4) == "/ban")
                    {
                        ban(text, who);
                    }
                    else if (text.left(6) == "/unban")
                    {
                        unban(text, who);
                    }
                    else
                    {
                        m_cmd.dispatch(text, who, channel);
                    }
                }
                else
                {
                    channel->sendMessage(who, text);
                }
            }
        }
    }
    else if (cmd == "PM")
    {
        if (list.length() == 3)
        {
            QByteArray name = list.at(1);
            QByteArray text = list.at(2);
            QLinkedList<AbsConnection*>::iterator it;
            for (it=m_connections.begin();it!=m_connections.end();++it)
            {
                AbsConnection * conn = (*it);
                if (conn->name() == name)
                {
                    conn->send(QVariantList() << "PM" << who->name() << text);
                    who->send(QVariantList() << "PM" << "OK" << text);
                    break;
                }
            }
            who->send(QVariantList() << "PM" << "ERR" << "NOT FOUND");
        }
    }
    else if (cmd == "UL")
    {
        if (list.length() == 2)
        {
            quint32 id = list.at(1).toInt();
            if (m_channels.contains(id))
            {
                Channel * channel = m_channels.value(id);
                who->send(QVariantList() << "UL" << id << channel->getUserList());
            }
        }
    }
}

void Areal::add(AbsConnection *connection)
{
    m_connections << connection;
}

void Areal::remove(AbsConnection *connection)
{
    QList<quint32> toRemove;
    m_connections.removeOne(connection);
    QHash<quint32, Channel*>::iterator it;
    for (it=m_channels.begin();it!=m_channels.end();++it)
    {
        Channel * channel = it.value();
        if (channel->contains(connection))
        {
            channel->remove(connection);
            if (channel->isEmpty())
            {
                toRemove << channel->id();
                qDebug() << "channel is empty, mark to remove" << channel->id() << channel->name();
            }
        }
    }
    for (int i=0;i<toRemove.length();++i)
    {
        quint32 id = toRemove.at(i);
        if (m_channels.contains(id))
        {
            m_channels[id]->deleteLater();
            m_channels.remove(id);
            qDebug() << "channel removed" << id;
        }
    }

}

bool Areal::isEmpty()
{
    return m_connections.isEmpty();
}
