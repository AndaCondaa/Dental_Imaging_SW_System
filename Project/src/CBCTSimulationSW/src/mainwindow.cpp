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


//#define USE_DISPLAY_GLOBALAXIS

MainWindow::MainWindow(QWidget* parent)
	: QMainWindow(parent), ui(new Ui::MainWindow)
{
	ui->setupUi(this);
	
	// Model Controller 생성. 
	m_modelController = new CBCTModelController(ui);
	if (!m_modelController->initialize())
		qDebug() << "CBCTModelController initialize Fail ! ";

    connect(ui->AscendingPushButton, SIGNAL(clicked()), m_modelController, SLOT(on_AscendingPushButton_pressed()));
    connect(ui->DescendingPushButton, SIGNAL(clicked()), m_modelController, SLOT(on_DescendingPushButton_pressed()));
	connect(ui->MainPushButton, SIGNAL(clicked()), m_modelController, SLOT(on_MainPushButton_clicked()));
	connect(ui->SubPushButton, SIGNAL(clicked()), m_modelController, SLOT(on_SubPushButton_clicked()));
    connect(ui->CaptureResetPushButton, SIGNAL(clicked()), m_modelController, SLOT(on_CaptureResetPushButton_VTK_clicked()));
    connect(ui->CaptureReadyPushButton, SIGNAL(clicked()), m_modelController, SLOT(on_CaptureReadyPushButton_VTK_clicked()));

    connect(ui->CaptureResetPushButton, SIGNAL(clicked()), this, SLOT(on_CaptureResetPushButton_clicked()));
	connect(ui->CaptureReadyPushButton, SIGNAL(clicked()), this, SLOT(on_CaptureReadyPushButton_clicked()));
	connect(ui->CaptureStartPushButton, SIGNAL(clicked()), this, SLOT(on_CaptureStartPushButton_clicked()));
	connect(ui->CaptureStopPushButton, SIGNAL(clicked()), this, SLOT(on_CaptureStopPushButton_clicked()));

	m_rawImageViewer = new CBCTRawImageViewer();
	connect(m_rawImageViewer, SIGNAL(signals_panoImage(QImage*)), this, SLOT(slot_panoImage(QImage*)));
	connect(m_rawImageViewer, SIGNAL(signals_cephImage(QImage*)), this, SLOT(slot_cephImage(QImage*)));

	m_fileTransfer = new CBCTFileTransfer(this);
    connect(ui->CaptureReadyPushButton, SIGNAL(clicked), m_fileTransfer, SLOT());
    connect(ui->CaptureResetPushButton, SIGNAL(clicked), m_fileTransfer, SLOT());
    connect(ui->CaptureStartPushButton, SIGNAL(clicked), m_fileTransfer, SLOT());
    connect(ui->CaptureStartPushButton, SIGNAL(clicked), m_fileTransfer, SLOT());

	connect(m_fileTransfer, SIGNAL(READYSignal(ControlType)), this, SLOT(sendControl(ControlType)));
	connect(m_fileTransfer, SIGNAL(RESETSignal(ControlType)), this, SLOT(sendControl(ControlType)));
	connect(m_fileTransfer, SIGNAL(STARTSignal(ControlType)), this, SLOT(sendControl(ControlType)));
	connect(m_fileTransfer, SIGNAL(STOPSignal(ControlType)), this, SLOT(sendControl(ControlType)));

	connect(this, SIGNAL(READYSignal(ControlType)), m_fileTransfer, SLOT(sendControl(ControlType)));
	connect(this, SIGNAL(READYSignal(ControlType)), m_fileTransfer, SLOT(sendControl(ControlType)));
	connect(this, SIGNAL(READYSignal(ControlType)), m_fileTransfer, SLOT(sendControl(ControlType)));
	connect(this, SIGNAL(READYSignal(ControlType)), m_fileTransfer, SLOT(sendControl(ControlType)));

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

void MainWindow::on_CaptureResetPushButton_clicked()
{
    emit RESETSignal(RESET);

}

void MainWindow::on_CaptureReadyPushButton_clicked()
{
    emit READYSignal(READY);

}

void MainWindow::on_CaptureStartPushButton_clicked()
{

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
    emit STARTSignal(START);

}

void MainWindow::on_CaptureStopPushButton_clicked()
{
	m_rawImageViewer->stopPanoTimer();
	m_rawImageViewer->stopCephTimer();
    emit STOPSignal(STOP);


}

void MainWindow::slot_panoImage(QImage* pImg)
{
	qDebug() << __FUNCTION__;
    QGraphicsScene* panoScene = new QGraphicsScene();


	QImage pano_Image(*pImg);
	QPixmap panoPix;
	panoPix = QPixmap::fromImage(pano_Image, Qt::AutoColor);

	/* 파노라마 이미지가 90도 회전되어 있으므로, 출력시 원상복구한다 */
	QTransform panoTransform;
	panoTransform.rotate(90);
    panoScene->addPixmap(panoPix);
	ui->PanoLabel->setPixmap(panoPix.transformed(panoTransform));

	/* 파노라마 Raw Image 전송상태를 표시해주는 ProgressBar */
	int panoValue = ui->PanoProgressBar->value();
	panoValue++;
	ui->PanoProgressBar->setValue(panoValue);
}

void MainWindow::slot_cephImage(QImage* cImg)
{
	qDebug() << __FUNCTION__;
    QGraphicsScene* cephScene = new QGraphicsScene();


	QImage ceph_Image(*cImg);
	QPixmap cephPix;
	cephPix = QPixmap::fromImage(ceph_Image, Qt::AutoColor);
    cephScene->addPixmap(cephPix);
	ui->CephLabel->setPixmap(cephPix);

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

