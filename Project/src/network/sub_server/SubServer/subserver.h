/*
 * 프로그램명 : SubServer
 * 파일명 : subserver.h
 * 설명 : 영상 검사실용 미니 서버 (촬영SW와 Modality의 연결)
 * 작성자 : 안다미로
 * 최종 수정일 : 2023.02.13
 */

#ifndef SUBSERVER_H
#define SUBSERVER_H

#include <QWidget>
#include <QFile>

class Protocol;
class QTcpServer;
class QTcpSocket;


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
    void newFileClient();
    void receiveControl();
    void firstFileSocket();
    void receiveFile();

private:
    Ui::SubServer *ui;

    QString currentPID = "NULL";
    QString currentType = "NULL";

    Protocol *protocol = nullptr;

    QTcpServer *controlServer = nullptr;
    QTcpServer *fileServer = nullptr;
    QMap<QTcpSocket*, int> controlSocketMap;    // <socket, SW = 0,  MODALITY = 1>
    QMap<QTcpSocket*, int> fileSocketMap;       // <socket, SW = 0,  MODALITY = 1>

    int count = 0;
    int countMax = 0;
    int frameSize = 0;
    QFile file;
    QString fileName;
    QByteArray receiveData;
};
#endif // SUBSERVER_H
