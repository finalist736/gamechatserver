#ifndef CHANNEL_H
#define CHANNEL_H

#include <QObject>
#include <QLinkedList>
#include "absconnection.h"

class Channel : public QObject
{
    Q_OBJECT
    quint32 m_id;
    QByteArray m_name;
    QLinkedList<AbsConnection*> m_connections;
public:
    explicit Channel(quint32 id, const QByteArray &name, QObject *parent = 0);
    void add(AbsConnection *connection);
    bool isEmpty();
    bool contains(AbsConnection *who);
    void remove(AbsConnection *connection);
    void sendMessage(AbsConnection *who, const QByteArray &text);
    QByteArray getUserList();
    AbsConnection *getConnectionByName(const QByteArray &name);
    quint32 getUserCount();

    inline quint32 id()
    {return m_id;}
    inline QByteArray name()
    {return m_name;}

signals:

public slots:


};

#endif // CHANNEL_H
