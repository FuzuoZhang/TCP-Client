#ifndef P2P_O2O_H
#define P2P_O2O_H
#include <QCloseEvent>
#include <QNetworkInterface>
#include <QIODevice>
#include <QMainWindow>
#include <QMessageBox>
#include <QTcpServer>
#include <QTcpSocket>
#include <QUdpSocket>
#include <QFileDialog>
#include <QTextCursor>
#include <QTextTable>
#include <QHostInfo>
#include <QTextStream>
#include <QDateTime>
#include <QFile>
#include <QDir>

#include "log.h"
namespace Ui {
class P2P_O2O;
}

class P2P_O2O : public QMainWindow
{
    Q_OBJECT

public:
    explicit P2P_O2O(int * label,/*QUdpSocket *UDP_Socket,*/ QString xh_ta,QString xh_me, QString ip_ta, QString ip_me,QString name, QWidget *parent = 0);
    ~P2P_O2O();

     void Init();
     void Send_Message();
     void Select_File();
     void Send_File();
     void CreateFileDir();
     QString time_now();  //返回当前时间;
     void closeEvent(QCloseEvent *event);
     private slots:
     void Receive_File();
     void readMessage();
     void on_pushButton_2_clicked(); //select file;
     void on_pushButton_fs_clicked();
     void Update_Send_Bar(qint64);
     void acceptConnection();
     void on_pushButton_clicked();

     void on_pushButton_3_clicked();

private:

   /****Basic infomation*****/
    QString ip_me;
    QString ip_ta;
    QString xh_me;
    QString xh_ta;
    QString name;
    int mark;  //mark=0 not send anything or receive anything
    int *label;
    /******UDP******/
    QUdpSocket *UDP_Socket;

    /******TCP******/
    QTcpServer * TCP_Server;
    QTcpSocket *TCP_Sender_Socket;
    QTcpSocket *TCP_Reader_Socket;
    /******Message******/
    QString tosend_message;

    /*****Files and Dirs*****/
    QDir * dir;
    QFile *file;
    /*****Send Files*****/
    QFile * toSendFile;   //文件；
    qint64 toSend_TotalBytes;  //发送文件的大小；
    qint64 Bytes_Written;  //已发送文件大小；
    qint64 Bytes_toWirte;  //剩余文件大小；
    qint64 MaxSize;  //
    QString FileName;   //绝对路径；
    QString filename; //文件名；
    QByteArray outBlock;  //数据缓存区，存储每次要发送的数据；

    /*********Receive Files**********/
    qint64 Bytes_Received;  //已接受的文件大小；
    qint64 toReceive_TotalBytes;
    qint64 FileNameSize; //文件名大小；
    QString FileName_R;
    QFile * ReceiveFile;
    QByteArray inBlock;  //接受数据缓存区；

    Ui::P2P_O2O *ui;

    void createdir();
    void createfile();//创建文本，存储聊天记录；
    void clearsendfile();
};

#endif // P2P_O2O_H
