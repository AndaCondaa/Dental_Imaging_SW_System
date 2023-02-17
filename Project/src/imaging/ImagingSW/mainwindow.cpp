/*
 * 프로그램명 : ImagingSW
 * 파일명 : mainwindow.cpp
 * 설명 : 촬영SW의 메인GUI
 * 작성자 : 안다미로
 * 최종 수정일 : 2023.02.13
 */

#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QVBoxLayout>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle("Imaging SW");
    settingStyleSheet();

    patientManager = new PatientManager(this);
    controlPanel = new ControlPanel(this);
    imagingManager = new ImagingManager(this);
    mainNetworkManager = new MainNetworkManager(this);
    subNetworkManager = new SubNetworkManager(this);

    QVBoxLayout *patientLayout = new QVBoxLayout();
    QVBoxLayout *controlLayout = new QVBoxLayout();
    QVBoxLayout *imagingLayout = new QVBoxLayout();

    patientLayout->addWidget(patientManager);
    controlLayout->addWidget(controlPanel);
    imagingLayout->addWidget(imagingManager);

    ui->patientFrame->setLayout(patientLayout);
    ui->controlFrame->setLayout(controlLayout);
    ui->imagingFrame->setLayout(imagingLayout);

    ui->patientFrame->setContentsMargins(0, 0, 0, 0);
    ui->controlFrame->setContentsMargins(0, 0, 0, 0);
    ui->imagingFrame->setContentsMargins(0, 0, 0, 0);

    // 서버 연결요청 이벤트
    connect(this, SIGNAL(connectMainServer(QString,int)), mainNetworkManager, SLOT(connectServer(QString,int)));
    connect(this, SIGNAL(connectSubServer(QString,int)), subNetworkManager, SLOT(connectServer(QString,int)));

    connect(ui->mainConnectButton, &QPushButton::clicked, this, [=](){                                      // 메인서버 연결버튼 클릭
        emit connectMainServer(ui->mainIpEdit->text(), ui->mainPortEdit->text().toInt());
    });

    connect(ui->subConnectButton, &QPushButton::clicked, this, [=]() {                                      // 서브서버 연결버튼 클릭
        emit connectSubServer(ui->subIpEdit->text(), ui->subPortEdit->text().toInt());
    });

    connect(mainNetworkManager, &MainNetworkManager::connectionStatusChanged, this, [=](bool op){           // 메인서버 연결상태가 변경된 경우
        if (op == true) {
            ui->mainIpEdit->setReadOnly(true);
            ui->mainPortEdit->setReadOnly(true);

            ui->mainConnectButton->setEnabled(false);
            ui->mainConnectButton->setText("정상 연결");

            if (!ui->subConnectButton->isEnabled())
                ui->pageStackedWidget->setCurrentIndex(1);
        } else if (op == false) {
            ui->mainIpEdit->setReadOnly(false);
            ui->mainPortEdit->setReadOnly(false);

            ui->mainConnectButton->setEnabled(true);
            ui->mainConnectButton->setText("접속");

            ui->pageStackedWidget->setCurrentIndex(0);
        }
    });

    connect(subNetworkManager, &SubNetworkManager::connectionStatusChanged, this, [=](bool op){
        if (op == true) {
            ui->subIpEdit->setReadOnly(true);
            ui->subPortEdit->setReadOnly(true);

            ui->subConnectButton->setEnabled(false);
            ui->subConnectButton->setText("정상 연결");

//            ui->pageStackedWidget->setCurrentIndex(1);
            if (!ui->mainConnectButton->isEnabled())
                ui->pageStackedWidget->setCurrentIndex(1);
        } else if (op == false) {
            ui->subIpEdit->setReadOnly(false);
            ui->subPortEdit->setReadOnly(false);

            ui->subConnectButton->setEnabled(true);
            ui->subConnectButton->setText("접속");

            ui->pageStackedWidget->setCurrentIndex(0);
        }
    });

    // 메인 서버 네트워크 관련 이벤트
    connect(mainNetworkManager, SIGNAL(sendWaitList(int,QString)), patientManager, SLOT(receiveWaitList(int,QString)));             // 메인서버 연결 시, 기존 대기목록 리시브
    connect(mainNetworkManager, SIGNAL(sendWaitPatient(QStringList)), patientManager, SLOT(receiveWaitPatient(QStringList)));       // 촬영의뢰가 들어온 경우, 환자관리 매니저에게 전송
    connect(patientManager, SIGNAL(sendPid(QString)), mainNetworkManager, SLOT(requestPatientInfo(QString)));       // 환자준비 버튼을 누르면, pid를 서버로 전송하여 환자정보요청
    connect(mainNetworkManager, SIGNAL(sendPatientInfo(QStringList)), patientManager, SLOT(receivePatientInfo(QStringList)));         // 요청한 환자 정보를 받음
    connect(patientManager, SIGNAL(finishSignal(QString,QString)), mainNetworkManager, SLOT(endImagingProcess(QString,QString)));     // 촬영프로세스완료 버튼 클릭 시, 소켓 전송
    connect(imagingManager, SIGNAL(saveSignal(QString)), mainNetworkManager, SLOT(sendFile(QString)));              // 저장버튼 클릭 시, 파일 전송

    // 서브 서버 네트워크 관련 이벤트
    connect(controlPanel, SIGNAL(buttonSignal(int,QString)), subNetworkManager, SLOT(sendButtonControl(int,QString)));      // 제어명령 패킷 송신
    connect(subNetworkManager, SIGNAL(buttonSignal(int)), controlPanel, SLOT(receiveButtonControl(int)));                   // 제어명령 패킷 수신
    connect(subNetworkManager, SIGNAL(sendFrameImg(int)), imagingManager, SLOT(recvFrameImg(int)));                         // 프레임 데이터 수신

    // 제어명령 관련 이벤트
    connect(patientManager, SIGNAL(sendType(QString)), controlPanel, SLOT(checkTypeButton(QString)));               // 촬영타입에 따른 버튼 조작
    connect(patientManager, SIGNAL(sendPidToImagingManager(QString)), imagingManager, SLOT(setPID(QString)));       // 촬영준비 시, 이미징클래스에 현재환자번호 저장
    connect(controlPanel, SIGNAL(readySignal(QString)), imagingManager, SLOT(setType(QString)));                    // 촬영준비 시, 이미징클래스에 타입 전송
    connect(controlPanel, SIGNAL(startSignal(QString,QString)), imagingManager, SLOT(startSetting(QString,QString)));           // start버튼 클릭 시, 이미징클래스에서 촬영 준비(쓰레드)
    connect(controlPanel, SIGNAL(stopSignal()), imagingManager, SLOT(stopButtonSlot()));                            // 촬영중단 시, 이미징클래스에서 스레드 종료
    connect(imagingManager, SIGNAL(saveSignal(QString)), patientManager, SLOT(saveSlot(QString)));                  // 저장버튼 클릭 시, 환자클래스에 노티
    connect(imagingManager, SIGNAL(saveSignal(QString)), controlPanel, SLOT(saveSlot(QString)));                    // 저장버튼 클릭 시, 버튼 셋팅
    connect(imagingManager, SIGNAL(shootingEndSignal(QString)), controlPanel, SLOT(shootingEndSlot(QString)));      // PANO,CEPH완료 시 , 버튼 활성화 컨트롤
    connect(patientManager, SIGNAL(finishSignal(QString,QString)), controlPanel, SLOT(finishSlot(QString,QString)));                // 촬영프로세스완료 시, 버튼 셋팅 리셋

    connect(ui->quitButton, SIGNAL(clicked()), qApp, SLOT(quit()));         // 프로그램 종료버튼
    connect(ui->quitButton_main, SIGNAL(clicked()), qApp, SLOT(quit()));


