#ifndef COMMANDS_H
#define COMMANDS_H

#include <QObject>
#include "absconnection.h"
#include "channel.h"
#include "settings.h"

class Commands : public QObject
{
    Q_OBJECT
    ArealData m_data;

public:
    explicit Commands(QObject *parent = 0);
    void dispatch(const QByteArray &text, AbsConnection *who, Channel *channel);

    inline void setArealData(const ArealData& data){
        m_data = data;
    }
    inline ArealData getArealData(){return m_data;}

signals:

public slots:

};

#endif // COMMANDS_H
