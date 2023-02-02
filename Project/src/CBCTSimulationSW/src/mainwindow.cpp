#include "mainwindow.h"
#include "ui_mainwindow.h"
//#include "cbctfiletransfer.h"
//#include "cbctlogthread.h"
//#include "cbctrawimageviewer.h"
#include <QMessageBox>
#include <QFile>
#include <QGraphicsPixmapItem>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QList>
#include <QTransform>
#include <qstring.h>
#include <qevent.h>
#include <QThread>
/* 3D .Obj Visualization */

#include <vtkRenderWindow.h>
#include<vtkOBJExporter.h>
#include <vtkPlaneSource.h>
#include <vtkAxesActor.h>
#include <vtkOBJImporter.h>
#include <vtkObject.h>
#include <vtkOBJReader.h>


#include <vtkNamedColors.h>
#include <vtkProperty.h>
#include <vtkPointData.h>
#include <vtkImporter.h>

/* 3D .Obj Movement */
#include <vtkRenderWindowInteractor.h>
#include <vtkTransform.h>
#include <vtkTransformPolyDataFilter.h>


#include <qvtkopenglstereowidget.h>
#include <QVTKRenderWidget.h>
#include <QVTKOpenGLWindow.h>
#include <vtkCamera.h>
#include <vtkNew.h>

#include <qopenglwidget.h>

/* 2D Visualization */
#include <vtkImageReader2Factory.h>
#include <vtkImageReader2.h>
#include <vtkImageViewer2.h>
#include <vtkRenderWindow.h>
#include <InteractionContext.h>
#include <QDebug>

#include <QtNetwork>
//#define USE_DISPLAY_GLOBALAXIS

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    panoScene = new QGraphicsScene();
    cephScene = new QGraphicsScene();
    // Model Controller 생성.

    m_modelController = new CBCTModelController(ui);
    m_rawImageViewer = new CBCTRawImageViewer();
    m_fileTransfer = new CBCTFileTransfer(this);

    if (!m_modelController->initialize())
        qDebug() << "CBCTModelController initialize Fail ! ";

    connect(ui->AscendingPushButton, SIGNAL(clicked()), m_modelController, SLOT(on_AscendingPushButton_pressed()));
    connect(ui->DescendingPushButton, SIGNAL(clicked()), m_modelController, SLOT(on_DescendingPushButton_pressed()));
    connect(ui->MainPushButton, SIGNAL(clicked()), m_modelController, SLOT(on_MainPushButton_clicked()));
    connect(ui->SubPushButton, SIGNAL(clicked()), m_modelController, SLOT(on_SubPushButton_clicked()));

    //    connect(ui->CaptureResetPushButton, SIGNAL(clicked()), m_modelController, SLOT(on_CaptureResetPushButton_VTK_clicked()));
    //    connect(ui->CaptureReadyPushButton, SIGNAL(clicked()), m_modelController, SLOT(on_CaptureReadyPushButton_VTK_clicked()));

    /* 버튼 클릭시 network slot 함수를 연결*/
    //    connect(ui->CaptureResetPushButton, SIGNAL(clicked()), m_fileTransfer, SLOT(sendButtonControl(int, QString)));
    //    connect(ui->CaptureReadyPushButton, SIGNAL(clicked()), m_fileTransfer, SLOT(sendButtonControl(int, QString)));
    //    connect(ui->CaptureStartPushButton, SIGNAL(clicked()), m_fileTransfer, SLOT(sendButtonControl(int, QString)));
    //    connect(ui->CaptureStopPushButton, SIGNAL(clicked()), m_fileTransfer, SLOT(sendButtonControl(int, QString)));

    connect(ui->CaptureResetPushButton, SIGNAL(clicked()), m_fileTransfer, SLOT(sendingControl(int,QString)));
    connect(ui->CaptureReadyPushButton, SIGNAL(clicked()), m_fileTransfer, SLOT(sendingControl(int,QString)));
    connect(ui->CaptureStartPushButton, SIGNAL(clicked()), m_fileTransfer, SLOT(sendingControl(int,QString)));
    connect(ui->CaptureStopPushButton, SIGNAL(clicked()), m_fileTransfer, SLOT(sendingControl(int,QString)));


    /* 버튼 클릭시 에밋을 위한 함수를 동작*/
    connect(ui->CaptureResetPushButton, SIGNAL(clicked()), this, SLOT(emitResetSignal()));
    connect(ui->CaptureReadyPushButton, SIGNAL(clicked()), this, SLOT(emitReadySignal()));
    connect(ui->CaptureStartPushButton, SIGNAL(clicked()), this, SLOT(emitStartSignal()));
    connect(ui->CaptureStopPushButton, SIGNAL(clicked()), this, SLOT(emitStopSignal()));

    /* 버튼 클릭시 기본 기능 동작*/
    connect(ui->CaptureResetPushButton, SIGNAL(clicked()), this, SLOT(on_CaptureResetPushButton_clicked()));
    connect(ui->CaptureReadyPushButton, SIGNAL(clicked()), this, SLOT(on_CaptureReadyPushButton_clicked()));
    connect(ui->CaptureStartPushButton, SIGNAL(clicked()), this, SLOT(on_CaptureStartPushButton_clicked()));
    connect(ui->CaptureStopPushButton, SIGNAL(clicked()), this, SLOT(on_CaptureStopPushButton_clicked()));


    connect(m_rawImageViewer, SIGNAL(signals_panoImage(QImage*)), this, SLOT(slot_panoImage(QImage*)));
    connect(m_rawImageViewer, SIGNAL(signals_cephImage(QImage*)), this, SLOT(slot_cephImage(QImage*)));


