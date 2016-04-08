#include "mdb.h"
#include "settings.h"

QSqlDatabase MDB::getDB()
{
    return QSqlDatabase::database(__mysql_connection_name);
}

MDB::MDB(QObject *parent) :
    QObject(parent)
{
    if (!QSqlDatabase::database(__mysql_connection_name, false).isValid())
    {
        QSqlDatabase::addDatabase(__mysql_database_type, __mysql_connection_name);
    }
}

MDB::~MDB()
{
    //QSqlDatabase::removeDatabase(__mysql_connection_name);
}

bool MDB::openDB()
{
    QSqlDatabase db = QSqlDatabase::database(__mysql_connection_name, false);
    if (!db.isOpen())
    {
        QString host = _settings.value("mdbhost").toString();
        QString user = _settings.value("mdbuser").toString();
        QString pass = _settings.value("mdbpass").toString();
        QString name = _settings.value("mdbname").toString();
        int     port = _settings.value("mdbport").toInt();

        db.setHostName(host);
        db.setDatabaseName(name);
        db.setPort(port);
        db.setUserName(user);
        db.setPassword(pass);
        return db.open();
    }
    return true;
}

MQuery MDB::getQuery()
{
    openDB();
    return MQuery(new QSqlQuery(getDB()));
}

QString MDB::lastError()
{
    return getDB().lastError().text();
}
