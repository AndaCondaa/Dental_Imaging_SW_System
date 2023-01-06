#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QTcpSocket>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    socket = new QTcpSocket(this);

    socket->connectToHost("192.168.0.46", 8000);
    if (socket->waitForConnected()) {
        qDebug("Connection Success!");
        connect(socket, SIGNAL(readyRead()), SLOT(receiveData()));

        QByteArray dataArray;
        QDataStream out(&dataArray, QIODevice::WriteOnly);
        out.device()->seek(0);
        out.writeRawData("Test1!!!!", 1020);
        socket->write(dataArray);     // Send data to server
        socket->flush();
        while(socket->waitForBytesWritten());
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}

