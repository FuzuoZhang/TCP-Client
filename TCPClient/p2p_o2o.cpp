#include "p2p_o2o.h"
#include "ui_p2p_o2o.h"

P2P_O2O::P2P_O2O(/*QUdpSocket *UDP_Socket,*/int *label, QString xh_ta,QString xh_me, QString ip_ta, QString ip_me,QString name,QWidget *parent) :
/*UDP_Socket(UDP_Socket), */ label(label),xh_ta(xh_ta),xh_me(xh_me),ip_ta(ip_ta),ip_me(ip_me),name(name),QMainWindow(parent),
     ui(new Ui::P2P_O2O)

{
    ui->setupUi(this);
    ui->progressBar->setVisible(false);
    ui->progressBar->setValue(0);
    Init();
    qDebug()<<xh_ta<<"\n"<<xh_me<<"\n"<<ip_ta<<"\n"<<ip_me<<"\n"<<name;

    /*********槽函数*********/
    connect(TCP_Sender_Socket, SIGNAL(bytesWritten(qint64)), this, SLOT(Update_Send_Bar(qint64)));
    connect(UDP_Socket, SIGNAL(readyRead()), this, SLOT(readMessage()));
    connect(TCP_Server, SIGNAL(newConnection()), this, SLOT(acceptConnection()));
}

P2P_O2O::~P2P_O2O()
{
    delete ui;
    delete TCP_Sender_Socket;
    delete TCP_Server;
    delete UDP_Socket;
}

void P2P_O2O::closeEvent(QCloseEvent *event){
       UDP_Socket->disconnect();
       UDP_Socket->close();
       TCP_Sender_Socket->close();
       TCP_Sender_Socket->disconnect();
       TCP_Server->close();
       qDebug()<<"close";
       *label=0;
       event->accept();
}
void P2P_O2O::Init(){
    ui->progressBar->setVisible(false);
    Bytes_toWirte=0;
    Bytes_Written=0;
    toSend_TotalBytes=0;
    FileName="";
    filename="";

    toReceive_TotalBytes=0;
    Bytes_Received=0;  //已接受的文件大小；
    FileName_R="";
    FileNameSize=0; //文件名大小；
    inBlock=0;

    mark=0;
    createdir();
    createfile();
    /*****参数******/
    setWindowTitle(name);

    UDP_Socket= new QUdpSocket(this);  //Reader套接字
    UDP_Socket->bind(5566,QUdpSocket::ShareAddress);  //输入的信息从5566端口进
     *label=1;
    TCP_Sender_Socket= new QTcpSocket(this);  //Sender套接字
    TCP_Server=new QTcpServer(this);
    QHostAddress *localHostAddr = new QHostAddress(ip_me);
    qDebug()<<localHostAddr->toString();
    TCP_Server->listen(*localHostAddr,5555);  //监听5555端口
    qDebug()<<"I'm listening: "<<TCP_Server->serverAddress()<<TCP_Server->serverPort();
}

