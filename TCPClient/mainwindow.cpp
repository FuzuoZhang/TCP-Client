#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle(QStringLiteral("聊天吧！"));
    Init();
}


MainWindow::~MainWindow()
{
    delete ui;
    mySocket->disconnect();
    mySocket->close();
   // TCP_Server->close();
   // UDP_Socket->close();
   // TCP_Reader_Socket->close();
  //  TCP_Sender_Socket->close();
}

void MainWindow::Init(){
    ui->stackedWidget->setCurrentIndex(0);
    label=new int;
    *label=0;
    mySocket= new QTcpSocket(this);   //与服务器通信的套接字
    myStatus=0;   //offline
    IP_Server="166.111.140.14";
    Port_Server=8000;
    mySocket->connectToHost(IP_Server,Port_Server);  //build connection with server
    if (mySocket->waitForConnected(3000))
          qDebug("Connected to Server!");   //成功建立TCP连接
    else{
        qDebug("Connection Timed Out to Server!");
    }
    xh_ta="2015011415";
  // UDP_Socket= new QUdpSocket(this);  //Reader套接字
 //   UDP_Socket->bind(5566,QUdpSocket::ShareAddress);  //输入的信息从5566端口进*/
}

QString MainWindow::getIP()
{
    QString tosend="q"+myxh;
    QByteArray toSend=tosend.toLatin1().data();
    mySocket->write(toSend.data());
    QString response;
    if(mySocket->waitForReadyRead(30000))
    {
      response=mySocket->readAll();
    }
    return response;
}
void MainWindow::closeEvent(QCloseEvent *event){
    if(myStatus==1){
        on_pushButton_xx_clicked();
    }
    event->accept();
}



void MainWindow::on_pushButton_tc_clicked()
{
    if(myStatus==1)
        on_pushButton_xx_clicked();
    close();
}

void MainWindow::on_pushButton_dl_clicked()  //登录
{
   myxh=ui->xuehao->toPlainText();
  if(myStatus==0){
    QString tosend=myxh+"_net2017";
    QByteArray toSend=tosend.toLatin1().data();
    mySocket->write(toSend.data());
    QString response;
    if(mySocket->waitForReadyRead(3000))
    {
      response=mySocket->readAll();
    }
    qDebug()<<"登录: "<<response;
    if(response.contains("lol", Qt::CaseInsensitive))  {
          myStatus=1;
          QMessageBox msgBox;
          msgBox.setText("Successful Login!");
          msgBox.exec();
    myIP=getIP();
    setWindowTitle(QStringLiteral("欢迎你")+myxh);
    qDebug()<<"myIP: "+myIP;
    CreateFileDir();
    }
    else{
        QMessageBox msgBox;
        msgBox.setText("Login Failed!");
        msgBox.exec();
    }
  }
  else{
      QMessageBox msgBox;
      msgBox.setText("You are logged in!");
      msgBox.exec();
  }
}

void MainWindow::on_pushButton_xx_clicked()
{
    if(myStatus==1)
    {
    QString tosend="logout"+myxh;
    QByteArray toSend=tosend.toLatin1().data();
    mySocket->write(toSend.data());
    QString response;
    if(mySocket->waitForReadyRead(3000))
    {
      response=mySocket->readAll();
    }
    qDebug()<<"下线: "<<response;
    if(response.contains("loo", Qt::CaseInsensitive))  {
          myStatus=0;
          QMessageBox msgBox;
          msgBox.setText("Successful Logout!");
          msgBox.exec();
    }
    else{
        QMessageBox msgBox;
        msgBox.setText("Logout Failed!");
        msgBox.exec();
    }
    }
    else{
        QMessageBox msgBox;
        msgBox.setText("You are logged out!");
        msgBox.exec();
    }
}

 void MainWindow::CreateFileDir()
 {
        QDir *myDir= new QDir;
         bool exist = myDir->exists(myxh);
         if(exist)
            qDebug()<<"文件夹已经存在";
         else
         {
             bool ok = myDir->mkdir(myxh);
             if( ok )
             qDebug()<<"文件夹创建成功";
         }
 }

 void MainWindow::siliao(){
     IP_Friend_Now="";
     QString tosend="q"+xh_ta;
     QByteArray toSend=tosend.toLatin1().data();
     mySocket->write(toSend.data());
     QString response;
     if(mySocket->waitForReadyRead(30000))
     {
       response=mySocket->readAll();
     }
     qDebug()<<"ip_ta: "+response;
      IP_Friend_Now=response;

 }

