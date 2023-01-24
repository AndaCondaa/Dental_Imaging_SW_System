#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "cbctfiletransfer.h"
#include "cbctlogthread.h"
#include "cbctrawimageviewer.h"
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

	connect(ui->MainPushButton, SIGNAL(clicked()), m_modelController, SLOT(on_MainPushButton_clicked()));
	connect(ui->SubPushButton, SIGNAL(clicked()), m_modelController, SLOT(on_SubPushButton_clicked()));

	connect(ui->AscendingPushButton, SIGNAL(clicked()), m_modelController, SLOT(on_AscendingPushButton_pressed()));
	connect(ui->DescendingPushButton, SIGNAL(clicked()), m_modelController, SLOT(on_DescendingPushButton_pressed()));

	connect(ui->CaptureResetPushButton, SIGNAL(clicked()), this, SLOT(on_CaptureResetPushButton_clicked()));
	connect(ui->CaptureReadyPushButton, SIGNAL(clicked()), this, SLOT(on_CaptureReadyPushButton_clicked()));
	connect(ui->CaptureStartPushButton, SIGNAL(clicked()), this, SLOT(on_CaptureStartPushButton_clicked()));
	connect(ui->CaptureStopPushButton, SIGNAL(clicked()), this, SLOT(on_CaptureStopPushButton_clicked()));


	m_rawImageViewer = new CBCTRawImageViewer();
	connect(m_rawImageViewer, SIGNAL(signals_panoImage(QImage*)), this, SLOT(slot_panoImage(QImage*)));
	connect(m_rawImageViewer, SIGNAL(signals_cephImage(QImage*)), this, SLOT(slot_cephImage(QImage*)));

	m_fileTransfer = new CBCTFileTransfer(this);
	connect(ui->CaptureReadyPushButton, SIGNAL(clicked), m_fileTransfer, SLOT(on_READYPushButton_clicked()));
	connect(ui->CaptureResetPushButton, SIGNAL(clicked), m_fileTransfer, SLOT(on_RESETPushButton_clicked()));
	connect(ui->CaptureStartPushButton, SIGNAL(clicked), m_fileTransfer, SLOT(on_STARTPushButton_clicked()));
	connect(ui->CaptureStartPushButton, SIGNAL(clicked), m_fileTransfer, SLOT(on_STOPPushButton_clicked()));

	connect(m_fileTransfer, SIGNAL(READYSignal(ControlType)), this, SLOT(sendControl(ControlType)));
	connect(m_fileTransfer, SIGNAL(RESETSignal(ControlType)), this, SLOT(sendControl(ControlType)));
	connect(m_fileTransfer, SIGNAL(STARTSignal(ControlType)), this, SLOT(sendControl(ControlType)));
	connect(m_fileTransfer, SIGNAL(STOPSignal(ControlType)), this, SLOT(sendControl(ControlType)));

	connect(this, SIGNAL(READYSignal(ControlType)), m_fileTransfer, SLOT(sendControl(ControlType)));
	connect(this, SIGNAL(READYSignal(ControlType)), m_fileTransfer, SLOT(sendControl(ControlType)));
	connect(this, SIGNAL(READYSignal(ControlType)), m_fileTransfer, SLOT(sendControl(ControlType)));
	connect(this, SIGNAL(READYSignal(ControlType)), m_fileTransfer, SLOT(sendControl(ControlType)));

	//connect(CBCTFileTransfer, SIGNAL(sendButtonSignal()), this, SLOT(()));
}

MainWindow::~MainWindow()
{
	delete ui;
}

void MainWindow::on_READYPushButton_clicked()
{
	emit READYSignal(READY);
}

void MainWindow::on_RESETPushButton_clicked()
{
	emit RESETSignal(RESET);
}

void MainWindow::on_STARTPushButton_clicked()
{
	emit STARTSignal(START);
}

void MainWindow::on_STOPPushButton_clicked()
{
	emit STOPSignal(STOP);
}

void MainWindow::resizeEvent(QResizeEvent* event)
{
	auto Size = event->size();

	//ui->openGLWidget_All->resize(Size);
	QMainWindow::resizeEvent(event);
}