void P2P_O2O::createdir(){   //在主账号文件加下创建子文件夹 保存聊天内容及收到的文件；
    dir= new QDir;
    QString newpath=xh_me+QString("/");
    dir->cd(newpath);
     bool exist = dir->exists(xh_ta);
     if(exist)
        qDebug()<<"文件夹存在";
     else
     {
         bool ok = dir->mkdir(xh_ta);
         if( ok )
            qDebug()<<"文件夹创建成功";
     }
     newpath=xh_ta+QString("/");
     dir->cd(newpath);

}

 void P2P_O2O::createfile()
 {
  QString temp = dir->path()+QString("/log.txt");
  file = new QFile(temp);
  file->open(QFile::ReadWrite);
  file->close();
  file->fileName();
   qDebug()<<dir->path();
  return ;
 }

  QString P2P_O2O::time_now()  //返回当前时间；
  {
      QDateTime time = QDateTime::currentDateTime();  //获取系统现在的时间
    //  QString str = time.toString("hh:mm:ss ddd");     //设置显示格式
      QString str = time.toString("yyyy-MM-dd HH:mm:ss");     //设置显示格式
      return str;
  }

 void P2P_O2O::Send_Message(){
     QString time="\n"+time_now();
     QString Sender_Text=ui->textEdit->toPlainText();
     qDebug()<<"I'm sending message";
     QByteArray toSend=Sender_Text.toUtf8();
     QHostAddress *tt= new QHostAddress(ip_ta);
     //UDP_Socket->writeDatagram(toSend,QHostAddress::Broadcast,5566);  //只用Reader
     UDP_Socket->writeDatagram(toSend,*tt,5566);
     if(mark==0){
     ui->textBrowser->setTextColor(QColor(128,128,128));
     ui->textBrowser->append(time+"\n");
     }
     ui->textBrowser->setTextColor(QColor(0,0,0));
     ui->textBrowser->append(QStringLiteral("我:")+Sender_Text+"\n");
    QFile temp(file->fileName());
    if(temp.open(QIODevice::WriteOnly | QIODevice::Text|QIODevice::Append))
    {
        QTextStream in(&temp);
        in.setCodec("utf8");
        if(mark==0)  in<<time<<"\n";
       in<<QStringLiteral("我:")<<Sender_Text<<"\n"<<"\n";
    }
    else {
        qDebug()<<"打不开文件";
    }
    temp.close();
    ui->textEdit->clear();
    mark=1;
 }

 void P2P_O2O::readMessage(){
     QString time="\n"+time_now();
     while(UDP_Socket->hasPendingDatagrams()){
     QHostAddress *IP_receive=new QHostAddress();
     QByteArray toRead;
     toRead.resize(UDP_Socket->pendingDatagramSize());
     UDP_Socket->readDatagram(toRead.data(),toRead.size(),IP_receive);
     qDebug()<<"Received from :"<< IP_receive->toString();
     if(IP_receive->toString().endsWith(ip_ta))
     {
      if(mark==0){
      ui->textBrowser->setTextColor(QColor(128,128,128));
      ui->textBrowser->append(time+"\n");
      }
      qDebug()<<"here";
     ui->textBrowser->setTextColor(QColor("blue"));
     ui->textBrowser->append(name+":"+toRead+"\n");
     QFile temp(file->fileName());
     if(temp.open(QIODevice::WriteOnly | QIODevice::Text|QIODevice::Append))
     {
         QTextStream in(&temp);
         in.setCodec("utf8");
         if(mark==0)  in<<time<<"\n";
        in<<name+":"<<toRead<<"\n"<<"\n";
     }
     else  qDebug()<<"打不开文件";
     temp.close();
     }
     }
 }

void P2P_O2O::Select_File(){
    FileName = QFileDialog::getOpenFileName(this);
    if(FileName.isEmpty()){
      return;
    }
    else{
       if(ip_ta.contains("n"))
       {
           QMessageBox::warning(this,tr("提示"),QString("对方不在线"),QMessageBox::Ok);
           return ;
       }
        //把文件名放在发送框内
       QFileInfo fi(FileName);
       filename=fi.fileName();
       QTextCursor cursor(ui->textEdit->textCursor());
       cursor.movePosition(QTextCursor::Start);
       QTextTable *table1 =cursor.insertTable(1,1);
       table1->cellAt(0,0).firstCursorPosition().insertHtml(fi.fileName());
       ui->textEdit->setTextCursor(cursor);
       Send_File();
    }
}

