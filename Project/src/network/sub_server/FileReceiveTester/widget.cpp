#include "widget.h"

#include <QTextEdit>
#include <QTcpSocket>
#include <QFileInfo>

Widget::Widget(QWidget *parent)
    : QWidget(parent)
{
    QTextEdit *log = new QTextEdit(this);
    log->setReadOnly(true);
    log->setGeometry(0, 0, 200, 200);

    server = new QTcpServer(this);
    connect(server, SIGNAL(newConnection()), this, SLOT(newFileClient()));
    server->listen(QHostAddress::Any, 8001);
}


Widget::~Widget()
{
}

void Widget::newFileClient() {
    QTcpSocket *fileSocket = server->nextPendingConnection();
    connect(fileSocket, SIGNAL(readyRead()), this, SLOT(receiveFile()));
}

void Widget::receiveFile()
{
    QTcpSocket *socket = dynamic_cast<QTcpSocket*>(sender());

    // Beginning File Transfer
    if (byteReceived == 0) {                                    // First Time(Block) , var byteReceived is always zero
        checkFileName = fileName;                               // 다음 패킷부터 파일이름으로 구분하기 위해 첫 패킷에서 보낸 파일이름을 임시로 저장

        QDataStream in(socket);
        in.device()->seek(0);
        in >> totalSize >> byteReceived >> fileName;
        if(checkFileName == fileName) return;

        QFileInfo info(fileName);
        QString currentFileName = info.fileName();
        qDebug() << currentFileName;
        file = new QFile(currentFileName);
        file->open(QFile::WriteOnly);
    } else {
        if(checkFileName == fileName) return;
        inBlock = socket->readAll();

        byteReceived += inBlock.size();
        file->write(inBlock);
        file->flush();
    }

    if (byteReceived == totalSize) {        // file sending is done
        qDebug() << QString("%1 receive completed").arg(fileName);
        inBlock.clear();
        byteReceived = 0;
        totalSize = 0;
        file->close();
        delete file;
    }
}
