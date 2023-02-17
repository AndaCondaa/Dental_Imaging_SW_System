/*
 * 프로그램명 : MainServer
 * 파일명 : mainserver.h
 * 설명 : PMS SW, 이미지 뷰어 SW, 촬영 SW와 연결되어 데이터를 주고받는 서버
 * 작성자 : 김유선
 * 최종 수정일 : 2023.02.16
 */

#ifndef MAINSERVER_H
#define MAINSERVER_H

#include <QMainWindow>
#include <QtCore>
#include <QtNetwork>
#include <QString>
#include <QSqlTableModel>
#include <QTableWidget>

#include <QtTest/QtTest>
#include <QTest>

class QStandardItemModel;

namespace Ui {
class MainServer;
}

class MainServer : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainServer(QWidget *parent = nullptr);
    ~MainServer();
    void loadData();

private slots:
    void newConnection();
    void newFileConnection();
    void socketDisconnected();
    void receiveData();
    bool writeData(QByteArray data);
    void receiveFile();
    void sendFile();
    void on_quitPushButton_clicked();
    void fileSocketDisconnected();

private:
    Ui::MainServer *ui;
    QTcpServer *server;
    QTcpServer *fileServer;

    QString makeId();
    QString makeReportNo();
    QString makeImageNo();

    QSqlQuery *query;
    QSqlQuery *query2;
    QSqlQuery *query3;
    QSqlQuery *query4;

    QTcpSocket *socket;

    QTcpSocket *pmsSocket = nullptr;
    QTcpSocket *imagingSocket = nullptr;
    QTcpSocket *viewerSocket = nullptr;

    QTcpSocket *pmsFileSocket = nullptr;
    QTcpSocket *imagingFileSocket = nullptr;
    QTcpSocket *viewerFileSocket = nullptr;

    QString saveData;

    QSqlTableModel *patientModel;
    QSqlTableModel *dentistModel;
    QSqlTableModel *imageModel;
    QSqlTableModel *reportModel;

    QMap<QTcpSocket *, QString> sk; //socket

    QString currentPID = "NULL";

    qint64 totalSize;
    qint64 byteReceived = 0;
    QFile* file;
    QByteArray inBlock;
    QString fileName;               // Receiving FileName
    qint64 byteToWrite;             // File Size per a block
    QByteArray outBlock;            // Block for sending
    qint64 loadSize;                // File Size

    QString currentFileName;
    QString type;

    int sendFileFlag = 0; //0이면 pms로, 1이면 viewer로

    void sendWaitingList(QTcpSocket*);

};
#endif // MAINSERVER_H
