/*
 * 프로그램명 : SubServer
 * 파일명 : subserver.h
 * 설명 : 영상 검사실용 미니 서버 (촬영SW와 Modality의 연결)
 * 작성자 : 안다미로
 * 최종 수정일 : 2023.01.06
 */

#ifndef SUBSERVER_H
#define SUBSERVER_H

#include <QWidget>
#include <QTcpServer>
#include <QTcpSocket>
#include <QMap>
#include <QFile>
#include <QFileInfo>
#include <QFileDialog>
#include <QProgressDialog>


#include "protocol.h"

#define SW 0
#define MODALITY 1

QT_BEGIN_NAMESPACE
namespace Ui { class SubServer; }
QT_END_NAMESPACE

class SubServer : public QWidget
{
    Q_OBJECT

public:
    explicit SubServer(QWidget *parent = nullptr);
    ~SubServer();

private slots:
    void newClient();
    void receiveControl();

    void newFileCilent();
    void receiveFile();
    void goOnSend(qint64 numBytes);
    void sendFile();

    void on_pushButton_clicked();

private:
    Ui::SubServer *ui;

    QTcpServer *controlServer;
    QTcpServer *fileServer;
    QMap<QTcpSocket*, int> controlSocketMap;    // <socket, SW or MODALITY>
    QMap<QTcpSocket*, int> fileSocketMap;       // <socket, SW or MODALITY>


    QFile* file;                                // File Object for FileSending Protocol
    qint64 totalSize;                           // Total size of File that clients are sending
    qint64 byteReceived = 0;                    // size of File read currently
    QByteArray inBlock;                         // Units divided to transfer files
    QString fileName;                           // Receiving FileName
    QString fileSender;                         // Receiving File's Sender
    QString checkFileName;                      // Previous File Name for checking new file
    qint64 loadSize;                // File Size
    qint64 byteToWrite;             // File Size per a block
    QByteArray outBlock;            // Block for sending


    Protocol *protocol;
};
#endif // SUBSERVER_H