void MainWindow::Load_OBJFile(QStringList paths, QStringList mtls, std::vector<vtkSmartPointer<vtkPolyData>>& objs)
{
	// TODO : vtkObjImport를 사용하여 MTL을 넣을 수 있긴하나 쫌 이상함. 
	// 바로 Render를 연결해줘야 하는 시스템으로 PolyData를 가져오려면 Mapper를 통해 접근해야 함. 
	for (int i = 0; i < paths.size(); i++)
	{
		vtkSmartPointer<vtkOBJReader> reader = vtkSmartPointer<vtkOBJReader>::New();
		reader->SetFileName(paths[i].toStdString().c_str());
		//reader->SetFileNameMTL(mtls[i].toStdString().c_str());
		reader->Update();

		objs.push_back(reader->GetOutput());
	}

#if USE_VTKOBJIMPORT
	for (int i = 0; i < paths.size(); i++)
	{
		vtkSmartPointer<vtkOBJImporter> reader = vtkSmartPointer<vtkOBJImporter>::New();
		reader->SetFileName(paths[i].toStdString().c_str());
		reader->SetFileNameMTL(mtls[i].toStdString().c_str());
		reader->Update();

		vtkNew<vtkNamedColors> colors;

		vtkNew<vtkRenderer> renderer;
		vtkNew<vtkRenderWindow> renWin;
		vtkNew<vtkRenderWindowInteractor> iren;

		renderer->SetBackground2(colors->GetColor3d("Silver").GetData());
		renderer->SetBackground(colors->GetColor3d("Gold").GetData());
		renderer->GradientBackgroundOn();
		renWin->AddRenderer(renderer);
		renderer->UseHiddenLineRemovalOn();
		renWin->AddRenderer(renderer);
		renWin->SetSize(640, 480);
		renWin->SetWindowName("OBJImporter");

		iren->SetRenderWindow(renWin);
		reader->SetRenderWindow(renWin);
		reader->Update();

		auto actors = renderer->GetActors();
		actors->InitTraversal();
		std::cout << "There are " << actors->GetNumberOfItems() << " actors"
			<< std::endl;

		for (vtkIdType a = 0; a < actors->GetNumberOfItems(); ++a)
		{
			std::cout << reader->GetOutputDescription(a) << std::endl;

			vtkActor* actor = actors->GetNextActor();

			// OBJImporter turns texture interpolation off
			if (actor->GetTexture())
			{
				std::cout << "Has texture\n";
				//actor->GetTexture()->InterpolateOn();
			}

			vtkPolyData* pd =
				dynamic_cast<vtkPolyData*>(actor->GetMapper()->GetInput());

			vtkPolyDataMapper* mapper =
				dynamic_cast<vtkPolyDataMapper*>(actor->GetMapper());
			mapper->SetInputData(pd);
			objs.push_back(pd);
		}



	}
#endif

}

void MainWindow::Create_Mapper(const std::vector<vtkSmartPointer<vtkPolyData>>& objs, std::vector<vtkSmartPointer<vtkMapper>>& mapper)
{
	for (auto data : objs)
	{
		vtkSmartPointer<vtkPolyDataMapper> map = vtkSmartPointer<vtkPolyDataMapper>::New();
		map->SetInputData(data);
		map->Update();
		mapper.push_back(map);
	}
}

void MainWindow::Create_Actor(const std::vector<vtkSmartPointer<vtkMapper>>& mappers, const QString& Color, std::vector<vtkSmartPointer<vtkActor>>& actors)
{
	for (auto map : mappers)
	{
		vtkSmartPointer<vtkActor> actor = vtkSmartPointer<vtkActor>::New();
		actor->SetMapper(map);

		vtkSmartPointer<vtkNamedColors> color = vtkSmartPointer<vtkNamedColors>::New();
		if (Color == "All")
		{
			actor->GetProperty()->SetDiffuseColor(color->GetColor3d("Light Gray").GetData());
		}
		else if (Color == "Main")
		{
			actor->GetProperty()->SetDiffuseColor(color->GetColor3d("Light Gray").GetData());
		}
		else if (Color == "Sub") {
			actor->GetProperty()->SetDiffuseColor(color->GetColor3d("Light Gray").GetData());
		}

		actor->GetProperty()->SetSpecular(5.0);
		actor->GetProperty()->SetSpecularPower(5.0);
		actors.push_back(actor);
	}
}

vtkSmartPointer<vtkGenericOpenGLRenderWindow> MainWindow::Create_Render(const std::vector<vtkSmartPointer<vtkActor>>& Actors)
{
	vtkSmartPointer<vtkNamedColors> colors = vtkSmartPointer<vtkNamedColors>::New();
	vtkSmartPointer<vtkRenderer> Renderer = vtkSmartPointer<vtkRenderer>::New();

	for (auto actor : Actors)
		Renderer->AddActor(actor);

#ifdef USE_DISPLAY_GLOBALAXIS
	vtkNew<vtkTransform> transform;
	transform->Translate(0.0, 0.0, 0.0);

	vtkSmartPointer<vtkAxesActor> axes = vtkSmartPointer<vtkAxesActor>::New();
	axes->SetTotalLength(700, 700, 700);
	Renderer->AddActor(axes);
#endif

	Renderer->SetBackground(colors->GetColor3d("Black").GetData());
	Renderer->ResetCamera();
	Renderer->LightFollowCameraOn();
	Renderer->GetActiveCamera()->Azimuth(240);
	Renderer->GetActiveCamera()->Elevation(15);
	Renderer->GetActiveCamera()->Zoom(3.0);
	Renderer->ResetCameraClippingRange();

	vtkSmartPointer<vtkGenericOpenGLRenderWindow> renderWindow = vtkSmartPointer<vtkGenericOpenGLRenderWindow>::New();
	renderWindow->AddRenderer(Renderer);
	return renderWindow;
}

void MainWindow::on_CaptureStartPushButton_clicked()
{
	QGraphicsScene* panoScene = new QGraphicsScene();
	QGraphicsScene* cephScene = new QGraphicsScene();

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
	ui->PanoLabel->setPixmap(panoPix.transformed(panoTransform));

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

