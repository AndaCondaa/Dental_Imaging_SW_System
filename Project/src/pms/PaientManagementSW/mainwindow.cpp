#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "enrollmanager.h"
#include "imagemanager.h"
#include "medicalrecordmanager.h"
#include "patientinfomanager.h"
#include "patientstatusmanager.h"
#include "networkmanager.h"


static inline QByteArray IntToArray(qint32 source);

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);


    //ui->textBrowser->hide();




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


}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_enrollButton_clicked()
{
    enrollManager->show();
}