void MainWindow::on_zuozuo_clicked()
{
    if (myStatus==0){
        QMessageBox msgBox;
        msgBox.setText(QStringLiteral("请先登录"));
        msgBox.exec();
        return ;}
      if((*label)==1){
        QMessageBox msgBox;
        msgBox.setWindowTitle(QStringLiteral("提示"));
        msgBox.setText(QStringLiteral("不能同时打开两个窗口"));
        msgBox.exec();
        return;
    }
      xh_ta="2015011397";
      siliao();
      if(IP_Friend_Now.contains("n", Qt::CaseInsensitive))  {  //not online
          QPalette   pal   = ui->zuozuo->palette();
          QColor gray(128,128,128);
          pal.setColor(QPalette::ButtonText,gray);
          ui->zuozuo ->setPalette(pal);
      }
      else if(!IP_Friend_Now.isEmpty()){
          QPalette   pal   = ui->zuozuo->palette();
          QColor blue("blue");
          pal.setColor(QPalette::ButtonText,blue);
          ui->zuozuo->setPalette(pal);
      }
       QString  Friend_Now=ui->zuozuo->text();
   //    ui->Friend_Now->setText(Friend_Now);

       P2P_O2O  *p2p=new P2P_O2O(label,/*UDP_Socket,*/"2015011397",myxh,IP_Friend_Now,myIP,Friend_Now);
       p2p->show();
       xh_ta="2015011415";

}


void MainWindow::on_alu_clicked()
{
    if (myStatus==0){
        QMessageBox msgBox;
        msgBox.setText(QStringLiteral("请先登录"));
        msgBox.exec();
        return ;}
    if((*label)==1){
      QMessageBox msgBox;
      msgBox.setWindowTitle(QStringLiteral("提示"));
      msgBox.setText(QStringLiteral("不能同时打开两个窗口"));
      msgBox.exec();
      return;
  }
    siliao();
    if(IP_Friend_Now.contains("n", Qt::CaseInsensitive))  {  //not online
        QPalette   pal   = ui->zuozuo->palette();
        QColor gray(128,128,128);
        pal.setColor(QPalette::ButtonText,gray);
        ui->alu ->setPalette(pal);
    }
    else if(!IP_Friend_Now.isEmpty()){
        QPalette   pal   = ui->alu->palette();
        QColor blue("blue");
        pal.setColor(QPalette::ButtonText,blue);
        ui->alu->setPalette(pal);
    }
     QString  Friend_Now=ui->alu->text();
 //    ui->Friend_Now->setText(Friend_Now);

     P2P_O2O  *p2p=new P2P_O2O(label,/*UDP_Socket,*/xh_ta,myxh,IP_Friend_Now,myIP,Friend_Now);
     p2p->show();
}

void MainWindow::qunliao(){
    QL *myql = new QL(myIP,myxh);
    qDebug()<<myIP;
    myql->show();
}


void MainWindow::on_pushButton_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
}

void MainWindow::on_pushButton_2_clicked()
{
    ui->stackedWidget->setCurrentIndex(1);
}

void MainWindow::on_toolButton_clicked()
{
    if (myStatus==0){
        QMessageBox msgBox;
        msgBox.setText(QStringLiteral("请先登录"));
        msgBox.exec();
        return ;}
    qunliao();
}

void MainWindow::on_pushButton_3_clicked()
{
    QString tosend="q2015011397";
    QByteArray toSend=tosend.toLatin1().data();
    mySocket->write(toSend.data());
    QString response;
    if(mySocket->waitForReadyRead(30000))
    {
      response=mySocket->readAll();
    }
    QString ip=response;
    if(ip.contains("n")) ip="";
    info * zuozuo=new info("左左","2015011397",ip);
    zuozuo->exec();
}

void MainWindow::on_pushButton_4_clicked()
{
    QString tosend="q2015011415";
    QByteArray toSend=tosend.toLatin1().data();
    mySocket->write(toSend.data());
    QString response;
    if(mySocket->waitForReadyRead(30000))
    {
      response=mySocket->readAll();
    }
    QString ip=response;
    if(ip.contains("n")) ip="";
    info * other=new info("other","2015011415",ip);
    other->exec();
}
