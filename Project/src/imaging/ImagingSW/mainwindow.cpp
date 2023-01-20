/*
 * 프로그램명 : ImagingSW
 * 파일명 : mainwindow.cpp
 * 설명 : 촬영SW의 메인GUI
 * 작성자 : 안다미로
 * 최종 수정일 : 2023.01.09
 */

#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QBoxLayout>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

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

    connect(controlPanel, SIGNAL(buttonSignal(int, QString)), subNetworkManager, SLOT(sendButtonControl(int, QString)));   // 제어명령 전송을 위해 네트워크매니저로 시그널 전송
    connect(subNetworkManager, SIGNAL(buttonSignal(int)), controlPanel, SLOT(receiveButtonControl(int)));   // 장비로부터 제어명령이 들어왔을 경우, 버튼 제어
    connect(mainNetworkManager, SIGNAL(sendWaitPatient(QStringList)), patientManager, SLOT(receiveWaitPatient(QStringList)));   // 촬영의뢰가 들어온 경우, 환자관리 매니저에게 전송
    connect(patientManager, SIGNAL(sendPid(QString)), mainNetworkManager, SLOT(requestPatientInfo(QString)));   // 환자준비 버튼을 누르면, pid를 서버로 전송하여 환자정보요청
    connect(mainNetworkManager, SIGNAL(sendPatientInfo(QStringList)), patientManager, SLOT(receivePatientInfo(QStringList)));   // 요청한 환자 정보를 받음
    connect(patientManager, SIGNAL(sendType(QString)), controlPanel, SLOT(checkTypeButton(QString)));   // 촬영요청 타입확인 후 , 촬영타입에 따라서 타입버튼 조작
    connect(patientManager, SIGNAL(sendPidToImagingManager(QString)), imagingManager, SLOT(setPID(QString)));   // 촬영준비 시, 이미징클래스에 현재환자번호 저장
    connect(controlPanel, SIGNAL(readySignal(QString)), imagingManager, SLOT(setType(QString)));        // ready버튼 클릭 시, 이미징클래스에 타입 전송
    connect(controlPanel, SIGNAL(startSignal()), imagingManager, SLOT(loadImage()));            // 촬영시작 버튼 클릭 시, 이미지 로드 시작
    connect(imagingManager, SIGNAL(saveSignal(QString)), mainNetworkManager, SLOT(endImagingProcess(QString)));     // 촬영종료 시, 소켓 전송
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
