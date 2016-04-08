#ifndef SERVER_H
#define SERVER_H

#include <QObject>
#include <QTcpServer>
#include "connection.h"

class Server : public QTcpServer
{
    Q_OBJECT
public:
    Server();
    void incomingConnection(qintptr handle);
public slots:
};

#endif // SERVER_H
