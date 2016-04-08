#include <QCoreApplication>
#include <QDateTime>
#include "settings.h"
#include "server.h"
#include <unistd.h>
#include "eventdispatcher_libevent.h"

#include "mdb.h"

int main(int argc, char *argv[])
{
    //QCoreApplication::setEventDispatcher(new QEventDispatcherEpoll());
    EventDispatcherLibEvent dispatcher;
    QCoreApplication a(argc, argv);
    qRegisterMetaType<AbsConnection*>("AbsConnection*");

    initSettings();

//    bool isFork = _settings.value("fork").toBool();
//    if (isFork){
//        pid_t pid = 0;
//        pid=fork();
//        if (pid == -1)
//        {
//            return EXIT_FAILURE;
//        }
//        else if (pid > 0)
//        {
//            return EXIT_SUCCESS;
//        }
//    }
    qsrand(QDateTime::currentDateTime().toTime_t());
    Server s;
    if (false == s.listen(QHostAddress::Any, _settings.value("port").toInt()))
    {
        return EXIT_FAILURE;
    }
    qDebug() << "GCS" << s.serverPort();
    return a.exec();
}
