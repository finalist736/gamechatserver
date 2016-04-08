#ifndef SETTINGS_H
#define SETTINGS_H

#include <QSettings>

struct Oper
{
    QByteArray login, password;
};

struct ArealData
{
    QList<Oper> opers;
    QByteArray name, key;
    quint32 channels, conn;
    QByteArray banned_message, unbanned_message;
};

static QSettings _settings(QString("gcs.ini"), QSettings::IniFormat);

void initSettings();

#endif // SETTINGS_H