//    ui->pageStackedWidget->setCurrentIndex(1);

//    QStringList dataList;
//    dataList << "P00001" << "김유선" << "BOTH";
//    patientManager->receiveWaitPatient(dataList);
//    dataList.clear();
//    dataList << "P00004" << "김도예" << "BOTH";
//    patientManager->receiveWaitPatient(dataList);
//    dataList.clear();
//    dataList << "P00005" << "김영희" << "BOTH";
//    patientManager->receiveWaitPatient(dataList);

//    QStringList test1;
//    test1 << "P00001" << "김유선" << "여성" << "1999-00-00";
//    patientManager->receivePatientInfo(test1);
}

MainWindow::~MainWindow()
{
    delete ui;
    delete patientManager;
    delete controlPanel;
    delete imagingManager;
    delete mainNetworkManager;
    delete subNetworkManager;
}

void MainWindow::settingStyleSheet()
{
    ui->mainConnectButton->setStyleSheet("QPushButton {"
                                         "border-radius:10px;"
                                         "background-color: rgb(170,170,170);"
                                         "}"
                                         "QPushButton:hover {"
                                         "background-color: #ED8817;"
                                         "color:white;"
                                         "border-radius:10px;"
                                         "}"
                                         "QPushButton:disabled {"
                                         "border-radius:10px;"
                                         "background-color: #ED8817;"
                                         "color: white;"
                                         "}");

    ui->subConnectButton->setStyleSheet("QPushButton {"
                                        "border-radius:10px;"
                                        "background-color: rgb(170,170,170);"
                                        "}"
                                        "QPushButton:hover {"
                                        "background-color: #ED8817;"
                                        "color:white;"
                                        "border-radius:10px;"
                                        "}"
                                        "QPushButton:disabled {"
                                        "border-radius:10px;"
                                        "background-color: #ED8817;"
                                        "color: white;"
                                        "}");

    ui->quitButton->setStyleSheet("QPushButton {"
                                  "background-color: rgb(99, 99, 99);"
                                  "border-radius:10px;"
                                  "color: rgb(255, 255, 255);"
                                  "}"
                                  "QPushButton:hover {"
                                  "background-color: rgb(158, 8, 3);"
                                  "border-radius:10px;"
                                  "}");

    ui->quitButton_main->setStyleSheet("QPushButton {"
                                  "background-color: rgb(99, 99, 99);"
                                  "border-radius:10px;"
                                  "color: rgb(255, 255, 255);"
                                  "}"
                                  "QPushButton:hover {"
                                  "background-color: rgb(158, 8, 3);"
                                  "border-radius:10px;"
                                  "}");
}
