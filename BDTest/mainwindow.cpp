#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QJsonArray>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->discButton->setEnabled(0);

    sendSocket = new QTcpSocket(this);
    recieveSocket = new QTcpSocket(this);

    connect(sendSocket, SIGNAL(readyRead()),this ,SLOT(sendSockReady()));   //bind readyRead() to sockReady()    (sendSocket)
    connect(sendSocket, SIGNAL(disconnected()),this ,SLOT(sendSockDisc())); //bind disconnected() to sockDisc()  (sendSocket)

    connect(recieveSocket, SIGNAL(readyRead()), this, SLOT(recieveSockReady()));
    connect(recieveSocket, SIGNAL(disconnected()), this, SLOT(recieveSockDisc()));

}
bool MainWindow::IsOpen(QTcpSocket *socket){
    return socket->isOpen();
}

void MainWindow::setsendSocketCondition(bool condition){
    sendSocketIsOpen = condition;
}

MainWindow::~MainWindow()
{
    delete ui;
}



void MainWindow::on_ConnectButton_clicked()
{


  if (ui->MateIpEdit->text().size() && ui->NameEdit->text().size()){
    name =  ui->NameEdit->text();
    ui->ConnectButton->setEnabled(0);                                                     //switch off connect button to avoid any new connections
    ui->discButton->setEnabled(1);                                                        //switch on disconnect button
    sendSocket = new QTcpSocket(this);                                                          //create a new sendSocket for current client
    recieveSocket = new QTcpSocket(this);
    ui->NameEdit->setEnabled(0);
    ui->MateIpEdit->setEnabled(0);
    ui->TokenEdit->setEnabled(0);
    sendSocket->connectToHost(IP_v4, PORT);                                               //connect current client to server by <IP_v4> & <PORT>
   // recieveSocket->connectToHost(IP_v4, PORT);

    if (sendSocket->isWritable() ){

        this->TOKEN =(QByteArray::fromStdString(ui->TokenEdit->text().toStdString()).size() == 0 ? DEFAULT :
                QByteArray::fromStdString(ui->TokenEdit->text().toStdString()));
        // ^
        // | if token is empty set token as <default>

        this->MateIp_v4 = QByteArray::fromStdString(ui->MateIpEdit->text().toStdString());//get Ip to connect with mate later in server

        IpAndToken["token"] = QString::fromStdString(this->TOKEN.toStdString());              //factoring json format
        IpAndToken["mateip"] = QString::fromStdString(this->MateIp_v4.toStdString());         //factoring json format
        IpAndToken["name"] = name;
        IpAndToken["sockdesc"] = sendSocket->socketDescriptor();
        ;


        QJsonDocument *doc = new QJsonDocument(IpAndToken);                                   //make json object to Json Document
        QByteArray docByteArray = doc->toJson(QJsonDocument::Compact);                        //make json document to QBYte array to send to server later
        if (sendSocket->isWritable())
        sendSocket->write(docByteArray);                                                       //send to server client condition in json formate
        else
            QMessageBox::critical(this, "Critical" , "Server not responding");

    }

}

  else {
      QMessageBox::critical(this, "Error", "Enter the Mate Ip Address");
     }

}
void MainWindow::on_discButton_clicked()
{
    ui->discButton->setEnabled(0);          //switch off disconnect button to avoid any new disconnections
    ui->ConnectButton->setEnabled(1);       //swtch on connect button
    ui->NameEdit->setEnabled(1);
    ui->MateIpEdit->setEnabled(1);
    ui->TokenEdit->setEnabled(1);
    sendSockDisc();                             //disconnect socket from Host
    ui->ChatBox->clear();
}

void MainWindow::sendSockDisc(){


    sendSocket->waitForDisconnected(50);
    sendSocket->deleteLater();
}

void MainWindow::recieveSockDisc(){
    recieveSocket->deleteLater();
}

void MainWindow::sendSockReady(){

}

void MainWindow::recieveSockReady(){



}


void MainWindow::on_SendBut_clicked()
{
    recieveSocket->open(QTcpSocket::ReadOnly);
    message = ui->textEdit->toPlainText();
    QJsonObject messageObj;

    messageObj["message"] = message;
    messageObj["token"] = QString::fromStdString(this->TOKEN.toStdString());
    messageObj["name"] = this->name;
    QJsonDocument *messageDoc = new QJsonDocument(messageObj);
    QByteArray messageByteArray = messageDoc->toJson(QJsonDocument::Compact);
    qDebug() << messageByteArray<<'\n';
    if (ui->discButton->isEnabled())
        if (!ui->textEdit->toPlainText().size()){

            QMessageBox::warning(this, "Warning!" , "we don't send empty messages");
        }
        else{

        sendSocket->write(messageByteArray);
        ui->textEdit->clear();

        }
    else
        QMessageBox::critical(this, "rr" ,"Not Connected to Server");

    sendSocket->waitForReadyRead(0);

    QByteArray recievedMessage = sendSocket->readAll();
    QJsonDocument recievedMessageDoc = QJsonDocument::fromJson(recievedMessage);
    QJsonObject   recievedMessageObj = recievedMessageDoc.object();
    QVariantMap   recievedMessageMap = recievedMessageObj.toVariantMap();
    this->recievedName = recievedMessageMap["name"].toString();
    this->recievedTOKEN = recievedMessageMap["token"].toString();
    this->recievedMessage = recievedMessageMap["message"].toString();
    ui->ChatBox->insertPlainText(recievedName+":"+recievedMessage + '\n');


}
void MainWindow::ClearBut_clicked()
{
    if (ui->ChatBox->toPlainText().size())
    ui->ChatBox->clear();
    else QMessageBox::warning(this, "Warning", "Nothing to clean");
}
