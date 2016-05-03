#include "server.h"

Server::Server(QObject *parent) :
    QTcpServer(parent)
{
    // set seed for random generator
    qsrand(QDateTime::currentDateTime().toTime_t());
}

FlightPlan Server::message2FP(const QString &message)
{
    QStringList messageList = message.split(";");

    uint unixtime = messageList[0].toUInt();

    Waypoint WP0(messageList[1].split(",")[0].toDouble(), messageList[1].split(",")[1].toDouble(),messageList[1].split(",")[2].toDouble(),
            messageList[1].split(",")[3], messageList[1].split(",")[4].toInt());
    Waypoint WP1(messageList[2].split(",")[0].toDouble(), messageList[2].split(",")[1].toDouble(),messageList[2].split(",")[2].toDouble(),
            messageList[2].split(",")[3], messageList[2].split(",")[4].toInt());
    Waypoint WP2(messageList[3].split(",")[0].toDouble(), messageList[3].split(",")[1].toDouble(),messageList[3].split(",")[2].toDouble(),
            messageList[3].split(",")[3], messageList[3].split(",")[4].toInt());
    Waypoint WP3(messageList[4].split(",")[0].toDouble(), messageList[4].split(",")[1].toDouble(),messageList[4].split(",")[2].toDouble(),
            messageList[4].split(",")[3], messageList[4].split(",")[4].toInt());
    Waypoint WP4(messageList[5].split(",")[0].toDouble(), messageList[5].split(",")[1].toDouble(),messageList[5].split(",")[2].toDouble(),
            messageList[5].split(",")[3], messageList[5].split(",")[4].toInt());
    Waypoint WP5(messageList[6].split(",")[0].toDouble(), messageList[6].split(",")[1].toDouble(),messageList[6].split(",")[2].toDouble(),
            messageList[6].split(",")[3], messageList[6].split(",")[4].toInt());
    Waypoint WP6(messageList[7].split(",")[0].toDouble(), messageList[7].split(",")[1].toDouble(),messageList[7].split(",")[2].toDouble(),
            messageList[7].split(",")[3], messageList[7].split(",")[4].toInt());
    Waypoint WP7(messageList[8].split(",")[0].toDouble(), messageList[8].split(",")[1].toDouble(),messageList[8].split(",")[2].toDouble(),
            messageList[8].split(",")[3], messageList[8].split(",")[4].toInt());
    Waypoint WP8(messageList[9].split(",")[0].toDouble(), messageList[9].split(",")[1].toDouble(),messageList[9].split(",")[2].toDouble(),
            messageList[9].split(",")[3], messageList[9].split(",")[4].toInt());
    Waypoint WP9(messageList[10].split(",")[0].toDouble(), messageList[10].split(",")[1].toDouble(),messageList[10].split(",")[2].toDouble(),
            messageList[10].split(",")[3], messageList[10].split(",")[4].toInt());
    Waypoint WP10(messageList[11].split(",")[0].toDouble(), messageList[11].split(",")[1].toDouble(),messageList[11].split(",")[2].toDouble(),
            messageList[11].split(",")[3], messageList[11].split(",")[4].toInt());
    Waypoint WP11(messageList[12].split(",")[0].toDouble(), messageList[12].split(",")[1].toDouble(),messageList[12].split(",")[2].toDouble(),
            messageList[12].split(",")[3], messageList[12].split(",")[4].toInt());

    nxtWP_idx = messageList[13].toInt();

    FlightPlan *FP = new FlightPlan(unixtime, WP0, WP1, WP2, WP3, WP4, WP5, WP6, WP7, WP8, WP9, WP10, WP11);

    return *FP;
}

QString Server::FP2Message(FlightPlan FP)
{
    // convert flight plan to string (serialization)
    QStringList messageList;

    messageList << QString::number(FP.timeStamp());

    for(int i=0; i<=11; i++)
    {
        QString WP = QString::number(FP.WP(i).x()) + "," + QString::number(FP.WP(i).y())+ "," + QString::number(FP.WP(i).altitude())
                + "," + FP.WP(i).label()+ "," + QString::number(FP.WP(i).mandatory());
        messageList << WP;
    }

    return messageList.join(";");
}

FlightPlan Server::findAlternativeFP(FlightPlan FP)
{
    // iterate over WP from nxtWP index to find alternative FP, 2 full legs before TOD has to stay unchanged
    uint unixtime = FP.timeStamp();
    Waypoint WP0 = FP.WP(0);
    Waypoint WP1 = FP.WP(1);
    Waypoint WP2 = FP.WP(2);
    Waypoint WP3 = FP.WP(3);
    Waypoint WP4 = FP.WP(4);
    Waypoint WP5 = FP.WP(5);
    Waypoint WP6 = FP.WP(6);
    Waypoint WP7 = FP.WP(7);
    Waypoint WP8 = FP.WP(8);
    Waypoint WP9 = FP.WP(9);
    Waypoint WP10 = FP.WP(10);
    Waypoint WP11 = FP.WP(11);

    FlightPlan *alternativeFP = new FlightPlan(unixtime, WP0, WP1, WP2, WP3, WP4, WP5, WP6, WP7, WP8, WP9, WP10, WP11);

    for(int i=nxtWP_idx; i<8; i++)
    {
        double p = (0.0 + ((double)rand() / (RAND_MAX + 1)));

        if(p < 0.25 && (alternativeFP->WP(i).mandatory()))
        {
            //overwrite WP to non-mandatory and break
            Waypoint WPX = alternativeFP->WP(i).setMandatory(false);

            alternativeFP = &alternativeFP->setWP(i,WPX);

            break;
        }
    }

    return *alternativeFP;
}

void Server::readyREAD()
{
    QTcpSocket *client = (QTcpSocket*)sender();         // create a client
    QByteArray Data = client->readAll();                // read all received data from client
    qDebug() << client->socketDescriptor() << Data;     // write to CMD Window received data

    QString message = QString::fromUtf8(Data);          // create string message from data for later message decoding

    FlightPlan FP = message2FP(message);                        // convert received message to Flight Plan + nextWP index

    FlightPlan alternativeFP = findAlternativeFP(FP);           // find alternative Flight Plan

    QString alternativeFP_message = FP2Message(alternativeFP);  // convert alternative Flight Plan to string

    client->write(alternativeFP_message.toUtf8());              // send alternative Flight Plan
}

void Server::disconnected()
{
    QTcpSocket *client = (QTcpSocket*)sender();
    QString user = users[client];
    users.remove(client);

    qDebug() << client->socketDescriptor() << user <<" Client disconnected";

    clients.remove(client);
}

void Server::incomingConnection(int socketfd)
{
    QTcpSocket *client = new QTcpSocket();
    client->setSocketDescriptor(socketfd);
    qDebug() << client->socketDescriptor() << " Client connected";
    //clients.insert(client);

    connect(client, SIGNAL(readyRead()), this, SLOT(readyREAD()));
    connect(client, SIGNAL(disconnected()), this, SLOT(disconnected()));
}
