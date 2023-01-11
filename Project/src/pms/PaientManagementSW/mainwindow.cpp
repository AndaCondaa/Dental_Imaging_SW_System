#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "enrollmanager.h"
#include "imagemanager.h"
#include "medicalrecordmanager.h"
#include "patientinfomanager.h"
#include "patientstatusmanager.h"

#include <QBoxLayout>
#include <QDataStream>
#include <QTcpSocket>


static inline QByteArray IntToArray(qint32 source);

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    socket = new QTcpSocket(this);
    fd_flag = connectToHost("127.0.0.1"); // localhost

    //ui->textBrowser->hide();

    if(!fd_flag)
        ui->textBrowser->insertPlainText("Socket connect fail\n");

    imageManager = new ImageManager(this);
    medicalRecordManager = new MedicalRecordManager(this);
    patientInfoManager = new PatientInfoManager(this);
    patientStatusManager = new PatientStatusManager(this);


    QVBoxLayout *imageLayout = new QVBoxLayout(this);
    QVBoxLayout *recordLayout = new QVBoxLayout(this);
    QVBoxLayout *infoLayout = new QVBoxLayout(this);
    QVBoxLayout *statusLayout = new QVBoxLayout(this);


    imageLayout->addWidget(imageManager);
    recordLayout->addWidget(medicalRecordManager);
    infoLayout->addWidget(patientInfoManager);
    statusLayout->addWidget(patientStatusManager);


    ui->imageFrame->setLayout(imageLayout);
    ui->medicalRecordFrame->setLayout(recordLayout);
    ui->patientInfoFrame->setLayout(infoLayout);
    ui->patientStatusFrame->setLayout(statusLayout);

//    connect(patientInfoManager, SIGNAL(()),
//            this, SLOT(()));



}

MainWindow::~MainWindow()
{
    delete ui;
}




bool MainWindow::connectToHost(QString host)
{
    socket->connectToHost(host, 8001); // ip address, port
    return socket->waitForConnected();
}

bool MainWindow::writeData(QByteArray data)
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



void MainWindow::on_enrollButton_clicked()
{
    int pid = 1; // 임시
    char name[10] = "김유선";
    qDebug("2");
    sendProtocol(pid, name);
    qDebug("3");


    enrollManager = new EnrollManager(0);
    enrollManager->show();


//    if(fd_flag)
//    {
//        //이거는 mainwindow쪽에 보내면 될 듯
//        QString textData = QString("Modify_PatientInfo Button click\n");
//        QString sendData = QString("Socket data: patient ID(%1) info changed");

//        ui->textBrowser->insertPlainText(textData);
//        send_flag = writeData(sendData.toStdString().c_str());

//        if(!send_flag)
//            ui->textBrowser->insertPlainText("Socket send fail\n");
//        else
//            ;

//    }
    //emit enrollSignal(pid, name);
}

void MainWindow::sendProtocol(int pid, char *name)
{

    QByteArray dateArray;
    QDataStream out(&dateArray, QIODevice::WriteOnly);
    qDebug("1");
    out.device()->seek(0);
    qDebug("2");
    out << pid;
    qDebug("3");
    out.writeRawData(name, 10);
    qDebug("4");
    qDebug(dateArray);
    qDebug("5");

    PMSocket->write(dateArray);
    qDebug("6");
    PMSocket->flush();
    while(PMSocket->waitForBytesWritten());

}
