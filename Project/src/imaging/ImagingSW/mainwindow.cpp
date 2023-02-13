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

    connect(this, SIGNAL(connectMainServer(QString,int)), mainNetworkManager, SLOT(connectSever(QString,int)));
    connect(this, SIGNAL(connectSubServer(QString,int)), subNetworkManager, SLOT(connectServer(QString,int)));

    connect(ui->mainConnectButton, &QPushButton::clicked, this, [=](){                                      // 메인서버 연결 요청
        emit connectMainServer(ui->mainIpEdit->text(), ui->mainPortEdit->text().toInt());
    });
    connect(ui->subConnectButton, &QPushButton::clicked, this, [=]() {
        emit connectSubServer(ui->subIpEdit->text(), ui->subPortEdit->text().toInt());
    });

    connect(mainNetworkManager, &MainNetworkManager::connectionStatusChanged, this, [=](bool op){           // 메인서버 연결상태 변경
        if (op == true) {
            ui->mainIpEdit->setReadOnly(true);
            ui->mainPortEdit->setReadOnly(true);

            ui->mainConnectButton->setEnabled(false);
            ui->mainConnectButton->setText("정상연결");

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
            ui->subConnectButton->setText("정상연결");

            ui->pageStackedWidget->setCurrentIndex(1);
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

    connect(mainNetworkManager, SIGNAL(sendWaitList(int,QString)), patientManager, SLOT(receiveWaitList(int,QString)));     // 메인서버 연결 시, 기존 대기목록 리시브
    connect(controlPanel, SIGNAL(buttonSignal(int,QString)), subNetworkManager, SLOT(sendButtonControl(int,QString)));   // 제어명령 전송을 위해 네트워크매니저로 시그널 전송
    connect(subNetworkManager, SIGNAL(buttonSignal(int)), controlPanel, SLOT(receiveButtonControl(int)));   // 장비로부터 제어명령이 들어왔을 경우, 버튼 제어
    connect(mainNetworkManager, SIGNAL(sendWaitPatient(QStringList)), patientManager, SLOT(receiveWaitPatient(QStringList)));   // 촬영의뢰가 들어온 경우, 환자관리 매니저에게 전송
    connect(patientManager, SIGNAL(sendPid(QString)), mainNetworkManager, SLOT(requestPatientInfo(QString)));   // 환자준비 버튼을 누르면, pid를 서버로 전송하여 환자정보요청
    connect(mainNetworkManager, SIGNAL(sendPatientInfo(QStringList)), patientManager, SLOT(receivePatientInfo(QStringList)));   // 요청한 환자 정보를 받음
    connect(patientManager, SIGNAL(sendType(QString)), controlPanel, SLOT(checkTypeButton(QString)));   // 촬영요청 타입확인 후 , 촬영타입에 따라서 타입버튼 조작 (PID|TYPE)
    connect(patientManager, SIGNAL(sendPidToImagingManager(QString)), imagingManager, SLOT(setPID(QString)));   // 촬영준비 시, 이미징클래스에 현재환자번호 저장
    connect(controlPanel, SIGNAL(readySignal(QString)), imagingManager, SLOT(setType(QString)));        // ready버튼 클릭 시, 이미징클래스에 타입 전송
    connect(controlPanel, SIGNAL(startSignal(QString,QString)), imagingManager, SLOT(startSetting(QString,QString)));   // start버튼 클릭 시, 이미징클래스에서 촬영 준비(쓰레드)
    connect(controlPanel, SIGNAL(stopSignal()), imagingManager, SLOT(stopButtonSlot()));    // 촬영중단 시, 이미징클래스에서 스레드 종료
    connect(imagingManager, SIGNAL(saveSignal(QString)), patientManager, SLOT(saveSlot(QString)));            // 저장버튼 클릭 시, 환자클래스에 노티
    connect(imagingManager, SIGNAL(saveSignal(QString)), controlPanel, SLOT(saveSlot(QString)));            // 저장버튼 클릭 시, 버튼 셋팅
    connect(imagingManager, SIGNAL(saveSignal(QString)), mainNetworkManager, SLOT(sendFile(QString)));           // 저장버튼 클릭 시, 파일 전송
    connect(imagingManager, SIGNAL(shootingEndSignal(QString)), controlPanel, SLOT(shootingEndSlot(QString)));      // PANO,CEPH완료 시 , 버튼 활성화 컨트롤
    connect(patientManager, SIGNAL(finishSignal(QString,QString)), controlPanel, SLOT(finishSlot(QString,QString)));                // 촬영프로세스완료 시, 버튼 셋팅 리셋
    connect(patientManager, SIGNAL(finishSignal(QString,QString)), mainNetworkManager, SLOT(endImagingProcess(QString,QString)));     // 촬영프로세스완료 버튼 클릭 시, 소켓 전송
    connect(subNetworkManager, SIGNAL(sendFrameImg(int)), imagingManager, SLOT(recvFrameImg(int)));

    connect(ui->quitButton, SIGNAL(clicked()), qApp, SLOT(quit()));

//    ui->pageStackedWidget->setCurrentIndex(1);

    QStringList dataList;
    dataList << "P00001" << "김유선" << "BOTH";
    patientManager->receiveWaitPatient(dataList);
//    dataList.clear();
//    dataList << "P00004" << "김도예" << "BOTH";
//    patientManager->receiveWaitPatient(dataList);
//    dataList.clear();
//    dataList << "P00005" << "김영희" << "BOTH";
//    patientManager->receiveWaitPatient(dataList);

    QStringList test1;
    test1 << "P00001" << "김유선" << "F" << "asdf";
    patientManager->receivePatientInfo(test1);
//    QStringList test2;
//    test2 << "P00002" << "김도예" << "asdf" << "asdf";
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
