#include "server.h"

///
/// \brief Server::Server
///
Server::Server(){//Server user-defined constructor

}
///
/// \brief Server::~Server
///
Server::~Server(){//Server default destructor

}
///
/// \brief Server::startServer
///
void Server::startServer(){//called when server starts working
    if (this->listen(QHostAddress::Any, Port)){
        qDebug()<< "LISTENING!!"<<'\n';
    }
    else qDebug()<< "NOt Listening"<<'\n';
}
///
/// \brief Server::BackUP
///
void Server::BackUP(){
    ofstream BackUp("C:\\Users\\Tyom\\Documents\\BDServer\\BackUp.txt");
    //system("cls");
    BackUp << showRecentClients().toStdString();

}
///
/// \brief Server::CLIENT::CLIENT
/// \param name
///
Server::CLIENT::CLIENT(QString name){
    ::userSerialNumber ++;

    selfSerialNumber = :: userSerialNumber;
    this->name = name;
}



////
/// \brief Server::checkConnections
/// \param Set
///
void Server::checkConnections( vector<CLIENT> &Set){
    if (!Set.size())return;
    for (size_t i = 0 ; i < Set.size();i++)
    {
        if (Set[i].recieveSocket->isValid()) Set[i].STATUS = true;
        else Set[i].STATUS = false;
    }
}
///
/// \brief Server::operator []
/// \param Token
/// \return
///
unsigned int Server::operator[](const unsigned int&Token){
    for (size_t i = 0 ; i < CLIENTSet.size(); i++){
        qDebug()<<CLIENTSet[i].TOKEN<<'\n';
        if (CLIENTSet[i].TOKEN.toUInt() == Token) return i;
    }

    qDebug()<<"Ne nashli"<<'\n';
}

////
/// \brief Server::sendToAll
/// \param message
///
void Server::sendToAll(const QByteArray &message){
    for (size_t i = 0 ; i < CLIENTSet.size(); i++){
        checkConnections(CLIENTSet);
        if (CLIENTSet[i].recieveSocket->isValid() && CLIENTSet[i].TOKEN == tmpToken){
            qDebug()<< CLIENTSet[i].TOKEN <<'\t'<<tmpToken;
            CLIENTSet[i].recieveSocket->open(QTcpSocket::ReadWrite);
        try {


                CLIENTSet[i].recieveSocket->write(message);

            //QTcpSocket *sendSock = this->Server::nextPendingConnection();
            //sendSock->flush();
            //sendSock->write(message);
            //sendSock->waitForBytesWritten(5);
            //sendSock->close();
           //throw EXEPTION;

        } catch (...) {
            qDebug()<< "Exception detected "<<'\n';
        }
        qDebug()<< "Sended to :"<<CLIENTSet[i].name<<'\n';
        }
    }
}
////
/// \brief Server::findClient
/// \param CLIENTSet
/// \param Token
/// \return
///
int Server::findClient( vector<CLIENT> &CLIENTSet, const QString &Token){
    for (int index = 0 ; index < CLIENTSet.size(); index++){
        if (CLIENTSet[index].TOKEN == QString::fromStdString(Token.toStdString())){
           this->index = index;
             return this->index;
        }

    }
}
///
/// \brief Server::CLIENT::~CLIENT
///
Server::CLIENT::~CLIENT(){

}
///
/// \brief Server::CLIENT::ShowConditions
/// \return
///
QString Server::CLIENT::ShowConditions(){//defining show condition of current socket

    return +"|Client N  :"    +QString::number(this->selfSerialNumber)
           +"   |STATUS :"    +(this->STATUS == false ? "NOT CONNECTED" : "CONNECTED")
           +"   |IP_v4  :"    +(this->IP_v4.toString())
           +"   |PERMISSION :"+(this->Permission);

}
///
/// \brief Server::showRecentClients
/// \return
///
QString Server::showRecentClients(){//show  conditions of recent connected clients
    QString MAINSTRING;
    qDebug()<< "\n\n_______RECENT_____CLIENTS_____";
    for (int i = 0 ; i < CLIENTSet.size(); ++i){
        qDebug() << CLIENTSet[i].ShowConditions();
        MAINSTRING +=( CLIENTSet[i].ShowConditions() + '\n');
    }
      qDebug()<< "_______RECENT_____CLIENTS_____\n\n";
      return MAINSTRING;
}
///
/// \brief Server::incomingConnection
/// \param socketDescriptorforSend
///
void Server::incomingConnection(int socketDescriptorforSend){                                             //called when incoming signal from 5555 port detected

    CLIENT *newCLIENT = new CLIENT();                                                              //create new client
    newCLIENT->selfSerialNumber = userSerialNumber;                                                //set new client serial number

    newCLIENT->recieveSocket = new QTcpSocket(this);                                               //create new recieveSocket to recieve packages from clients
    newCLIENT->sendSocket = new QTcpSocket(this);                                                  //create new socket to send packages to clients
    //checkConnections(CLIENTSet);
    CLIENTSet.push_back(*newCLIENT);                                                               //add new client to recent clients set   <CLIENTSet>
    newCLIENT->recieveSocket->setSocketDescriptor(socketDescriptorforSend);                               //.........................................
   // newCLIENT->sendSocket->setSocketDescriptor(++socketDescriptorforSend);

    connect(newCLIENT->recieveSocket, SIGNAL(readyRead()),this,SLOT(recieveSockReady()));                 //bind function readyRead() with sockReady()
    connect(newCLIENT->recieveSocket,SIGNAL(disconnected()),this, SLOT(recieveSockDisc()));               //bind funtcion disconnected() with sockDisc()

    connect(newCLIENT->sendSocket, SIGNAL(readyRead()), this, SLOT(sendSockReady()));
    connect(newCLIENT->sendSocket, SIGNAL(disconnected()), this, SLOT(sendSockDisc()));
    newCLIENT->STATUS = true;
///
    //CLIENTSet[CLIENTSet.size()-1].STATUS =  CLIENTSet[CLIENTSet.size()-1].recieveSocket->isOpen();
////
    if ( CLIENTSet[CLIENTSet.size()-1].recieveSocket->isOpen())  qDebug() << "New Connection ... ";

}
///
/// \brief Server::recieveSockReady
///
void Server::recieveSockReady(){//called when sockt is ready to send/recieve Bytes                                                  /////STE POXELU BAN KA CONFIG@////
    if (CLIENTSet[CLIENTSet.size()-1].FirstTime){
    CLIENTSet[CLIENTSet.size()-1].CONFIG = CLIENTSet[CLIENTSet.size()-1].recieveSocket->readAll();//get CONFIG from client
    CLIENTSet[CLIENTSet.size()-1].IP_v4 = CLIENTSet[CLIENTSet.size()-1].recieveSocket->peerAddress();//get IP_v4 address from client
    ParseFirstData(CLIENTSet[CLIENTSet.size()-1].CONFIG);
    if (CLIENTSet[CLIENTSet.size()-1].CONFIG.isEmpty()) {
        CLIENTSet[CLIENTSet.size()-1].CONFIG = DEFAULT;
        qDebug() << "CONFIG IS EMPTY\n";
    }
    if (CLIENTSet[CLIENTSet.size()-1].TOKEN == this->rootTOKEN)
          CLIENTSet[CLIENTSet.size()-1].Permission = Zero;//if token ,sended from client , is equal to rootTOKEN , give client ROOT (Zero) Permission

    else if (CLIENTSet[CLIENTSet.size()-1].TOKEN == this->userTOKEN)//if token ,sended from client , is equal to userTOKEN , give client USER (One) Permission
          CLIENTSet[CLIENTSet.size()-1].Permission = One;

    else CLIENTSet[CLIENTSet.size()-1].Permission = Two;//if given token is not equal given above tokens give client guest permission
    CLIENTSet[CLIENTSet.size()-1].FirstTime = false;
}
    else {

    for (int i = 0 ; i < CLIENTSet.size(); i++)        ////
      if (CLIENTSet[i].recieveSocket->isReadable())
        ParseSecondData(CLIENTSet[i].recieveSocket->readAll());
    }

}
////
/// \brief Server::sendSockReady
///
void Server::sendSockReady(){
    qDebug()<< "MEssage Recieved to server"<<'\n';
}
///
/// \brief Server::sendSockDisc
///
void Server::sendSockDisc(){

    CLIENTSet[index].sendSocket->close();//close recieveSocket to ac/////////////////////////////////////////////
}


