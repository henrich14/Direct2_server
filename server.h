#ifndef SERVER_H
#define SERVER_H

#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>
#include <QMap>
#include <QSet>
#include <QDateTime>

#include "waypoint.h"
#include "flightplan.h"

class Server : public QTcpServer
{
    Q_OBJECT

public:
    Server(QObject *parent=0);

    FlightPlan message2FP(const QString &message);
    QString FP2Message(FlightPlan FP);
    FlightPlan findAlternativeFP(FlightPlan FP);

private slots:
    void readyREAD();
    void disconnected();

protected:
    void incomingConnection(int socketfd);

private:
    QSet<QTcpSocket*> clients;
    QMap<QTcpSocket*,QString> users;

    int nxtWP_idx;
};

#endif // SERVER_H