//    connect(ui->CaptureReadyPushButton, SIGNAL(clicked), m_fileTransfer, SLOT());
//    connect(ui->CaptureResetPushButton, SIGNAL(clicked), m_fileTransfer, SLOT());
//    connect(ui->CaptureStartPushButton, SIGNAL(clicked), m_fileTransfer, SLOT());
//    connect(ui->CaptureStartPushButton, SIGNAL(clicked), m_fileTransfer, SLOT());

    /* 촬영 SW에서 Signal 받았을 시 Emit Signal 제외한 기능 동작 */
    connect(m_fileTransfer, SIGNAL(resetSignal()), this, SLOT(on_CaptureResetPushButton_clicked()));
    connect(m_fileTransfer, SIGNAL(readySignal()), this, SLOT(on_CaptureReadyPushButton_clicked()));
    connect(m_fileTransfer, SIGNAL(startSignal()), this, SLOT(on_CaptureStartPushButton_clicked()));
    connect(m_fileTransfer, SIGNAL(stopSignal()), this, SLOT(on_CaptureStopPushButton_clicked()));

    /* 촬영 SW에서 Signal 받았을 시 Modality CheckBox 기능 동작 */
    connect(m_fileTransfer, SIGNAL(panoSignal()), this, SLOT(receive_Pano_Modality()));
    connect(m_fileTransfer, SIGNAL(cephSignal()), this, SLOT(receive_Ceph_Modality()));

    // ui check box Update
    connect(ui->PanoCheckBox, &QCheckBox::clicked, this, [&](bool state) {
        if (state == true)
        {
            if (ui->CephCheckBox->isChecked())
            {
                ui->CephCheckBox->setCheckState(Qt::Unchecked);
            }
        }
    });
    connect(ui->CephCheckBox, &QCheckBox::clicked, this, [&](bool state) {
        if (state == true)
        {
            if (ui->PanoCheckBox->isChecked())
            {
                ui->PanoCheckBox->setCheckState(Qt::Unchecked);
            }
        }
    });

    //ProgressBar 동작을 모션과 연결시켜 준다
    connect(ui->PanoProgressBar, &QProgressBar::valueChanged, this, [&](int value) {
        m_modelController->on_Rotate_PanoObject(value);
    });
    connect(ui->CephProgressBar, &QProgressBar::valueChanged, this, [&](int value) {
        m_modelController->on_Translate_CephObject(value);
    });
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::resizeEvent(QResizeEvent* event)
{
    auto Size = event->size();

    //ui->openGLWidget_All->resize(Size);
    QMainWindow::resizeEvent(event);
}

