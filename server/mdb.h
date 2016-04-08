#ifndef MDB_H
#define MDB_H

#include <QObject>
#include <QSqlDatabase>
#include <QDebug>
#include <QStringList>
#include <QSqlQuery>
#include <QSqlError>
#include <QSharedPointer>
#include "sqlqueries.h"

const static QString __mysql_connection_name = "mysqlTaxiServConnection";
const static QString __mysql_database_type = "QSQLITE";//"QMYSQL";

typedef QSharedPointer<QSqlQuery> MQuery;

class MDB : public QObject
{
    Q_OBJECT
    QSqlDatabase getDB();

public:
    explicit MDB(QObject *parent = 0);
    ~MDB();

    bool openDB();
    MQuery getQuery();
    QString lastError();

signals:

public slots:

};

#endif // MDB_H
