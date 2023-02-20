/*프로그램명 : ImageViewerSW
파일명 : mainwindow.cpp
설명 : 각 클래스들 간의 signal-slot을 연결해주는 connect문을 작성한 부분
stackedWidget을 통해 login창과 viewer창으로 구분되어 있는 클래스
작성자 : 이정연
최종 수정 날짜 : 2023.02.11*/


#include "mainwindow.h"
#include "qobjectdefs.h"
#include "ui_mainwindow.h"
#include "imagealbum.h"
#include "patientinfo.h"
#include "networkmanager.h"
#include "loginform.h"

#include <QFileDialog>
#include <QColorSpace>
#include <QImageReader>
#include <QMdiSubWindow>
#include <QVBoxLayout>

#include <QtTest/QtTest>
#include <QTest>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->stackedWidget->setCurrentIndex(0);
    ui->toolBar->hide();
    this->showMaximized();

    // 클래스 객체 생성
    m_patientInfo = new PatientInfo(this);
    m_imageAlbum = new ImageAlbum(this);
    m_networkManager = new NetworkManager(this);
    m_logIn = new LoginForm(this);

//    QTest::qExec(m_patientInfo);
//    QTest::qExec(m_imageAlbum);
//    QTest::qExec(m_logIn);
//    QTest::qExec(m_networkManager);

    // frame에 환자 관리 클래스, ImageAlbum 클래스, 로그인 클래스를 올려줌
    QVBoxLayout *PatientLayout = new QVBoxLayout();
    PatientLayout->addWidget(m_patientInfo);
    ui->frame->setLayout(PatientLayout);

    QVBoxLayout *AlbumLayout = new QVBoxLayout();
    AlbumLayout->addWidget(m_imageAlbum);
    ui->frame_2->setLayout(AlbumLayout);

    QVBoxLayout *LoginLayout = new QVBoxLayout();
    LoginLayout->addWidget(m_logIn);
    ui->frame_3->setLayout(LoginLayout);
    m_logIn->showMaximized();

    // 재접속 버튼 클릭 시, 소켓 생성 및 서버와 연결
    connect(m_logIn, SIGNAL(reconnect(QString,int)), m_networkManager, SLOT(receiveLoginToServer(QString,int)));

    // 서버와 성공적으로 연결되면 재접속 버튼 비활성화, 로그인 버튼 활성화
    connect(m_networkManager, SIGNAL(serverConnection(int)), m_logIn, SLOT(serverConnected(int)));

    // 서버와 연결 종료 시 대기 환자 리스트 클리어
    connect(m_networkManager, SIGNAL(disconnectedToServer(int)), m_patientInfo, SLOT(clearWaitingList(int)));

    // 서버와 연결 종료 시 재접속 버튼 활성화, 로그인 버튼 비활성화, 로그인 창으로 변경
    connect(m_networkManager, SIGNAL(disconnectedToServer(int)), ui->stackedWidget, SLOT(setCurrentIndex(int)));

    // 서버와 연결 종료 시 프린터 버튼 숨겨줌
    connect(m_networkManager, SIGNAL(disconnectedToServer(int)), ui->toolBar, SLOT(hide()));

    // 로그인 창에서 아이디/이름 입력 시 해당 의사의 정보를 확인하기 위해 서버로 로그인 데이터 전송
    connect(m_logIn, SIGNAL(SendDoctorCheck(QString)), m_networkManager, SLOT(newDataSended(QString)));

    // 서버로부터 해당 의사 정보로 로그인 성공 유무를 LogIn 클래스에 알려주기 위한 시그널-슬롯
    connect(m_networkManager, SIGNAL(sendLogInCheck(QString)), m_logIn, SLOT(receiveLogInCheck(QString)));

    // 로그인 성공 시 메인 윈도우의 로그인 창에서 뷰어 창으로 변경되도록 하기 위한 시그널-슬롯
    connect(m_logIn, SIGNAL(LogInSuccess(QString, QString)), this, SLOT(ViewerOpen(QString, QString)));

    // 로그인 성공 시 뷰어 창에서 담당 의사 명이 자동으로 설정되도록 하기 위한 시그널-슬롯
    connect(m_logIn, SIGNAL(LogInSuccess(QString, QString)), m_patientInfo, SLOT(receiveDoctorName(QString, QString)));

    // 로그인 성공 시 처방전 창에서 담당 의사 명이 자동으로 설정되도록 하기 위한 시그널-슬롯
    connect(m_logIn, SIGNAL(LogInSuccess(QString, QString)), m_imageAlbum, SLOT(receiveDoctorInfo(QString, QString)));

    // PMS에서 대기 환자 추가 시 뷰어에도 해당 환자 추가되도록 하기 위한 시그널-슬롯
    connect(m_networkManager, SIGNAL(sendWaitingList(QString, QString)), m_patientInfo, SLOT(receiveWaitingList(QString, QString)));

    // 대기 리스트의 환자 중 선택된 환자의 진료 시작 시 서버에도 해당 환자의 정보를 보내주기 위한 시그널 슬롯
    connect(m_patientInfo, SIGNAL(sendWaitingPatient(QString)), m_networkManager, SLOT(newDataSended(QString)));

    // 대기리스트에서 선택되어 진료 시작한 환자의 정보를 tableWidget에 띄워주기 위한 시그널-슬롯
    connect(m_networkManager, SIGNAL(sendSelectPatient(QString, QString)), m_patientInfo, SLOT(receiveSelectPatient(QString, QString)));

    // 진료 시작한 환자의 촬영 의뢰를 요청 시 서버에도 해당 환자의 정보 알려주기 위한 시그널-슬롯
    connect(m_patientInfo, SIGNAL(sendCameraPatient(QString)), m_networkManager, SLOT(newDataSended(QString)));

    // 촬영 SW가 작동하지 않아 해당 환자가 촬영을 하지 못했을 때 해당 환자 정보를 서버로 전송
    connect(m_patientInfo, SIGNAL(sendCameraDelay(QString)), m_networkManager, SLOT(newDataSended(QString)));

    // 뷰어 SW 로그인 시 기존에 추가되어있던 대기 리스트 환자 목록을 받아와 띄워주기 위한 시그널-슬롯
    connect(m_networkManager, SIGNAL(sendWaitTreatment(int, QString)), m_patientInfo, SLOT(receiveWaitTreatment(int, QString)));

    // 진료 시작 버튼 클릭 시 리스트 위젯에 해당 환자의 이미지 업로드
    connect(m_networkManager, SIGNAL(sendImageFile()), m_imageAlbum, SLOT(reloadImages()));

    // PMS에서 촬영 의뢰를 눌렀을 시 Viewer의 대기 환자 리스트에서도 환자의 진행 상황을 "촬영중"으로 변경
    connect(m_networkManager, SIGNAL(sendPMSCameraPatient(QString, QString)), m_patientInfo, SLOT(receivePMSCameraPatient(QString, QString)));

    // 촬영 SW에서 촬영 완료 신호가 오면 환자 클래스에서 해당 환자의 진행 상황을 촬영중->대기중으로 변경
    connect(m_networkManager, SIGNAL(sendPhotoEnd(QString)), m_patientInfo, SLOT(receivePhotoEnd(QString)));

    // 서버에서 환자의 다중 이미지 파일의 수신이 완료되었다는 신호가 오면 환자 관리 클래스에서 촬영 의뢰를 하도록 설정
    connect(m_networkManager, SIGNAL(sendAllImageFile(bool)), m_patientInfo, SLOT(receiveAllImageFileP(bool)));

    // 서버에서 환자의 다중 이미지 파일의 수신이 완료되었다는 신호가 오면 진료 종료를 하도록 설정
    connect(m_networkManager, SIGNAL(sendAllImageFile(bool)), m_imageAlbum, SLOT(receiveAllImageFileA(bool)));

    // 환자 정보 클래스에서 대기 리스트 내에서 해당 환자 클릭된 후 진료 시작을 눌렀을 때 처방전 작성 폼에 띄워지도록 하기 위한 시그널-슬롯
    connect(m_patientInfo, SIGNAL(sendPatientInfo(QString, QString, QString)), m_imageAlbum, SLOT(receivePatientInfo(QString, QString, QString)));

    // 환자 정보 클래스에서 촬영 의뢰 버튼 클릭 시 이미지 listWidget에 있는 사진들 삭제시켜주기 위한 시그널-슬롯
    connect(m_patientInfo, SIGNAL(sendCameraStart()), m_imageAlbum, SLOT(receiveCameraStart()));

    // 처방전 창에서 작성 완료 버튼 클릭 시 서버로 처방전의 내용을 전송하기 위한 시그널-슬롯
    connect(m_imageAlbum, SIGNAL(sendPrescriptiontoServer(QString)), m_networkManager, SLOT(newDataSended(QString)));

    // 진료 종료 클릭 시 해당 환자의 정보를 서버로 전송하기 위한 시그널-슬롯
    connect(m_imageAlbum, SIGNAL(sendEndTreatment(QString)), m_networkManager, SLOT(newDataSended(QString)));

    // 진료 종료 버튼 클릭 시 해당 환자의 정보 삭제, 대기 리스트에서도 삭제를 해주기 위한 시그널-슬롯
    connect(m_imageAlbum, SIGNAL(sendEndTreatment(QString)), m_patientInfo, SLOT(receiveEndTreatment(QString)));

    // 진료 종료 버튼 클릭 시 진료 시작 버튼 활성화, 촬영 의뢰 버튼 비활성화
    connect(m_imageAlbum, SIGNAL(sendEndSignal()), m_patientInfo, SLOT(receiveEndSignal()));

    // 프린트 버튼 클릭 시 ImageAlbum 클래스에서 이미지 출력 기능 수행
    connect(this, SIGNAL(sendPrintStart()), m_imageAlbum, SLOT(receivePrintStart()));

    // 진료 종료 버튼 클릭 시 서버에서 완료 신호가 오면 환자 관리 클래스로 해당 환자의 진료 종료를 수행
    connect(m_networkManager, SIGNAL(sendPatientTreatmentEnd()), m_imageAlbum, SLOT(receivePatientTreatmentEnd()));

    // 처방전 작성 완료 버튼 클릭 시 DB에서 저장 성공 여부를 확인 후 처방전 클래스로 전송
    connect(m_networkManager, SIGNAL(sendPrescriptionCheck(QString)), m_imageAlbum, SLOT(receivePrescriptionCheck(QString)));
}

MainWindow::~MainWindow()
{
    delete ui;
}

// 로그인이 성공하면 뷰어 프로그램 창으로 widget이 변경
void MainWindow::ViewerOpen(QString, QString)
{
    ui->stackedWidget->setCurrentIndex(1);
    ui->toolBar->show();
}

// 프린트 버튼 클릭 시 ImageAlbum 클래스로 시그널 전송
void MainWindow::on_actionPrinter_triggered()
{
    emit sendPrintStart();
}

