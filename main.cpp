#include <QCoreApplication>
#include "server.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    Server *server = new Server();
    bool success = server->listen(QHostAddress::Any, 4200);
    if(!success)
    {
        qFatal("Could not listen on port 4200.");
    }

    qDebug() << "Listening...";

    return a.exec();
}
