#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "imagealbum.h"
#include "patientinfo.h"

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

    QVBoxLayout *PatientLayout = new QVBoxLayout();
    PatientLayout->addWidget(m_patientInfo);
    ui->frame->setLayout(PatientLayout);  

    QVBoxLayout *AlbumLayout = new QVBoxLayout();
    AlbumLayout->addWidget(m_imageAlbum);
    ui->frame_2->setLayout(AlbumLayout);
}

MainWindow::~MainWindow()
{
    delete ui;
}


