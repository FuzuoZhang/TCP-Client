#include "ql.h"
#include "ui_ql.h"

QL::QL(QString ip_me, QString xh_me,QWidget *parent) :
   ip_me(ip_me),xh_me(xh_me) ,QMainWindow(parent),
    ui(new Ui::QL)
{
    ui->setupUi(this);
    Init();
    connect(timer, SIGNAL(timeout()), this, SLOT(check())); // ***就是所说的响应函数
    timer->start(1000); // 每隔1s
    connect(UDP_Socket,SIGNAL(readyRead()),this,SLOT(readmessage()));

}

QL::~QL()
{
    delete ui;
}

void QL::closeEvent(QCloseEvent *event){
    UDP_Socket->disconnect();
    UDP_Socket->close();
    mySocket->disconnect();
    mySocket->close();
    event->accept();
}

void QL::Init(){
    name[0]="左左";
    name[1]="小红";
    name[2]="小明";
    xh[0]="2015011397";
    xh[1]="2015011415";
    xh[2]="2015011410";
    UDP_Socket= new QUdpSocket(this);  //Reader套接字
    UDP_Socket->bind(5555,QUdpSocket::ShareAddress);
    mySocket= new QTcpSocket(this);
    IP_Server="166.111.140.14";
    Port_Server=8000;
    mySocket->connectToHost(IP_Server,Port_Server);
    timer = new QTimer(this);

}

void QL::check(){
    int i;
    QString response;
    for(i=0;i<3;i++){
        QString tosend="q"+xh[i];
      //  if(xh[i]==xh_me) continue;
        QByteArray toSend=tosend.toLatin1().data();
        if(mySocket->isOpen())
        mySocket->write(toSend.data());
        if(mySocket->waitForReadyRead(30000))
        {
          response=mySocket->readAll();
        }
        ip[i]=response;
    }
    QPalette blue,gray;
    blue.setColor(QPalette::WindowText,Qt::blue);
    gray.setColor(QPalette::WindowText,Qt::red);

    if(ip[0].contains("n")){
        ui->zuozuo->setPalette(gray);
    }
    else  ui->zuozuo->setPalette(blue);

    if(ip[1].contains("n")){
        ui->xiaohong->setPalette(gray);
    }
    else ui->xiaohong->setPalette(blue);

    if(ip[2].contains("n")){
        ui->xiaoming->setPalette(gray);
    }
    else ui->xiaoming->setPalette(blue);
}

void QL::sendmessage(){
    int i;
    check();
    QString ip_ta;
    QString Sender_Text=ui->textEdit->toPlainText();
    QByteArray toSend=Sender_Text.toUtf8();

    for(i=0;i<3;i++){
        ip_ta=ip[i];
        if(xh[i]==xh_me ||ip_ta.contains("n") ) continue;
     qDebug()<<ip_ta;
    QHostAddress *tt= new QHostAddress(ip_ta);
    //UDP_Socket->writeDatagram(toSend,QHostAddress::Broadcast,5555);  //只用Reader
    UDP_Socket->writeDatagram(toSend,*tt,5555);
    }
    ui->textBrowser->setTextColor(QColor(0,0,0));
    ui->textBrowser->append(QStringLiteral("我:")+Sender_Text+"\n");
    ui->textEdit->clear();
 //   qDebug()<<"I'm sending message";

}

void QL::readmessage(){
    while(UDP_Socket->hasPendingDatagrams()){
    QHostAddress *IP_receive=new QHostAddress();
    QByteArray toRead;
    toRead.resize(UDP_Socket->pendingDatagramSize());
    UDP_Socket->readDatagram(toRead.data(),toRead.size(),IP_receive);
    qDebug()<<"Received from :"<< IP_receive->toString();
    int i;
    for(i=0;i<3;i++){
      //  if(/*ip[i]==ip_me && */xh[i]==xh_me) continue;
        if(IP_receive->toString().endsWith(ip[i]) && xh[i]!=xh_me){
            ui->textBrowser->setTextColor(QColor("blue"));
            ui->textBrowser->append(name[i]+":"+toRead+"\n");
            return;
        }
    }
}
}

void QL::on_pushButton_clicked()
{
    if(ui->textEdit->toPlainText().isEmpty()) return;
    sendmessage();
}
