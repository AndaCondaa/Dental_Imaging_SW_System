/*
 * 프로그램명 : PatientManagementSW
 * 파일명 : mainWindow.cpp
 * 설명 : 로그인과 PMS의 메인화면
 * 작성자 : 김유선
 * 최종 수정일 : 2023.02.20
 */


#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "enrollmanager.h"
#include "imagemanager.h"
#include "medicalrecordmanager.h"
#include "patientinfomanager.h"
#include "patientstatusmanager.h"
#include "networkmanager.h"
#include "choosepatientmanager.h"
#include <QPixmap>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);


    //this->showMaximized();
    this->showFullScreen();
    this->statusBar()->hide();

    // 로그인 이미지 설정
    pixmap = new QPixmap();
    pixmap->load(":/img/login.png");
    pixmap->scaled(180,180,Qt::IgnoreAspectRatio);
    ui->label_11->setPixmap(pixmap->scaled(ui->label_11->size(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation));

    // 로그인 화면에서 Enter 누르면 로그인 시도
    connect(ui->ipLineEdit, SIGNAL(returnPressed()), this, SLOT(on_pushButton_clicked()));
    connect(ui->portLineEdit, SIGNAL(returnPressed()), this, SLOT(on_pushButton_clicked()));

    // X 아이콘 클릭 시 프로그램 종료
    connect(ui->quitToolButton, SIGNAL(clicked()), qApp, SLOT(quit()));
    connect(ui->quitToolButton_2, SIGNAL(clicked()), qApp, SLOT(quit()));


    // 로그인 버튼 설정
    ui->pushButton->setStyleSheet("QPushButton { "
                                  "border-radius: 10px;"
                                  "background-color: rgb(255, 162, 0);"
                                  "border-radius:10px;"
                                  "color:#ffffff;"
                                  "outline: 0; "
                              "}"
                                  "QPushButton:hover { "
                                    "background-color: rgb(255, 150, 0);"
                                    "border-radius:10px;"
                                    "color:#ffffff;"
                                    "outline: 0; "
                                "}");


    // 신규환자 등록 설정
    ui->enrollButton->setIcon(QIcon(":/img/enroll.png"));
    ui->enrollButton->setStyleSheet("QPushButton { "
                                    "border-style: outset;"
                                    "border-width: 3px;"
                                    "border-radius: 10px;"
                                    "border-color: orange;"
                                    "background-color: #FFFFFF;"
                                    "border-radius:10px;"
                                    "color:#ffffff;"
                                    "outline: 0; "
                                "}"
                                "QPushButton:hover { "
                                    "background-color: qlineargradient(spread:pad, x1:0.6, y1:0.6, x2:1, y2:1, stop:0 rgba(255, 255, 255, 255), stop:1 rgba(255, 165, 1, 100));"

                                    "border-radius:10px;"
                                    "color:#ffffff;"
                                    "outline: 0; "
                                "}");


    // 클래스 생성
    imageManager = new ImageManager(this);
    medicalRecordManager = new MedicalRecordManager(this);
    patientInfoManager = new PatientInfoManager(this);
    patientStatusManager = new PatientStatusManager(this);
    enrollManager = new EnrollManager(0);
    networkManager = new NetworkManager(this);
    networkManager->setObjectName("NETWORK");
    choosePatientManager = new ChoosePatientManager(0);


    // QTest::qExec(imageManager);
    // QTest::qExec(medicalRecordManager);
    // QTest::qExec(patientInfoManager);
    // QTest::qExec(patientStatusManager);
    // QTest::qExec(enrollManager);
    // QTest::qExec(networkManager);
    // QTest::qExec(choosePatientManager);


    // 레이아웃 설정
    QVBoxLayout *imageLayout = new QVBoxLayout;
    QVBoxLayout *recordLayout = new QVBoxLayout;
    QVBoxLayout *infoLayout = new QVBoxLayout;
    QVBoxLayout *statusLayout = new QVBoxLayout;

    imageLayout->addWidget(imageManager);
    recordLayout->addWidget(medicalRecordManager);
    infoLayout->addWidget(patientInfoManager);
    statusLayout->addWidget(patientStatusManager);

    ui->imageFrame->setLayout(imageLayout);
    ui->medicalRecordFrame->setLayout(recordLayout);
    ui->patientInfoFrame->setLayout(infoLayout);
    ui->patientStatusFrame->setLayout(statusLayout);





    connect(patientInfoManager, SIGNAL(sendTempData(QString, QString)), choosePatientManager, SLOT(sendedTempData(QString, QString)));

    connect(choosePatientManager, SIGNAL(choosePatientSignal(QString)), networkManager, SLOT(newDataSended(QString)));


    connect(ui->enrollButton, SIGNAL(clicked()), this, SLOT(enrollClient()));

    connect(enrollManager, SIGNAL(sendNewData(QString)), networkManager, SLOT(newDataSended(QString)));
    connect(enrollManager, SIGNAL(sendNewDataForShow(QString, QString)), patientInfoManager, SLOT(sendedNewDataForShow(QString, QString)));


    connect(medicalRecordManager, SIGNAL(sendReSearchData(QString)), networkManager, SLOT(newDataSended(QString)));



    connect(patientInfoManager, SIGNAL(sendSearchData(QString)), networkManager, SLOT(newDataSended(QString)));
    connect(this, SIGNAL(requestPID(QString)), networkManager, SLOT(newDataSended(QString)));
    connect(networkManager, SIGNAL(sendNewPID(QString)), enrollManager, SLOT(newPIDSended(QString)));
    connect(networkManager, SIGNAL(sendSearchResult(QString, QString)), patientInfoManager, SLOT(searchDataSended(QString, QString)));

    connect(networkManager, SIGNAL(PSEDataInNET(QString)), imageManager, SLOT(PSEDataSended(QString)));

    connect(patientInfoManager, SIGNAL(sendDelData(QString)), networkManager, SLOT(newDataSended(QString)));



    // 대기명단 추가 되는지 안되는지 받아오는 부분
    connect(networkManager, SIGNAL(sendAWLRequest(QString)), patientInfoManager, SLOT(sendedAWLRequest(QString)));




    // 대기명단에 추가해도 되는 상태인지 메인서버에 확인
    connect(patientInfoManager, SIGNAL(sendWaitInfo(QString)), networkManager, SLOT(newDataSended(QString)));

    connect(patientStatusManager, SIGNAL(sendRequest(QString)), networkManager, SLOT(newDataSended(QString)));
    connect(patientInfoManager, SIGNAL(sendModifyData(QString)), networkManager, SLOT(newDataSended(QString)));


    connect(networkManager, SIGNAL(sendSRQRequest(QString)), patientStatusManager, SLOT(statusRequestSended(QString)));
    connect(networkManager, SIGNAL(sendVTSRequest(QString)), patientStatusManager, SLOT(statusRequestSended(QString)));

    // 처방전 작성하면 환자진료기록 바로 띄울 수 있도록 함
    connect(networkManager, SIGNAL(sendVNTevent(QString)), medicalRecordManager, SLOT(addNewRecord(QString)));

    connect(networkManager, SIGNAL(sendISVevent(QString)), patientStatusManager, SLOT(statusRequestSended(QString)));


    connect(networkManager, SIGNAL(sendVTFevent(QString)), patientStatusManager, SLOT(statusRequestSended(QString)));

    connect(networkManager, SIGNAL(sendSearchResult(QString, QString)), medicalRecordManager, SLOT(recordDataSended(QString, QString)));



    connect(patientInfoManager, SIGNAL(sendPIDtoWaitList(QString)), patientStatusManager, SLOT(PIDsendedtoWaitList(QString)));
    connect(patientStatusManager, SIGNAL(inWaitListSignal(int)), patientInfoManager, SLOT(inWaitListSlot(int)));



    connect(patientInfoManager, SIGNAL(sendDelPID(QString)), patientStatusManager, SLOT(delPIDSended(QString)));
    connect(patientStatusManager, SIGNAL(sendDelFlag(int)), patientInfoManager, SLOT(delFlagSended(int)));
    connect(patientInfoManager, SIGNAL(sendDelPID(QString)), imageManager, SLOT(delPIDSendedToImg(QString)));   // 인자 필요없지만 signal 재활용 위해 사용



    connect(patientStatusManager, SIGNAL(sendPayInfo(QString)), networkManager, SLOT(newDataSended(QString)));



    connect(networkManager, SIGNAL(sendWTRevent(QString)), patientStatusManager, SLOT(oldListSended(QString)));



    connect(patientInfoManager, SIGNAL(downloadOrNotSignal()), networkManager, SLOT(downloadOrNotSlot()));



    connect(networkManager, SIGNAL(fileSendedSig(int)), patientInfoManager, SLOT(fileSendedSlot(int)));


    // search버튼 클릭되면 이전 환자의 이미지가 보이지 않도록 imageManager쪽으로 signal 보내줌
    connect(patientInfoManager, SIGNAL(cleanImageSignal()), imageManager, SLOT(cleanImageSlot()));   // 인자 필요없지만 signal 재활용 위해 사용


    // 진료중이었던 환자 상태를 진료대기로 변경
    connect(networkManager, SIGNAL(sendMWLevent(QString)), patientStatusManager, SLOT(statusRequestSended(QString)));


    connect(patientInfoManager, SIGNAL(sendWaitInfoToWaitList(QString, QString)), patientStatusManager, SLOT(waitInfoSended(QString, QString)));


    connect(this, SIGNAL(sendIP(QString, int)), networkManager, SLOT(sendedIP(QString, int)));

    connect(networkManager, SIGNAL(changeScreenSignal(int)), this, SLOT(changeScreenSlot(int)));

    connect(networkManager, SIGNAL(waitListClearSignal()), patientStatusManager, SLOT(waitListClearSlot()));
}

MainWindow::~MainWindow()
{
    delete ui;
}


// 환자를 등록할 때 새로운 환자 번호를 서버에 요청하는 부분
void MainWindow::enrollClient()
{
    enrollManager->show();
    QString requestPIDData = "SEN^PID<CR>NULL<CR>NULL";  // PID라는 이벤트만 전송되면 되는 부분. 나머지 데이터는 0으로 의미없는 데이터를 임의로 보내주었음
    emit requestPID(requestPIDData);
}


// 로그인 버튼 클릭하면 네트워크매니저쪽으로 입력한 id와 port번호를 전송
void MainWindow::on_pushButton_clicked()
{
    QString IP = ui->ipLineEdit->text();
    int PORT = ui->portLineEdit->text().toInt();

    emit sendIP(IP, PORT);

}

// flag가 0이면 로그인 화면, 1이면 메인화면으로 화면 전환
void MainWindow::changeScreenSlot(int flag)
{
    if(flag == 1)
    {

        ui->stackedWidget->setCurrentIndex(0);

    }
    else if(flag == 0)
    {
        ui->stackedWidget->setCurrentIndex(1);
    }
}


