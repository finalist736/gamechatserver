#ifndef CONNECTION_H
#define CONNECTION_H

#include "absconnection.h"
#include <QDebug>
#include "authorizer.h"

class Connection : public AbsConnection
{
    Q_OBJECT
    QByteArray m_readData;
    QList<QByteArray> m_commandList;
    Authorizer m_auth;

public:
    explicit Connection(QObject *parent = 0);
    ~Connection();

signals:

public slots:
    void send(QVariantList list);
    void letsRead();
};

#endif // CONNECTION_H