///
/// \brief Server::recieveSockDisc
///
void Server::recieveSockDisc()
{
    //checkConnections(CLIENTSet);
    BackUP();
}
///
/// \brief Server::ParseFirstData
/// \param jsonByte
///
void Server::ParseFirstData(const QByteArray &jsonByte){
   QJsonDocument jsonDoc = QJsonDocument::fromJson(jsonByte);
   QJsonObject jsonObj = jsonDoc.object();
   QVariantMap jsonMap = jsonObj.toVariantMap();



   this->CLIENTSet[CLIENTSet.size()-1].MateIp = jsonMap["mateip"].toString();
   this->CLIENTSet[CLIENTSet.size()-1].TOKEN  = jsonMap["token"].toString();
   this->CLIENTSet[CLIENTSet.size()-1].name  = jsonMap["name"].toString();

}
///
/// \brief Server::ParseSecondData
/// \param messageByte
///
void Server::ParseSecondData(const QByteArray &messageByte){
    if (messageByte.size() == 0){
        return;
    }
    QJsonDocument jsonDoc = QJsonDocument::fromJson(messageByte);
    QJsonObject jsonObj = jsonDoc.object();
    QVariantMap jsonMap = jsonObj.toVariantMap();

    this->tmpMessage = jsonMap["message"].toString();
    this->tmpToken  = jsonMap["token"].toString();
    this->tmpName = jsonMap["name"].toString();
    index = findClient(this->CLIENTSet,this->tmpToken);

    CLIENTSet[index].TOKEN = tmpToken;
    CLIENTSet[index].Message = tmpMessage;
    CLIENTSet[index].name = tmpName;

     qDebug()<< "Message recieved :" << CLIENTSet[index].Message;   CLIENTSet[index].getData();

     sendToAll(messageByte);

     this->tmpToken.clear() ; this->tmpMessage.clear();

}

///
/// \brief Server::CLIENT::getData
///
void Server::CLIENT::getData(){
    qDebug()<<"From : "<<this->IP_v4.toString()
           <<"| To : "<< this->MateIp
          << "| Token : "<<this->TOKEN
          <<" | Name : "<< this->name
          <<" | Premission : "<<this->Permission
          <<'\n';
}






