void MainWindow::receive_Pano_Modality()
{
    ui->PanoCheckBox->setCheckState(Qt::Unchecked);
    ui->CephCheckBox->setCheckState(Qt::Unchecked);
    ui->PanoCheckBox->setChecked(true);
}


void MainWindow::receive_Ceph_Modality()
{
    ui->PanoCheckBox->setCheckState(Qt::Unchecked);
    ui->CephCheckBox->setCheckState(Qt::Unchecked);
    ui->CephCheckBox->setChecked(true);
}

void MainWindow::emitResetSignal()
{
    //   m_mainWindow->on_CaptureResetPushButton_clicked();
//    if(ui->PanoCheckBox->isChecked())
//    {
//        m_fileTransfer->sendingControl(ControlType::RESET, "PANO");
//    }
//    else if(ui->CephCheckBox->isChecked())
//    {
        m_fileTransfer->sendingControl(ControlType::RESET, "RESET");
//    }
//    emit RESETSignal(ControlType::RESET);
}

void MainWindow::emitReadySignal()
{
    //   m_mainWindow->on_CaptureReadyPushButton_clicked();
//    if(ui->PanoCheckBox->isChecked())
//    {
//        m_fileTransfer->sendingControl(ControlType::READY, "PANO");
//    }
//    else if(ui->CephCheckBox->isChecked())
//    {
        m_fileTransfer->sendingControl(ControlType::READY, "READY");
//    }
//    emit READYSignal(ControlType::READY);

}
//current type QString,
void MainWindow::emitStartSignal()
{
    //   m_mainWindow->on_CaptureStartPushButton_clicked();
    if(ui->PanoCheckBox->isChecked())
    {
        m_fileTransfer->sendingControl(ControlType::START, "PANO");
    }
    else if(ui->CephCheckBox->isChecked())
    {
        m_fileTransfer->sendingControl(ControlType::START, "CEPH ");
    }

    emit STARTSignal(ControlType::START);
}

void MainWindow::emitStopSignal()
{
    //    m_mainWindow->on_CaptureStopPushButton_clicked();
//    if(ui->PanoCheckBox->isChecked())
//    {
//        m_fileTransfer->sendingControl(ControlType::STOP, "PANO");
//    }
//    else if(ui->CephCheckBox->isChecked())
//    {
        m_fileTransfer->sendingControl(ControlType::STOP, "STOP");
//    }
//    emit STOPSignal(ControlType::STOP);


}
void MainWindow::on_CaptureResetPushButton_clicked()
{

    /* reset 버튼 클릭시 start, stop 버튼 비활성화 */
    ui->CaptureResetPushButton->setEnabled(true);
    ui->CaptureReadyPushButton->setEnabled(true);
    ui->CaptureStartPushButton->setEnabled(false);
    ui->CaptureStopPushButton->setEnabled(false);

    m_modelController->on_CaptureResetPushButton_VTK_clicked();
    m_rawImageViewer->resetPanoTimer();
    m_rawImageViewer->resetCephTimer();
    ui->PanoGraphicsView->resetTransform();
    ui->CephGraphicsView->resetTransform();
    panoScene->clear();
    cephScene->clear();
    ui->PanoProgressBar->reset();
    ui->CephProgressBar->reset();
    ui->PanoLabel->clear();
    ui->CephLabel->clear();



    if(ui->PanoCheckBox->isChecked())
    {
        m_fileTransfer->sendPanoFile();
    }
    else if(ui->CephCheckBox->isChecked())
    {
        m_fileTransfer->sendCephFile();
    }

}

void MainWindow::on_CaptureReadyPushButton_clicked()
{
    /* ready 버튼 클릭시 pressed 상태로 고정 후 start 버튼 활성화 */
    ui->CaptureReadyPushButton->setEnabled(false);
    ui->CaptureStartPushButton->setEnabled(true);

    m_modelController->on_CaptureReadyPushButton_VTK_clicked();
    m_rawImageViewer->readyPanoTimer();
    m_rawImageViewer->readyCephTimer();



}

