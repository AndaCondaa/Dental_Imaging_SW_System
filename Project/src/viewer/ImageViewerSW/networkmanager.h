#ifndef NETWORKMANAGER_H
#define NETWORKMANAGER_H

#include <QObject>
#include <QtCore>
#include <QtNetwork>

class NetworkManager : public QObject
{
    Q_OBJECT
public:
    NetworkManager(QObject *parent = nullptr);

public slots:
    bool connectToHost(QString host);
    bool writeData(QByteArray data);

private slots:
    void newDataSended(QString);
    void receiveData();
    void receiveFile();
    void makeFiles();
    void disconnect();

private:
    QTcpSocket *socket;
    QTcpSocket* fileSocket;
    bool fd_flag = false;
    bool send_flag = false;
    QByteArray *buffer;
    QString saveData;
    QTcpServer *server;

    QString fileName;                           // Receiving FileName
    QString checkFileName;

    QByteArray allFile;
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
};

#endif // NETWORKMANAGER_H
