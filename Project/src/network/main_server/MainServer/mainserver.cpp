#include "mainserver.h"
#include "ui_mainserver.h"
#include <QTcpServer>
#include <QTcpSocket>

static inline qint32 ArrayToInt(QByteArray source);

MainServer::MainServer(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainServer)
{
    ui->setupUi(this);
    server = new QTcpServer(this);
    connect(server, SIGNAL(newConnection()), this, SLOT(newConnection()));

    QString socket_data = QString("Listening: %1\n").arg(server->listen(QHostAddress::Any, 8001) ? "true" : "false");
    ui->textEdit->insertPlainText(socket_data);

}

MainServer::~MainServer()
{
    delete ui;
}

void MainServer::newConnection()
{
    while (server->hasPendingConnections())
    {
        QTcpSocket *socket = server->nextPendingConnection();
        connect(socket, SIGNAL(readyRead()), this, SLOT(receiveData()));
        connect(socket, SIGNAL(disconnected()), this, SLOT(disconnected()));
        QByteArray *buffer = new QByteArray();
        qint32 *s = new qint32(0);
        buffers.insert(socket, buffer);
        sizes.insert(socket, s);
    }
}

void MainServer::disconnected()
{
    QTcpSocket *socket = static_cast<QTcpSocket*>(sender());
    QByteArray *buffer = buffers.value(socket);
    qint32 *s = sizes.value(socket);
    socket->deleteLater();
    delete buffer;
    delete s;
}

void MainServer::receiveData()
{

    QTcpSocket *socket = static_cast<QTcpSocket*>(sender());
    QByteArray *buffer = buffers.value(socket);
        qint32 *s = sizes.value(socket);
        qint32 size = *s;
        while (socket->bytesAvailable() > 0)
        {
            buffer->append(socket->readAll());
            while ((size == 0 && buffer->size() >= 4) || (size > 0 && buffer->size() >= size)) //While can process data, process it
            {
                if (size == 0 && buffer->size() >= 4) //if size of data has received completely, then store it on our global variable
                {
                    size = ArrayToInt(buffer->mid(0, 4));
                    *s = size;
                    buffer->remove(0, 4);
                }

                if (size > 0 && buffer->size() >= size) // If data has received completely, then emit our SIGNAL with the data
                {
                    ui->textEdit->insertPlainText(QString(buffer->data()));
                    QByteArray data = buffer->mid(0, size);
                    buffer->remove(0, size);
                    size = 0;
                    *s = size;
                    emit dataReceived(data);
                }
            }
        }
}

qint32 ArrayToInt(QByteArray source)
{
    qint32 temp;
    QDataStream data(&source, QIODevice::ReadWrite);
    data >> temp;
    return temp;
}
