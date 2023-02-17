/*
 * 프로그램명 : PatientManagementSW
 * 파일명 : networkManager.h
 * 설명 : 메인서버와 통신
 * 작성자 : 김유선
 * 최종 수정일 : 2023.02.16
 */


#ifndef NETWORKMANAGER_H
#define NETWORKMANAGER_H

#include <QObject>
#include <QtCore>
#include <QtNetwork>
#include <QMessageBox>

#include <QtTest/QtTest>
#include <QTest>

class NetworkManager : public QObject
{
    Q_OBJECT
public:
    NetworkManager(QObject *parent = nullptr);

private slots:
    bool writeData(QByteArray data);
    void connectSocket(QString, int);


    void sendedIP(QString, int);

    void newDataSended(QString);
    void receiveData();
    void receiveFile();


    void makeFiles();



    void downloadOrNotSlot();
    void disconnect();




private:
    QTcpSocket *socket;
    bool send_flag = false;
    QString saveData;


    QTcpSocket *fileSocket;
    QString fileName;

    QByteArray allFile;


    QString id;



    int downloadOrNotFlag = 0;
    int downButtonClicked =0;


    // 0일때는 서버 연결 시도를 했는데 연결 안 된 상태, 1일때는 연결되었다가 서버가 꺼져 연결 끊긴 상태
    int connectCount=0;

    QMessageBox *qmsgBox;

signals:
    void sendNewPID(QString);
    void sendSearchResult(QString, QString);
    void sendSRQRequest(QString);
    void sendVTSRequest(QString);
    void sendISVevent(QString);
    void sendVTFevent(QString);
    void sendVNTevent(QString);


    void PSEDataInNET(QString);


   void sendWTRevent(QString);

   void fileSendedSig(int);


   void sendMWLevent(QString);

   void sendAWLRequest(QString);


   void changeScreenSignal(int);   // 0이면 로그인 화면, 1이면 메인화면

   void waitListClearSignal();

};

#endif // NETWORKMANAGER_H
