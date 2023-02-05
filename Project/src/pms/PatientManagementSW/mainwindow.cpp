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

    this->showMaximized();


//    ui->enrollButton->setStyleSheet("QPushButton { "
//                                            "qlineargradient(spread:pad, x1:0, y1:0, x2:1, y2:1, stop:0 rgba(149, 210, 253, 255), stop:1 rgba(152, 136, 248, 255));"
//                                            "border-radius:10px;"
//                                            "color:#ffffff;"
//                                            "padding-right:10px;"
//                                            "outline: 0; "
//                                        "}"
//                                        "QPushButton:hover { "
//                                            "background-color: qlineargradient(spread:pad, x1:0, y1:0, x2:1, y2:1, stop:0 rgb(66, 130, 220), stop:1 rgb(184, 58, 166)); "
//                                            "border-radius:10px;"
//                                            "color:#ffffff;"
//                                            "outline: 0; "
//                                        "}");





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


    connect(medicalRecordManager, SIGNAL(sendReSearchData(QString)), networkManager, SLOT(newDataSended(QString)));



    connect(patientInfoManager, SIGNAL(sendSearchData(QString)), networkManager, SLOT(newDataSended(QString)));
    connect(this, SIGNAL(requestPID(QString)), networkManager, SLOT(newDataSended(QString)));
    connect(networkManager, SIGNAL(sendNewPID(QString)), enrollManager, SLOT(newPIDSended(QString)));
    connect(networkManager, SIGNAL(sendSearchResult(QString, QString)), patientInfoManager, SLOT(searchDataSended(QString, QString)));

    //검색될 때 관련 이미지를 불러올 수 있도록 pid를 전송
    //connect(networkManager, SIGNAL(sendSearchResult(QString, QString)), imageManager, SLOT(PSEDataToImgManager(QString, QString)));
    connect(networkManager, SIGNAL(PSEDataInNET(QString)), imageManager, SLOT(PSEDataSended(QString)));

    connect(patientInfoManager, SIGNAL(sendDelData(QString)), networkManager, SLOT(newDataSended(QString)));
    //connect(patientInfoManager, SIGNAL(sendWaitInfo(QString, QString)), patientStatusManager, SLOT(waitInfoSended(QString, QString)));
    connect(patientInfoManager, SIGNAL(sendWaitInfo(QString)), patientStatusManager, SLOT(waitInfoSended(QString)));
    connect(patientInfoManager, SIGNAL(sendWaitInfo(QString)), networkManager, SLOT(newDataSended(QString)));

    //상관없는지 정연이꺼에서 테스트
    //connect(patientStatusManager, SIGNAL(sendWaitInfo(QString)), networkManager, SLOT(newDataSended(QString)));
    connect(patientStatusManager, SIGNAL(sendRequest(QString)), networkManager, SLOT(newDataSended(QString)));
    connect(patientInfoManager, SIGNAL(sendModifyData(QString)), networkManager, SLOT(newDataSended(QString)));


    //connect(patientInfoManager, SIGNAL(sendWaitToServer(QString)), networkManager, SLOT(newDataSended(QString)));
    connect(networkManager, SIGNAL(sendSRQRequest(QString)), patientStatusManager, SLOT(statusRequestSended(QString)));
    connect(networkManager, SIGNAL(sendVTSRequest(QString)), patientStatusManager, SLOT(statusRequestSended(QString)));

    //처방전 작성하면 환자진료기록 바로 띄울 수 있도록 함
    connect(networkManager, SIGNAL(sendVNTevent(QString)), medicalRecordManager, SLOT(addNewRecord(QString)));

    connect(networkManager, SIGNAL(sendISVevent(QString)), patientStatusManager, SLOT(statusRequestSended(QString)));


    connect(networkManager, SIGNAL(sendVTFevent(QString)), patientStatusManager, SLOT(statusRequestSended(QString)));

    connect(networkManager, SIGNAL(sendSearchResult(QString, QString)), medicalRecordManager, SLOT(recordDataSended(QString, QString)));



    connect(patientInfoManager, SIGNAL(sendPIDtoWaitList(QString)), patientStatusManager, SLOT(PIDsendedtoWaitList(QString)));
    connect(patientStatusManager, SIGNAL(inWaitListSignal(int)), patientInfoManager, SLOT(inWaitListSlot(int)));



    connect(patientInfoManager, SIGNAL(sendDelPID(QString)), patientStatusManager, SLOT(delPIDSended(QString)));
    connect(patientStatusManager, SIGNAL(sendDelFlag(int)), patientInfoManager, SLOT(delFlagSended(int)));
    connect(patientInfoManager, SIGNAL(sendDelPID(QString)), imageManager, SLOT(delPIDSendedToImg(QString)));   //인자 필요없지만 signal 재활용 위해 사용



    connect(patientStatusManager, SIGNAL(sendPayInfo(QString)), networkManager, SLOT(newDataSended(QString)));



    connect(networkManager, SIGNAL(sendWTRevent(QString)), patientStatusManager, SLOT(oldListSended(QString)));



    connect(patientInfoManager, SIGNAL(downloadOrNotSignal()), networkManager, SLOT(downloadOrNotSlot()));





//    connect(networkManager, SIGNAL(sendByteArray(QByteArray*)), patientInfoManager, SLOT(byteArraySended(QByteArray*)));
//    connect(networkManager, SIGNAL(sendByteArray(const QPixmap&)), patientInfoManager, SLOT(byteArraySended(const QPixmap&)));


    connect(networkManager, SIGNAL(fileSendedSig(int)), patientInfoManager, SLOT(fileSendedSlot(int)));


    //search버튼 클릭되면 이전 환자의 이미지가 보이지 않도록 imageManager쪽으로 signal 보내줌
    connect(patientInfoManager, SIGNAL(cleanImageSignal()), imageManager, SLOT(cleanImageSlot()));   //인자 필요없지만 signal 재활용 위해 사용

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


