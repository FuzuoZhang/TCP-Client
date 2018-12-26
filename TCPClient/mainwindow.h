#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include "ql.h"
#include "info.h"
#include "p2p_o2o.h"
#include <QCloseEvent>
#include <QNetworkInterface>
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
#include <QFile>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    QString getIP();
    void closeEvent(QCloseEvent *event);
   /* void Send_Message();
    void Select_File();
    void Send_File();
    */
    void CreateFileDir();
    void siliao();
    void qunliao();
private slots:

    void on_pushButton_dl_clicked();

    void on_pushButton_xx_clicked();

    void on_zuozuo_clicked();

  /*  void on_pushButton_fs_clicked();

    void on_pushButton_2_clicked();
    void readMessage();
    void Update_Send_Bar(qint64);
    void acceptConnection();
    void Receive_File();
    */
    void on_alu_clicked();
    void on_pushButton_tc_clicked();

    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_toolButton_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_4_clicked();

private:
    Ui::MainWindow *ui;
    /*********个人信息***********/
    QTcpSocket *mySocket;
    QDir *myDir;  //我的文件夹
    QString myIP;
    QString myxh;
    QString xh_ta;
    QString IP_Friend_Now;  //当前所打开的好友的通信界面;其对应的IP
    QString friendname;
    /********Udp protocal to send or receive message*********/
    //QUdpSocket *UDP_Sender_Socket;  //聊天用UDP
  //   QUdpSocket *UDP_Socket;   //现在只用Reader，，后面可以直接删掉Sender
/*    QTcpSocket *TCP_Sender_Socket;   //发送文件用TCP
    QTcpSocket *TCP_Reader_Socket;
*/
    /******Tcp protocal to send or receive files*******/
  /*  QTcpServer * TCP_Server;*/
  //  QTcpSocket * TCP_Socket;

    /*****Send Files*****/
 /*   QFile * toSendFile;   //文件；
    qint64 toSend_TotalBytes;  //发送文件的大小；
    qint64 Bytes_Written;  //已发送文件大小；
    qint64 Bytes_toWirte;  //剩余文件大小；
    qint64 MaxSize;  //
    QString FileName;   //绝对路径；
    QString filename; //文件名；
    QByteArray outBlock;  //数据缓存区，存储每次要发送的数据；
*/
    /*********Receive Files**********/
 /*   qint64 Bytes_Received;  //已接受的文件大小；
    qint64 toReceive_TotalBytes;
    qint64 FileNameSize; //文件名大小；
    QString FileName_R;
    QFile * ReceiveFile;
    QByteArray inBlock;  //接受数据缓存区；
*/
    /**********Sever Information****************/
    QString IP_Server;
    int Port_Server;
    int  *label;
    int myStatus;  //0:logout;  1:login
    void Init();

};

#endif // MAINWINDOW_H
