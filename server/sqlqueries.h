#ifndef SQLQUERIES_H
#define SQLQUERIES_H

#include <QString>

static const QString SQL_AUTH_AUTH = "SELECT * FROM `areals` WHERE `key`=?";
static const QString SQL_AUTH_OPERS = "SELECT * FROM `opers` WHERE `areal`=?";

#endif // SQLQUERIES_H
