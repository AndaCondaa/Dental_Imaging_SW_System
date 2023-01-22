#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "enrollmanager.h"
#include "imagemanager.h"
#include "medicalrecordmanager.h"
#include "patientinfomanager.h"
#include "patientstatusmanager.h"
#include "networkmanager.h"


//static inline QByteArray IntToArray(qint32 source);

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);


    ui->textEdit->hide();




    imageManager = new ImageManager(this);
    medicalRecordManager = new MedicalRecordManager(this);
    patientInfoManager = new PatientInfoManager(this);
    patientStatusManager = new PatientStatusManager(this);
    enrollManager = new EnrollManager(0);
    networkManager = new NetworkManager(this);


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

    connect(enrollManager, SIGNAL(sendNewData(QString)), networkManager, SLOT(newDataSended(QString)));
    connect(enrollManager, SIGNAL(sendNewDataForShow(QString, QString)), patientInfoManager, SLOT(searchDataSended(QString, QString)));


    connect(patientInfoManager, SIGNAL(sendSearchData(QString)), networkManager, SLOT(newDataSended(QString)));
    connect(this, SIGNAL(requestPID(QString)), networkManager, SLOT(newDataSended(QString)));
    connect(networkManager, SIGNAL(sendNewPID(QString)), enrollManager, SLOT(newPIDSended(QString)));
    connect(networkManager, SIGNAL(sendSearchResult(QString, QString)), patientInfoManager, SLOT(searchDataSended(QString, QString)));
    connect(patientInfoManager, SIGNAL(sendDelData(QString)), networkManager, SLOT(newDataSended(QString)));
    //connect(patientInfoManager, SIGNAL(sendWaitInfo(QString, QString)), patientStatusManager, SLOT(waitInfoSended(QString, QString)));
    connect(patientInfoManager, SIGNAL(sendWaitInfo(QString)), patientStatusManager, SLOT(waitInfoSended(QString)));
    connect(patientInfoManager, SIGNAL(sendWaitInfo(QString)), networkManager, SLOT(newDataSended(QString)));
    connect(patientStatusManager, SIGNAL(sendWaitInfo(QString)), networkManager, SLOT(newDataSended(QString)));
    connect(patientStatusManager, SIGNAL(sendRequest(QString)), networkManager, SLOT(newDataSended(QString)));
    //connect(networkManager, SIGNAL(sendSRQRequest(QString)), patientStatusManager, SLOT(SRQRequestSended(QString)));
    connect(patientInfoManager, SIGNAL(sendModifyData(QString)), networkManager, SLOT(newDataSended(QString)));


    //connect(patientInfoManager, SIGNAL(sendWaitToServer(QString)), networkManager, SLOT(newDataSended(QString)));
    connect(networkManager, SIGNAL(sendSRQRequest(QString)), patientStatusManager, SLOT(statusRequestSended(QString)));
    connect(networkManager, SIGNAL(sendVTSRequest(QString)), patientStatusManager, SLOT(statusRequestSended(QString)));
    connect(networkManager, SIGNAL(sendISVevent(QString)), patientStatusManager, SLOT(statusRequestSended(QString)));


    connect(networkManager, SIGNAL(sendVTFevent(QString)), patientStatusManager, SLOT(statusRequestSended(QString)));

    connect(networkManager, SIGNAL(sendSearchResult(QString, QString)), medicalRecordManager, SLOT(recordDataSended(QString, QString)));

}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_enrollButton_clicked()
{
    enrollManager->show();
    QString requestPIDData = "PID<CR>0<CR>0";  //PID라는 이벤트만 전송되면 되는 부분. 나머지 데이터는 0으로 의미없는 데이터를 임의로 보내주었음
    emit requestPID(requestPIDData);

}


