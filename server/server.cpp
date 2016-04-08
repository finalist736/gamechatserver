#include "server.h"

Server::Server() :QTcpServer()
{
}

void Server::incomingConnection(qintptr handle)
{
    qDebug() << "new connection" << handle;

    Connection * c = new Connection(this);
    c->setSocketDescriptor(handle);
}
