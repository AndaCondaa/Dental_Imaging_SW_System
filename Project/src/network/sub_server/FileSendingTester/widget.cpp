#include "widget.h"
#include "packetdata.h"
#include <QTcpSocket>
#include <QPushButton>

Widget::Widget(QWidget *parent)
    : QWidget(parent)
{
    QPushButton *button = new QPushButton("send", this);
    QPushButton *cephButton = new QPushButton("CEPH", this);
    QPushButton *panoButton = new QPushButton("PANO", this);
    button->setGeometry(60, 120, 100, 100);
    panoButton->setGeometry(0, 0, 100, 100);
    cephButton->setGeometry(120, 0, 100, 100);
    connect(button, SIGNAL(clicked()), this, SLOT(buttonClicked()));
    connect(cephButton, SIGNAL(clicked()), this, SLOT(cephButtonClicked()));
    connect(panoButton, SIGNAL(clicked()), this, SLOT(panoButtonClicked()));


    protocol = new Protocol;

    controlSocket = new QTcpSocket;

    controlSocket->connectToHost("127.0.0.1", 8002);

    if (controlSocket->waitForConnected()) {

        connect(controlSocket, SIGNAL(readyRead()), SLOT(receiveControl()));
        protocol->sendProtocol(controlSocket, "SEN", "NEW", ConnectType::MODALITY, "MODALITY");

    }

    fileSocket = new QTcpSocket;
    fileSocket->connectToHost("127.0.0.1", 8003);
    if (fileSocket->waitForConnected()) {
        protocol->sendProtocol(fileSocket, "SEN", "NEW", ConnectType::MODALITY, "MODALITY");
    } else {
        // 연결 실패  예외처리 구현
    }
}

Widget::~Widget()
{
}

void Widget::receiveControl()
{
    QTcpSocket *socket = dynamic_cast<QTcpSocket*>(sender());
    protocol->receiveProtocol(socket);
    if (protocol->packetData()->type() == ControlType::START) {
        sendFile();
    } else if (protocol->packetData()->type() == ControlType::STOP) {
        flag = true;
    }
}

void Widget::buttonClicked()
{
    sendFile();
}

void Widget::cephButtonClicked()
{
    currentType = "CEPH";
}

void Widget::panoButtonClicked()
{
    currentType = "PANO";
}

void Widget::sendFile()
{
    if (currentType == "PANO")
        countMax = 1750;
    else if (currentType == "CEPH")
        countMax = 1250;

    // CEPH MODE
    for (int i = 0; i < countMax; i++) {
        if (flag) {
            flag = false;
            return;
        }

        if (i >= 1000)
            fileName = QString("./%1/%2.raw").arg(currentType).arg(i);
        else if (i < 1000 && i >= 100)
            fileName = QString("./%1/0%2.raw").arg(currentType).arg(i);
        else if (i < 100 && i >= 10)
            fileName = QString("./%1/00%2.raw").arg(currentType).arg(i);
        else
            fileName = QString("./%1/000%2.raw").arg(currentType).arg(i);

        file.setFileName(fileName);
        file.open(QIODevice::ReadOnly);
        fileSocket->write(file.readAll());
        file.close();
        qDebug("%d", i);
    }
}
