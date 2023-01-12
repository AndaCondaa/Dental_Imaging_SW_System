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
    networkManager = new NetworkManager(this);

    QVBoxLayout *patientLayout = new QVBoxLayout();
    QVBoxLayout *controlLayout = new QVBoxLayout();
    QVBoxLayout *imagingLayout = new QVBoxLayout();

    patientLayout->addWidget(patientManager);
    controlLayout->addWidget(controlPanel);
    imagingLayout->addWidget(imagingManager);

    ui->patientFrame->setLayout(patientLayout);
    ui->controlFrame->setLayout(controlLayout);
    ui->imagingFrame->setLayout(imagingLayout);

    connect(controlPanel, SIGNAL(buttonSignal(int)), networkManager, SLOT(receiveButtonControl(int)));
    connect(networkManager, SIGNAL(buttonSignal(int)), controlPanel, SLOT(receiveButtonControl(int)));
}

MainWindow::~MainWindow()
{
    delete ui;
    delete patientManager;
    delete controlPanel;
    delete imagingManager;
    delete networkManager;
}