void MainWindow::on_CaptureStartPushButton_clicked()
{
    /* start 버튼 클릭시 Stop 버튼만 활성화 */
    ui->CaptureResetPushButton->setEnabled(false);
    ui->CaptureStartPushButton->setEnabled(false);
    ui->CaptureStopPushButton->setEnabled(true);

    if (ui->PanoCheckBox->isChecked())
    {
        if (ui->CephCheckBox->isChecked())
        {
            m_panoErrorMessage = new QMessageBox;
m_panoErrorMessage:ERROR_LOG_POLICY_CONFLICT;
        }
        else
        {
            qDebug() << __FUNCTION__;
            m_rawImageViewer->startPanoTimer();
        }

        //CBCTRawImageViewer m_rawImageViewer;
        //QPixmap panoPix = m_rawImageViewer.PanoImageViewer();
        //ui->PanoLabel->setPixmap(panoPix);
    }

    if (ui->CephCheckBox->isChecked())
    {
        if (ui->PanoCheckBox->isChecked())
        {
            m_cephErrorMessage = new QMessageBox;
m_cephErrorMessage:ERROR_LOG_POLICY_CONFLICT;

        }
        else
        {
            qDebug() << __FUNCTION__;
            m_rawImageViewer->startCephTimer();
        }

        //        CBCTRawImageViewer m_rawImageViewer;
        //        QPixmap cephPix = m_rawImageViewer.CephImageViewer();
        //        m_rawImageViewer.CephImageViewer();
        //        ui->CephLabel->setPixmap(cephPix);

    }


}

void MainWindow::on_CaptureStopPushButton_clicked()
{
    /* stop 버튼 클릭시 reset 버튼만 활성화 */
    ui->CaptureResetPushButton->setEnabled(true);
    ui->CaptureReadyPushButton->setEnabled(false);
    ui->CaptureStartPushButton->setEnabled(false);
    ui->CaptureStopPushButton->setEnabled(false);

    ui->PanoCheckBox->setCheckState(Qt::Unchecked);
    ui->CephCheckBox->setCheckState(Qt::Unchecked);

    m_rawImageViewer->stopPanoTimer();
    m_rawImageViewer->stopCephTimer();


}

void MainWindow::slot_panoImage(QImage* pImg)
{
    qDebug() << __FUNCTION__;
    QImage pano_Image(*pImg);
    QPixmap panoPix;
    panoPix = QPixmap::fromImage(pano_Image, Qt::AutoColor);

    /* 파노라마 이미지가 90도 회전되어 있으므로, 출력시 원상복구한다 */
    QTransform panoTransform;
    panoTransform.rotate(90);
    panoScene->addPixmap(panoPix.transformed(panoTransform));
    ui->PanoLabel->setPixmap(panoPix.transformed(panoTransform));
    ui->PanoGraphicsView->setScene(panoScene);
    /* 파노라마 Raw Image 전송상태를 표시해주는 ProgressBar */
    int panoValue = ui->PanoProgressBar->value();
    panoValue++;
    ui->PanoProgressBar->setValue(panoValue);
}

void MainWindow::slot_cephImage(QImage* cImg)
{
    qDebug() << __FUNCTION__;
    QImage ceph_Image(*cImg);
    QPixmap cephPix;
    cephPix = QPixmap::fromImage(ceph_Image, Qt::AutoColor);
    cephScene->addPixmap(cephPix);
    ui->CephLabel->setPixmap(cephPix);
    ui->CephGraphicsView->setScene(cephScene);
    /* 세팔로 Raw Image 전송상태를 표시해주는 ProgressBar */
    int cephValue = ui->CephProgressBar->value();
    cephValue++;
    ui->CephProgressBar->setValue(cephValue);

}



void MainWindow::on_openGLWidget_All_resized()
{
    auto size = this->size();
    ui->openGLWidget_All->resize(size);
}

