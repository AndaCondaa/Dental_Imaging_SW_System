/*프로그램명 : ImageViewerSW
파일명 : networkmanager.h
설명 : 서버와 클라이언트(뷰어 SW)간 Event를 통해 데이터를 주고받고 하기 위한 클래스
작성자 : 이정연
최종 수정 날짜 : 2023.02.11*/

#ifndef NETWORKMANAGER_H
#define NETWORKMANAGER_H

#include <QObject>
#include <QtCore>
#include <QtNetwork>
#include <QMessageBox>

class NetworkManager : public QObject
{
    Q_OBJECT

public:
    NetworkManager(QObject *parent = nullptr);

private slots:
    bool writeData(QByteArray data);
    void newDataSended(QString);
    void receiveData();
    void receiveFile();
    void makeFiles();
    void disconnect();
    void receiveLoginToServer(QString);

private:
    QTcpSocket *socket;
    QTcpSocket* fileSocket;
    QByteArray allFile;
    QString saveData;
    QString fileName;                           // Receiving FileName
    QString checkFileName;
    QMessageBox* messagebox;
    bool send_flag = false;
    int allFileSize = 0;

signals:
    void sendWaitingList(QString, QString);
    void sendSelectPatient(QString, QString);
    void sendPMSCameraPatient(QString, QString);
    void sendLogInCheck(QString);
    void sendPhotoEnd(QString);
    void sendWaitTreatment(int, QString);
    void sendImageFile();
    void sendAllImageFile(bool);
    void sendPatientTreatmentEnd();
    void sendPrescriptionCheck(QString);
    void serverConnection(int);
    void disconnectedToServer(int);
};

#endif // NETWORKMANAGER_H
