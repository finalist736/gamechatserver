#ifndef AUTHORIZER_H
#define AUTHORIZER_H

#include <QObject>
#include <QVariant>
#include <QVariantMap>
#include "arealkeeper.h"
#include "mdb.h"

class Authorizer : public QObject
{
    Q_OBJECT
    AbsConnection *m_connection;
    bool isAuthorized;
    quint32 m_areal;
public:
    explicit Authorizer(AbsConnection *parent = 0);

    void dispatch(const QList<QByteArray>& data);
    void remove();

signals:

public slots:

};

#endif // AUTHORIZER_H
