#include "connection.h"

Connection::Connection(QObject *parent) :
    AbsConnection(parent), m_auth(this)
{
    connect(this, SIGNAL(disconnected()), SLOT(deleteLater()), Qt::QueuedConnection);
    connect(this, SIGNAL(readyRead()), SLOT(letsRead()), Qt::QueuedConnection);
    //connect(&m_auth, SIGNAL(send(QVariantList)), SLOT(send(QVariantList)), Qt::QueuedConnection);
}

Connection::~Connection()
{
    qDebug() << "disconnected";
    m_auth.remove();
}

void Connection::send(QVariantList list)
{
    QByteArray ba;
    QVariantList::iterator it;
    for (it=list.begin();it!=list.end();it++)
    {
        ba += (*it).toByteArray();
        ba += "\n";
    }
    ba += ".";
    if (state() == QAbstractSocket::ConnectedState)
    {
        qDebug() << list;
        write(ba);
    }
    else
    {
        qDebug() << "sending error";
    }
}

void Connection::letsRead()
{
    m_readData += readAll();
    qDebug() << m_readData;
    m_commandList << m_readData.split('\n');
    m_commandList.removeAll("");
    m_readData.clear();
    qDebug() << m_commandList;
    for(;m_commandList.contains(".");)
    {
        QList<QByteArray> list;
        QList<QByteArray> copy(m_commandList);
        while (!copy.isEmpty())
        {
            QByteArray ba = copy.first();
            if (ba!=".")
            {
                list << ba;
                copy.removeFirst();
            }
            else
            {
                copy.removeFirst();
                if (!copy.isEmpty())
                {
                    m_commandList = copy;
                }
                else
                {
                    m_commandList.clear();
                }
            }
        }
        m_auth.dispatch(list);
    }
}
