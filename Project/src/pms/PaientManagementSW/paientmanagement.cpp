#include "paientmanagement.h"
#include "ui_paientmanagement.h"

static inline QByteArray IntToArray(qint32 source);

PaientManagement::PaientManagement(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::PaientManagement)
{
    ui->setupUi(this);
    socket = new QTcpSocket(this);
    fd_flag = connectToHost("127.0.0.1"); // localhost

    //ui->textBrowser->hide();

    if(!fd_flag)
        ui->textBrowser->insertPlainText("Socket connect fail\n");
}

PaientManagement::~PaientManagement()
{
    delete ui;
}




bool PaientManagement::connectToHost(QString host)
{
    socket->connectToHost(host, 8001); // ip address, port
    return socket->waitForConnected();
}

bool PaientManagement::writeData(QByteArray data)
{
    if(socket->state() == QAbstractSocket::ConnectedState)
    {
        socket->write(IntToArray(data.size())); // write size of data
        socket->write(data); // write the data itself
        return socket->waitForBytesWritten();
    }
    else
    {
        return false;
    }
}

QByteArray IntToArray(qint32 source) // Use qint32 to ensure that the number have 4 bytes
{
    // Avoid use of cast, this is the Qt way to serialize objects
    QByteArray temp;
    QDataStream data(&temp, QIODevice::ReadWrite);
    data << source;
    return temp;
}

void PaientManagement::on_modifyPatientInfo_pushButton_clicked()
{
    if(fd_flag)
    {
        QString textData = QString("Modify_PatientInfo Button click: %1\n").arg(cnt);
        QString sendData = QString("Socket data: %1\n").arg(cnt);

        ui->textBrowser->insertPlainText(textData);
        send_flag = writeData(sendData.toStdString().c_str());

        if(!send_flag)
            ui->textBrowser->insertPlainText("Socket send fail\n");
        else
            cnt++;
    }
}
