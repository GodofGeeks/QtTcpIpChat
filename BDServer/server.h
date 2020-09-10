#ifndef SERVER_H
#define SERVER_H
#define DEFAULT "default"
#define EXEPTION  1
#include <QTcpServer>
#include <QTcpSocket>
#include <QDebug>
#include <QHostAddress>
#include <QJsonDocument>
#include <QJsonArray>
#include<QJsonObject>
#include <vector>
#include <string>
#include <conio.h>
#include <fstream>
#include <iostream>
using namespace std;
constexpr auto Zero = "ROOT",
                One =  "USER",
                  Two = "GUEST";
constexpr short int Port = 80;

static unsigned int userSerialNumber = 0;
class Server: public QTcpServer{
    Q_OBJECT
public:
    Server();
    ~Server();
    struct CLIENT{//client methods and fields
        CLIENT(QString name = QString());
        ~CLIENT();
        void getData();
        QString name;
        unsigned int selfSerialNumber ;

        QTcpSocket *recieveSocket;
        QTcpSocket *sendSocket;
        bool STATUS = false;
        bool FirstTime = true;
        bool toDisc =false;
        QHostAddress IP_v4 ;
        QString MateIp;
        QString TOKEN;
        QByteArray CONFIG;
        int SocketDescriptor;
        QString Permission;
        QString Message;
        QString ShowConditions();

    };

    QString showRecentClients();
    void BackUP();
    void ParseFirstData(const QByteArray&);
    void ParseSecondData(const QByteArray&);
    int findClient( vector<CLIENT>&, const QString &Token);
    void sendToAll(const QByteArray &);
    unsigned int operator [](const unsigned int&);
    void checkConnections( vector<CLIENT>&);
public slots:
    void startServer();
    void incomingConnection(int socketDescriptorforSend);
    void recieveSockReady();
    void recieveSockDisc();
    void sendSockReady();
    void sendSockDisc();

private:
    vector <CLIENT> CLIENTSet;
     int index;
     int selfIndex ;

    QString tmpMessage,
            tmpToken,
            tmpName;
    const QByteArray rootTOKEN = "1234",
                     userTOKEN = "abcd";
};
#endif // SERVER_H
