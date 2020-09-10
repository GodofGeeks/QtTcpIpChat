#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#define DEFAULT "default"
#include <QMainWindow>
#include <QTcpSocket>
#include <QMessageBox>
#include <QDebug>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonParseError>
#include <QTableView>
#include <QStandardItem>
#include <QPushButton>
#include <QLineEdit>




QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    bool IsOpen(QTcpSocket *);
    void setsendSocketCondition(bool);

public slots:

    void sendSockReady();      //send Socket slots to bind
    void sendSockDisc();       //send Socket slots to bind

    void recieveSockReady();   //recieve Socket slots to bind
    void recieveSockDisc();    //recieve Socket slots to bind

private slots:

    void on_ConnectButton_clicked();
    void on_discButton_clicked();
    void on_SendBut_clicked();
    void ClearBut_clicked();

private:
    QTcpSocket *sendSocket;   //socket for SENDING packages to server
    QTcpSocket *recieveSocket;//socket for RECIEVING  packages from server

    const QByteArray IP_v4 = "127.0.0.1";//ip to connect to host
    const short PORT = 80;               //port to connect to host

    QJsonObject IpAndToken;             //json Object for conditions

    QString name;
    QString message;
    QString recievedMessage;
    QString recievedName;
    QString recievedTOKEN;
    QByteArray TOKEN ;
    QByteArray MateIp_v4;
    QByteArray SocketDescriptor;

    bool sendSocketIsOpen = false;

    Ui::MainWindow *ui;


};
#endif // MAINWINDOW_H
