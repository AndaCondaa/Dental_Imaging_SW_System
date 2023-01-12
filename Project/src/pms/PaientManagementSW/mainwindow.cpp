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
        ui->textEdit->insertPlainText("Socket connect fail\n");


    imageManager = new ImageManager(this);
    medicalRecordManager = new MedicalRecordManager(this);
    patientInfoManager = new PatientInfoManager(this);
    patientStatusManager = new PatientStatusManager(this);
    enrollManager = new EnrollManager(0);

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

    connect(enrollManager, SIGNAL(sendNewData(QString)), this, SLOT(newDataSended(QString)));


}

MainWindow::~MainWindow()
{
    delete ui;
}




bool MainWindow::connectToHost(QString host)
{
    socket->connectToHost(host, 8001);
    return socket->waitForConnected();
}

bool MainWindow::writeData(QByteArray data)
{
    if(socket->state() == QAbstractSocket::ConnectedState)
    {
        socket->write(IntToArray(data.size())); // 데이터 사이즈를 보내줌
        socket->write(data); // 데이터를 보내줌
        return socket->waitForBytesWritten();
    }
    else
    {
        return false;
    }
}

QByteArray IntToArray(qint32 source)
{
    QByteArray temp;
    QDataStream data(&temp, QIODevice::ReadWrite);
    data << source;
    return temp;
}



void MainWindow::on_enrollButton_clicked()
{
//    int pid = 1; // 임시
//    char name[10] = "김유선";
//    qDebug("2");
//    sendProtocol(pid, name);
//    qDebug("3");



    enrollManager->show();





}

void MainWindow::newDataSended(QString newData)
{

    if(fd_flag)
    {
        QString textData = QString("Enroll_PatientInfo Button click\n");    //MainWindow의 textEdit에 띄울 정보
        QString sendData = newData; //MainServer의 textEdit에 띄울 정보

        ui->textEdit->insertPlainText(textData);
        send_flag = writeData(sendData.toStdString().c_str()); //writeData의 첫 번째 인자는 char *data와 같은 형식임

        if(!send_flag)
            ui->textEdit->insertPlainText("Socket send fail\n");
        else
            ;

    }

}
