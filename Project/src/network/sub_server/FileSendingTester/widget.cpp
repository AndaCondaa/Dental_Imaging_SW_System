#include "widget.h"
#include <QTcpSocket>
#include <QPushButton>

Widget::Widget(QWidget *parent)
    : QWidget(parent)
{
    QPushButton *button = new QPushButton(this);
    button->setGeometry(50, 50, 100, 100);
    connect(button, SIGNAL(clicked()), this, SLOT(buttonClicked()));

    protocol = new Protocol;

    fileSocket = new QTcpSocket;
    fileSocket->connectToHost("127.0.0.1", 8003);
    if (fileSocket->waitForConnected()) {
//        connect(fileSocket, SIGNAL(readyRead()), SLOT(receiveFile()));
        protocol->sendProtocol(fileSocket, "NEW", ConnectType::MODALITY, "MODALITY");
    } else {
        // 연결 실패  예외처리 구현
    }
}

Widget::~Widget()
{
}

void Widget::buttonClicked()
{
    sendFile();
}

void Widget::sendFile()
{
    QFile file;
    QString fileName;
    QString fileInfo;           // <INFO>파일개수<CR>프레임파일1개당크기

    for (int i = 10; i < 999; i++) {
        if (i >= 100)
            fileName = QString("./CEPH/0%1.raw").arg(i);
        else
            fileName = QString("./CEPH/00%1.raw").arg(i);

        file.setFileName(fileName);
        file.open(QIODevice::ReadOnly);
        totalData.append(file.readAll());
        file.close();
    }
    qDebug("TOTAL SIZE : %d", totalData.size());
    fileInfo = "<CR>" + QString::number(totalData.size()) + "<CR>" + QString::number(48*2400);
    //    totalData.append(fileInfo.toStdString());
    fileSocket->write(totalData);                                       // 영상 파일 전송
    fileSocket->flush();
    totalData.clear();

    fileSocket->write(fileInfo.toStdString().data());                   // 영상 파일 정보 전송
}
