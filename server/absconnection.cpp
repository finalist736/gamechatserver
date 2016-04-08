#include "absconnection.h"

AbsConnection::AbsConnection(QObject *parent) :
    QTcpSocket(parent), m_name(""), m_oper(false)
{
}

AbsConnection::~AbsConnection()
{
    qDebug() << "remove" << this;
}
