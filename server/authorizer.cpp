#include "authorizer.h"
#include <QSqlRecord>

Authorizer::Authorizer(AbsConnection *parent) :
    QObject(parent), m_connection(parent), isAuthorized(false)
{

}

void Authorizer::dispatch(const QList<QByteArray> &data)
{
    if (data.empty()) return;
    if (isAuthorized)
    {
        ArealKeeper::instance()->dispatch(m_areal, data, m_connection);
    }
    else
    {
        QList<QByteArray> copy(data);
        if (copy.length() == 3)
        {
            QByteArray cmd = copy.first();
            if (cmd == "AU")
            {
                QByteArray key = copy.at(1);
                QByteArray name = copy.at(2);

                if (ArealKeeper::instance()->contains(key))
                {
                    m_areal = ArealKeeper::instance()->getId(key);
                    m_connection->setName(name);
                    ArealKeeper::instance()->add(m_areal, m_connection);
                    m_connection->send(QVariantList() << "AU" << "OK");
                    isAuthorized = true;
                    return;
                }
                else
                {
                    MDB db;
                    MQuery query = db.getQuery();
                    query->prepare(SQL_AUTH_AUTH);
                    query->bindValue(0, QString(key));
                    if (query->exec())
                    {
                        if (query->next())
                        {
                            ArealData data;
                            m_areal = query->value("id").toUInt();
                            data.name = query->value("name").toByteArray();
                            data.channels = query->value("channels").toUInt();
                            data.conn = query->value("conn").toUInt();
                            data.key = query->value("key").toByteArray();
                            data.banned_message = "Вас забанили на %m. Причина: %r";
                            data.unbanned_message = "Вас разбанили";

                            query->finish();
                            query->prepare(SQL_AUTH_OPERS);
                            query->bindValue(0, m_areal);
                            if (query->exec())
                            {
                                while (query->next())
                                {
                                    Oper oper;
                                    oper.login = query->value("login").toByteArray();
                                    oper.password = query->value("password").toByteArray();
                                    data.opers.append(oper);
                                }
                            }
                            else
                            {
                                qDebug() << query->lastError();
                            }
                            m_connection->setName(name);
                            Areal *areal = ArealKeeper::instance()->add(m_areal, m_connection);
                            areal->setArealData(data);
                            m_connection->send(QVariantList() << "AU" << "OK");
                            isAuthorized = true;
                            return;
                        }
                    }
                    else
                    {
                        qDebug() << query->lastError();
                    }
                }
            }
        }
        m_connection->send(QVariantList() << "AU" << "ERROR");
    }
}

void Authorizer::remove()
{
    ArealKeeper::instance()->remove(m_areal, m_connection);
}
