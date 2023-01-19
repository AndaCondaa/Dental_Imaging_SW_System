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



    //PMS에서 촬영 의뢰를 눌렀을 시 Viewer의 대기 환자 리스트에서도 환자의 진행 상황을 "촬영중"으로 변경
    connect(m_networkManager, SIGNAL(sendPMSCameraPatient(QString, QString)), m_patientInfo, SLOT(receivePMSCameraPatient(QString, QString)));

    connect(m_networkManager, SIGNAL(sendPhotoEnd(QString)), m_patientInfo, SLOT(receivePhotoEnd(QString)));

    connect(m_patientInfo, SIGNAL(sendPatientInfo(QString, QString)), m_imageAlbum, SLOT(receivePatientInfo(QString, QString)));


}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::ViewerOpen(QString, QString)
{
    ui->stackedWidget->setCurrentIndex(1);
}

