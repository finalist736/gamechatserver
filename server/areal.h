#ifndef AREAL_H
#define AREAL_H

#include <QObject>
#include <QDebug>
#include <QLinkedList>
#include "absconnection.h"
#include "channel.h"
#include "commands.h"
#include "settings.h"

class Areal : public QObject
{
    Q_OBJECT
    quint32 m_id;
    quint32 m_next_channel;
    QHash<quint32, Channel*> m_channels;
    QLinkedList<AbsConnection *> m_connections;

    ArealData m_data;
    Commands m_cmd;

    QHash<QByteArray, quint32> m_banlist;


    AbsConnection *getConnectionByName(const QByteArray &name);

    void ban(const QByteArray& text, AbsConnection *who);
    void unban(const QByteArray& text, AbsConnection *who);
    void removeFromBanlist();

public:
    explicit Areal(quint32 id, QObject *parent = 0);
    void dispatch(QList<QByteArray> list, AbsConnection *who);
    void add(AbsConnection *connection);
    void remove(AbsConnection *connection);
    bool isEmpty();

    inline void setArealData(const ArealData& data){
        m_data = data;
        m_cmd.setArealData(data);
    }
    inline ArealData getArealData(){return m_data;}
    inline quint32 id(){return m_id;}

signals:

public slots:


};

#endif // AREAL_H
