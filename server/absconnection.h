#ifndef ABSCONNECTION_H
#define ABSCONNECTION_H

#include <QTcpSocket>
#include <QVariantList>
#include <QByteArray>

class AbsConnection : public QTcpSocket
{
    Q_OBJECT
    QByteArray m_name;
    bool m_oper;
public:
    explicit AbsConnection(QObject *parent = 0);
    virtual ~AbsConnection();
    virtual void send(QVariantList list) = 0;

    inline void setName(const QByteArray& name)
    {
        m_name = name;
    }
    inline QByteArray name(){return m_name;}

    inline void setOper(const bool& oper)
    {
        m_oper = oper;
    }
    inline bool oper(){return m_oper;}

signals:

public slots:

};

#endif // ABSCONNECTION_H