void P2P_O2O::Send_File(){
    TCP_Sender_Socket->connectToHost(ip_ta,5555);
    if (!TCP_Sender_Socket->waitForConnected(3000)){
        QMessageBox msgBox;
        msgBox.setWindowTitle(QStringLiteral("提示"));
        msgBox.setText(QStringLiteral("请求超时"));
        ui->textEdit->clear();
        msgBox.exec();
        return;
    }
    qDebug("Connected!");   //成功建立TCP连接
    ui->progressBar->setVisible(true);
    toSendFile = new QFile(FileName);
    if(!toSendFile->open(QFile::ReadOnly))
    {
        qDebug()<<"open file fialed!";
        return ;
    }
    toSend_TotalBytes=toSendFile->size();
    QDataStream sendOut(&outBlock,QIODevice::WriteOnly);
    sendOut.setVersion(QDataStream::Qt_DefaultCompiledVersion);//
    //写入文件信息
    sendOut<<qint64(0)<<qint64(0)<<filename;
    toSend_TotalBytes+=outBlock.size();

    sendOut.device()->seek(0);
    sendOut<<toSend_TotalBytes<<qint64((outBlock.size()-sizeof(qint64)*2));
    Bytes_toWirte=toSend_TotalBytes-TCP_Sender_Socket->write(outBlock);
    /* if (!TCP_Sender_Socket->waitForBytesWritten(300000)){
         QMessageBox msgBox;
         msgBox.setWindowTitle(QStringLiteral("提示"));
         msgBox.setText(QStringLiteral("发送失败"));
         ui->textEdit->clear();
         msgBox.exec();
         TCP_Sender_Socket->close();
         ui->progressBar->setVisible(false);
         clearsendfile();
         return;
     }*/
    outBlock.resize(0);
    qDebug()<<"sendfile"<<toSendFile;
}
void P2P_O2O::clearsendfile(){
    Bytes_toWirte=0;
    Bytes_Written=0;
    toSend_TotalBytes=0;
    FileName="";
    filename="";
}

void P2P_O2O::Update_Send_Bar(qint64 num){
    Bytes_Written+=(int)num;
    if(Bytes_toWirte>0)
    {
        outBlock=toSendFile->read(qMin(Bytes_toWirte,MaxSize));
        Bytes_toWirte-=(int)TCP_Sender_Socket->write(outBlock);
       /* if (!TCP_Sender_Socket->waitForBytesWritten(300000)){
            QMessageBox msgBox;
            msgBox.setWindowTitle(QStringLiteral("提示"));
            msgBox.setText(QStringLiteral("发送失败"));
            ui->textEdit->clear();
            msgBox.exec();
            TCP_Sender_Socket->close();
            ui->progressBar->setVisible(false);
            clearsendfile();
            return;
        }*/
        outBlock.resize(0);
    }
    else{
        toSendFile->close();
    }

    //更新数据条
    ui->progressBar->setMaximum(toSend_TotalBytes);
    ui->progressBar->setValue(Bytes_Written);
    qDebug()<<"written"<<Bytes_Written;
    if(Bytes_Written==toSend_TotalBytes){  //发送完毕
        QString time="\n"+time_now();
        if(mark==0){
        ui->textBrowser->setTextColor(QColor(128,128,128));
        ui->textBrowser->append(time+"\n");
        }
        toSendFile->close();
        ui->progressBar->setVisible(false);
        ui->textBrowser->setTextColor(QColor(0,0,0));
        ui->textBrowser->append(QStringLiteral("我向")+name+QStringLiteral("发送文件：")+filename+"\n");
        ui->textEdit->clear();
        TCP_Sender_Socket->close();
        QFile temp(file->fileName());
        if(temp.open(QIODevice::WriteOnly | QIODevice::Text|QIODevice::Append))
        {
            QTextStream in(&temp);
            in.setCodec("utf8");
            if(mark==0)  in<<time+"\n";
           in<<QStringLiteral("我向")<<name<<QStringLiteral("发送文件：")<<filename<<"\n"<<"\n";
        }
        else {
            qDebug()<<"打不开文件";
        }
        temp.close();
        Bytes_toWirte=0;
        Bytes_Written=0;
        toSend_TotalBytes=0;
        FileName="";
        filename="";
        mark=1; //mark time
    }

}

void P2P_O2O::acceptConnection(){  //接受tcp连接请求
     TCP_Reader_Socket = TCP_Server->nextPendingConnection();
     connect(TCP_Reader_Socket, SIGNAL(readyRead()), this,SLOT(Receive_File()));
    // TCP_Server->close();
}

