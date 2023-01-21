#ifndef MAINSERVER_H
#define MAINSERVER_H

#include <QMainWindow>
#include <QtCore>
#include <QtNetwork>
#include <QString>
#include <QSqlTableModel>
#include <QTableWidget>

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
    void updateRecentData();

signals:
    //void dataReceived(QByteArray);

private slots:
    void newConnection();
    void disconnected();
    void receiveData();
    bool writeData(QByteArray data);

private slots:
    void sendDataToClient(QString);

private:
    Ui::MainServer *ui;
    QTcpServer *server;
    QHash<QTcpSocket*, QByteArray*> buffers; //We need a buffer to store data until block has completely received
    QHash<QTcpSocket*, qint32*> sizes; //We need to store the size to verify if a block has received completely
    QString makeId();
    QString makeReportNo();
    QString newReportNo();
    QSqlQuery *query;
    QSqlQuery *query2;
    QSqlQuery *query3;
    QSqlQuery *query4;
    QSqlQuery *query5;

    //QTcpSocket *socket;
    QTcpSocket *pmsSocket;
    QTcpSocket *imagingSocket;
    QTcpSocket *viewerSocket;

    QByteArray *buffer;
    QString saveData;

    QSqlTableModel *patientModel;
    QSqlTableModel *dentistModel;
    QSqlTableModel *imageModel;
    QSqlTableModel *reportModel;
    QSqlTableModel *imageRelationModel;

    bool fd_flag = false;
    bool send_flag = false;

    QMap<QTcpSocket *, QString> sk;

signals:
    void sendNewPID(QString);

};
#endif // MAINSERVER_H
