#include "mainwindow.h"
#include "qobjectdefs.h"
#include "ui_mainwindow.h"
#include "imagealbum.h"
#include "patientinfo.h"
#include "networkmanager.h"
#include "loginform.h"

#include <QFileDialog>
#include <QColorSpace>
#include <QMessageBox>
#include <QImageReader>
#include <QMdiSubWindow>
#include <QVBoxLayout>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->showMaximized();

    m_patientInfo = new PatientInfo(this);
    m_imageAlbum = new ImageAlbum(this);
    m_networkManager = new NetworkManager(this);
    m_logIn = new LoginForm(this);

    QVBoxLayout *PatientLayout = new QVBoxLayout();
    PatientLayout->addWidget(m_patientInfo);
    ui->frame->setLayout(PatientLayout);

    QVBoxLayout *AlbumLayout = new QVBoxLayout();
    AlbumLayout->addWidget(m_imageAlbum);
    ui->frame_2->setLayout(AlbumLayout);

    QVBoxLayout *LoginLayout = new QVBoxLayout();
    LoginLayout->addWidget(m_logIn);
    ui->frame_3->setLayout(LoginLayout);


    //로그인 창에서 아이디/이름 입력 시 해당 의사의 정보를 확인하기 위해 서버로 로그인 데이터 전송
    connect(m_logIn, SIGNAL(SendDoctorCheck(QString)), m_networkManager, SLOT(newDataSended(QString)));
    connect(m_networkManager, SIGNAL(sendLogInCheck(QString)), m_logIn, SLOT(receiveLogInCheck(QString)));

    //로그인 성공 시 메인 윈도우의 로그인 창에서 뷰어 창으로 변경되도록 하기 위한 시그널-슬롯
    connect(m_logIn, SIGNAL(LogInSuccess(QString, QString)), this, SLOT(ViewerOpen(QString, QString)));

    //로그인 성공 시 뷰어 창에서 담당 의사 명이 자동으로 설정되도록 하기 위한 시그널-슬롯
    connect(m_logIn, SIGNAL(LogInSuccess(QString, QString)), m_patientInfo, SLOT(receiveDoctorName(QString, QString)));

    //로그인 성공 시 처방전 창에서 담당 의사 명이 자동으로 설정되도록 하기 위한 시그널-슬롯
    connect(m_logIn, SIGNAL(LogInSuccess(QString, QString)), m_imageAlbum, SLOT(receiveDoctorInfo(QString, QString)));

    connect(m_networkManager, SIGNAL(sendWaitingList(QString, QString)), m_patientInfo, SLOT(receiveWaitingList(QString, QString)));
    connect(m_patientInfo, SIGNAL(sendWaitingPatient(QString)), m_networkManager, SLOT(newDataSended(QString)));

    connect(m_networkManager, SIGNAL(sendSelectPatient(QString, QString)), m_patientInfo, SLOT(receiveSelectPatient(QString, QString)));
    connect(m_patientInfo, SIGNAL(sendCameraPatient(QString)), m_networkManager, SLOT(newDataSended(QString)));

    //뷰어 SW 로그인 시 기존에 추가되어있던 대기 리스트 환자 목록을 받아와 띄워주기 위한 시그널-슬롯
    connect(m_networkManager, SIGNAL(sendWaitTreatment(int, QString)), m_patientInfo, SLOT(receiveWaitTreatment(int, QString)));

    //진료 시작 버튼 클릭 시 리스트 위젯에 해당 환자의 이미지 업로드
    connect(m_networkManager, SIGNAL(sendImageFile()), m_imageAlbum, SLOT(reloadImages()));

    //PMS에서 촬영 의뢰를 눌렀을 시 Viewer의 대기 환자 리스트에서도 환자의 진행 상황을 "촬영중"으로 변경
    connect(m_networkManager, SIGNAL(sendPMSCameraPatient(QString, QString)), m_patientInfo, SLOT(receivePMSCameraPatient(QString, QString)));

    //촬영 SW에서 촬영 완료 신호가 오면 환자 클래스에서 해당 환자의 진행 상황을 촬영중->대기중으로 변경
    connect(m_networkManager, SIGNAL(sendPhotoEnd(QString)), m_patientInfo, SLOT(receivePhotoEnd(QString)));

    //서버에서 환자의 다중 이미지 파일의 수신이 완료되었다는 신호가 오면 환자 관리 클래스에서 촬영 의뢰를 하도록 설정
    connect(m_networkManager, SIGNAL(sendAllImageFile(bool)), m_patientInfo, SLOT(receiveAllImageFileP(bool)));

    //서버에서 환자의 다중 이미지 파일의 수신이 완료되었다는 신호가 오면 진료 종료를 하도록 설정
    connect(m_networkManager, SIGNAL(sendAllImageFile(bool)), m_imageAlbum, SLOT(receiveAllImageFileA(bool)));

    //환자 정보 클래스에서 대기 리스트 내에서 해당 환자 클릭된 후 진료 시작을 눌렀을 때 처방전 작성 폼에 띄워지도록 하기 위한 시그널-슬롯
    connect(m_patientInfo, SIGNAL(sendPatientInfo(QString, QString, QString)), m_imageAlbum, SLOT(receivePatientInfo(QString, QString, QString)));

    //환자 정보 클래스에서 촬영 의뢰 버튼 클릭 시 이미지 listWidget에 있는 사진들 삭제시켜주기 위한 시그널-슬롯
    connect(m_patientInfo, SIGNAL(sendCameraStart()), m_imageAlbum, SLOT(receiveCameraStart()));

    //처방전 창에서 작성 완료 버튼 클릭 시 서버로 처방전의 내용을 전송하기 위한 시그널-슬롯
    connect(m_imageAlbum, SIGNAL(sendPrescriptiontoServer(QString)), m_networkManager, SLOT(newDataSended(QString)));

    //진료 종료 클릭 시 해당 환자의 정보를 서버로 전송하기 위한 시그널-슬롯
    connect(m_imageAlbum, SIGNAL(sendEndTreatment(QString)), m_networkManager, SLOT(newDataSended(QString)));
    connect(m_imageAlbum, SIGNAL(sendEndTreatment(QString)), m_patientInfo, SLOT(receiveEndTreatment(QString)));
    connect(m_imageAlbum, SIGNAL(sendEndSignal()), m_patientInfo, SLOT(receiveEndSignal()));

    //프린트 버튼 클릭 시 ImageAlbum 클래스에서 이미지 출력 기능 수행
    connect(this, SIGNAL(sendPrintStart()), m_imageAlbum, SLOT(receivePrintStart()));

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::ViewerOpen(QString, QString)
{
    ui->stackedWidget->setCurrentIndex(1);
}

//프린트 버튼 클릭 시 ImageAlbum 클래스로 시그널 전송
void MainWindow::on_actionPrinter_triggered()
{
    emit sendPrintStart();
}