void P2P_O2O::Receive_File(){
       QDataStream in(TCP_Reader_Socket);
       in.setVersion(QDataStream::Qt_DefaultCompiledVersion);
       //如果接收到的数据小于等于16个字节，那么是刚开始接收数据，我们保存为头文件信息
       if(Bytes_Received <= sizeof(qint64)*2)
       {
           //接收数据总大小信息和文件名大小信息
           if((TCP_Reader_Socket->bytesAvailable() >= sizeof(qint64)*2) && (FileNameSize == 0))
           {
               in >>toReceive_TotalBytes >> FileNameSize;
               Bytes_Received += sizeof(qint64) * 2;
           }

           //接收文件名，并建立文件
           if((TCP_Reader_Socket->bytesAvailable() >= FileNameSize) && (FileNameSize != 0))
           {
               in >> FileName_R;
               Bytes_Received += FileNameSize;

              ReceiveFile = new QFile(dir->path()+QString("/")+FileName_R);
               if(!ReceiveFile->open(QFile::WriteOnly))
               {
                   qDebug() << "writing file error!";
                   return;
               }
           }
           else
               return;

       }

       //如果接收的数据小于总数据，那么写入文件
       if(Bytes_Received < toReceive_TotalBytes)
       {
           Bytes_Received += TCP_Reader_Socket->bytesAvailable();
           inBlock = TCP_Reader_Socket->readAll();
           ReceiveFile->write(inBlock);
           inBlock.resize(0);
       }
      qDebug()<<"receive files";
       //接收数据完成时
       if(Bytes_Received == toReceive_TotalBytes)
       {
           QString time="\n"+time_now();
           if(mark==0){
           ui->textBrowser->setTextColor(QColor(128,128,128));
           ui->textBrowser->append(time+"\n");
           }
           qDebug()<<"完成";
           if(mark==0){
           ui->textBrowser->setTextColor(QColor(128,128,128));
           ui->textBrowser->append(time+"\n");
           }
          ui->textBrowser->setTextColor(QColor("blue"));
          ui->textBrowser->append(QStringLiteral("我收到来自")+name+QStringLiteral("的文件：")+FileName_R+"\n");
          QFile temp(file->fileName());
          if(temp.open(QIODevice::WriteOnly | QIODevice::Text|QIODevice::Append))
          {
              QTextStream in(&temp);
              in.setCodec("utf8");
              if(mark==0) in<<time<<"\n";
             in<<QStringLiteral("我收到来自")<<name<<QStringLiteral("的文件：")<<FileName_R<<"\n"<<"\n";
          }
          else {
              qDebug()<<"打不开文件";
          }
          temp.close();
       //   TCP_Reader_Socket->close();
          ReceiveFile->close();
          Bytes_Received = 0; // clear for next receive
          toReceive_TotalBytes = 0;
          FileNameSize = 0;
          FileName_R="";
          TCP_Reader_Socket->disconnect();
          TCP_Reader_Socket->close();
          delete TCP_Reader_Socket;
       }
}

void P2P_O2O::on_pushButton_2_clicked()  //select file;
{
    if(FileName.isEmpty())  Select_File();
    else {
        QMessageBox msgBox;
        msgBox.setWindowTitle(QStringLiteral("提示"));
        msgBox.setText(QStringLiteral("只能选择一个文件"));
        msgBox.exec();
    }
}

void P2P_O2O::on_pushButton_fs_clicked()
{
    if(ip_ta.contains("n")) {
        QMessageBox::warning(this,tr("提示"),QString("对方不在线"),QMessageBox::Ok);
        return ;
    }
   QString text_edit=ui->textEdit->toPlainText();
   if(text_edit.isEmpty()){
       return ;
   }
   else{
     if(!filename.isEmpty() && text_edit.contains(filename))  //包含文件名，文件名非空
     {
         Send_File();
   }
     else Send_Message();
   }
}

void P2P_O2O::on_pushButton_clicked()   //聊天记录
{
     Log *log=new Log(name,file);
     log->exec();
}

void P2P_O2O::on_pushButton_3_clicked()
{

}
