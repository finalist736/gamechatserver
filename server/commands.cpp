#include "commands.h"

Commands::Commands(QObject *parent) :
    QObject(parent)
{
}

void Commands::dispatch(const QByteArray &text, AbsConnection *who, Channel *channel)
{
    if (text.left(5) == "/oper")
    {
        if (who->oper())
        {
            QList<QByteArray> list = text.split(' ');
            if (list.length() == 2)
            {
                QByteArray name = list.at(1);
                AbsConnection *conn = channel->getConnectionByName(name);
                if (conn != NULL)
                {
                    conn->setOper(true);
                    who->send(QVariantList() << "SM" << getArealData().name << "/oper = ok");
                    conn->send(QVariantList() << "SM" << getArealData().name << "/oper = ok");
                }
            }
        }
        else
        {
            QList<QByteArray> list = text.split(' ');
            if (list.length() == 3)
            {
                QByteArray login = list.at(1);
                QByteArray pass = list.at(2);

                int len = getArealData().opers.length();
                qDebug() << len;
                for (int i=0; i<len; ++i)
                {
                    Oper oper = getArealData().opers.at(i);
                    if (oper.login == login && oper.password == pass)
                    {
                        who->setOper(true);
                        who->send(QVariantList() << "SM" << getArealData().name << "/oper = ok");
                    }
                }

                return;
            }
            who->send(QVariantList() << "ERR" << "syntax error");
        }
    }
    else if (text.left(7) == "/uc")
    {
        QString cnt = QString::number(channel->getUserCount());
        who->send(QVariantList() << "SM" << getArealData().name << cnt);
    }
    else if (text.left(7) == "/rehash")
    {

    }
    else if (text.left(7) == "/ban")
    {
        QList<QByteArray> list = text.split(' ');
        if (list.length() == 3)
        {

        }
    }
}
