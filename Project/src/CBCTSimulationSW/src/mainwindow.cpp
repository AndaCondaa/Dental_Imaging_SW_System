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

/* 3D .Obj Visualization */

#include <vtkRenderWindow.h>
#include<vtkOBJExporter.h>


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

MainWindow::MainWindow(QWidget* parent)
	: QMainWindow(parent), ui(new Ui::MainWindow)
{
	ui->setupUi(this);

	/* 가시화 파이프라인 순서도 */
	/* PolyData -> Mapper -> Actor -> Renderer -> RenderWindow */

	/* 상부, 하부, 파노라마, 세팔로좌+우 총 4개 모듈 */

	/* Load Source */
	QStringList FilePath;
	FilePath << "lowerbody.obj";
	FilePath << "upperbody.obj";
	FilePath << "panomodule.obj";
	FilePath << "cephmodule.obj";

	QStringList MtlFilePath;
	MtlFilePath << "lowerbody.mtl";
	MtlFilePath << "upperbody.mtl";
	MtlFilePath << "panomodule.mtl";
	MtlFilePath << "cephmodule.mtl";

	Load_OBJFile(FilePath, MtlFilePath, m_vecdata);

	/* Create Mapper & Actor */
	Create_Mapper(m_vecdata, m_mapper_All);
	Create_Mapper(m_vecdata, m_mapper_Main);
	Create_Mapper(m_vecdata, m_mapper_Sub);

	Create_Actor(m_mapper_All, "All", m_actor_All);
	Create_Actor(m_mapper_Main, "Main", m_actor_Main);
	Create_Actor(m_mapper_Sub, "Sub", m_actor_Sub);


	/* Starting Location */
#if USE_TRANSFORM
	vtkSmartPointer<vtkTransform> transformLowerBodyAll = vtkSmartPointer<vtkTransform>::New();
	transformLowerBodyAll->Translate(0.0, -1000.0, 10.0);
	transformLowerBodyAll->RotateWXYZ(5, 0.0, 1.0, 0.0);
	actorLowerBodyAll->SetUserTransform(transformLowerBodyAll);
	transformLowerBodyAll->Update();

	vtkSmartPointer<vtkTransform> transformUpperBodyAll = vtkSmartPointer<vtkTransform>::New();
	transformUpperBodyAll->Translate(0.0, -1000.0, 10.0);
	transformUpperBodyAll->RotateWXYZ(5, 0.0, 1.0, 0.0);
	actorUpperBodyAll->SetUserTransform(transformUpperBodyAll);
	transformUpperBodyAll->Update();

	vtkSmartPointer<vtkTransform> transformPanoModuleAll = vtkSmartPointer<vtkTransform>::New();
	transformPanoModuleAll->Translate(0.0, -1000.0, 10.0);
	transformPanoModuleAll->RotateWXYZ(5, 0.0, 1.0, 0.0);
	actorPanoModuleAll->SetUserTransform(transformPanoModuleAll);
	transformPanoModuleAll->Update();

	vtkSmartPointer<vtkTransform> transformCephModuleAll = vtkSmartPointer<vtkTransform>::New();
	transformCephModuleAll->Translate(0.0, -1000.0, 10.0);
	transformCephModuleAll->RotateWXYZ(5, 0.0, 1.0, 0.0);
	actorCephModuleAll->SetUserTransform(transformCephModuleAll);
	transformCephModuleAll->Update();

	vtkSmartPointer<vtkTransform> transformMain = vtkSmartPointer<vtkTransform>::New();
	transformMain->Translate(0.0, -200.0, 100.0);
	transformMain->RotateWXYZ(10, 0.0, 1.0, 1.0);
	actorLowerBodyMain->SetUserTransform(transformMain);
	actorUpperBodyMain->SetUserTransform(transformMain);
	actorPanoModuleMain->SetUserTransform(transformMain);
	actorCephModuleMain->SetUserTransform(transformMain);
	transformMain->Update();

	vtkSmartPointer<vtkTransform> transformSub = vtkSmartPointer<vtkTransform>::New();
	transformSub->Translate(100.0, -100.0, -500.0);
	transformSub->RotateWXYZ(10, 5.0, 10.0, 5.0);
	actorLowerBodySub->SetUserTransform(transformSub);
	actorUpperBodySub->SetUserTransform(transformSub);
	actorPanoModuleSub->SetUserTransform(transformSub);
	actorCephModuleSub->SetUserTransform(transformSub);
	transformSub->Update();

	vtkSmartPointer<vtkTransformPolyDataFilter> transformFilterLowerBodyAll = vtkSmartPointer<vtkTransformPolyDataFilter>::New();
	//     transformFilterAll->SetInputConnection(readerObjAll->GetOutputPort());
	transformFilterLowerBodyAll->SetInputConnection(readerOBJLowerBodyAll->GetOutputPort());
	transformFilterLowerBodyAll->SetInputConnection(readerOBJUpperBodyAll->GetOutputPort());
	transformFilterLowerBodyAll->SetInputConnection(readerOBJPanoModuleAll->GetOutputPort());
	transformFilterLowerBodyAll->SetInputConnection(readerOBJCephModuleAll->GetOutputPort());

	transformFilterLowerBodyAll->SetTransform(transformLowerBodyAll);
	transformFilterLowerBodyAll->SetTransform(transformUpperBodyAll);
	transformFilterLowerBodyAll->SetTransform(transformPanoModuleAll);
	transformFilterLowerBodyAll->SetTransform(transformCephModuleAll);


	transformFilterLowerBodyAll->Update();
#endif
	/* Visualize with Cam */

	m_renderwindow.push_back(Create_Render(m_actor_All));
	m_renderwindow.push_back(Create_Render(m_actor_Main));
	m_renderwindow.push_back(Create_Render(m_actor_Sub));


	ui->openGLWidget_All->setRenderWindow(m_renderwindow[0]);
	ui->openGLWidget_Main->setRenderWindow(m_renderwindow[1]);
	ui->openGLWidget_Sub->setRenderWindow(m_renderwindow[2]);

	ui->openGLWidget_All->interactor()->ProcessEvents();
	ui->openGLWidget_Main->interactor()->ProcessEvents();
	ui->openGLWidget_Sub->interactor()->ProcessEvents();

	for (auto win : m_renderwindow)
		win->Render();
	

	connect(ui->MainPushButton, SIGNAL(clicked()), this, SLOT(on_MainPushButton_Clicked()));
	connect(ui->SubPushButton, SIGNAL(clicked()), this, SLOT(on_SubPushButton_Clicked()));
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

vtkSmartPointer<vtkPolyData> MainWindow::Load_OBJFile(DataType type)
{
	switch (type)
	{
	case Lower:
	{
		vtkSmartPointer<vtkOBJReader> reader = vtkSmartPointer<vtkOBJReader>::New();
		reader->SetFileName("");
		reader->Update();
		return reader->GetOutput();
	}
	default:
		break;
	}
	return vtkSmartPointer<vtkPolyData>();
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
            //actor->GetProperty()->SetDiffuseColor(color->GetColor3d("Light Gray").GetData());
		}
		else if (Color == "Main")
		{
            //actor->GetProperty()->SetDiffuseColor(color->GetColor3d("Light Gray").GetData());
		}
		else if (Color == "Sub") {
            //actor->GetProperty()->SetDiffuseColor(color->GetColor3d("Light Gray").GetData());
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

void MainWindow::on_MainPushButton_clicked()
{
	qDebug() << "Main Push Btn!!";
	auto center = m_actor_Main[3]->GetCenter();
	
	vtkSmartPointer<vtkTransform> transformPanoModuleAll = vtkSmartPointer<vtkTransform>::New();
	transformPanoModuleAll->Translate(center[0], center[1]-10, center[2]);
	transformPanoModuleAll->RotateWXYZ(0, 1.0, 1.0, 1.0);
	m_actor_Main[3]->SetUserTransform(transformPanoModuleAll);
	transformPanoModuleAll->Update();
	m_renderwindow[1]->Render();


	//for (int i = 360; i > 0; i--)
	//{
	//	Sleep(50);

	//	vtkSmartPointer<vtkTransform> transformPanoModuleAll = vtkSmartPointer<vtkTransform>::New();
	//	transformPanoModuleAll->RotateWXYZ(i, 0.0, 1.0, 0.0);
	//	m_actor_Main[2]->SetUserTransform(transformPanoModuleAll);
	//	transformPanoModuleAll->Update();

	//	for (auto win : m_renderwindow)
	//		win->Render();
	//}

}

void MainWindow::on_SubPushButton_clicked()
{
	qDebug() << "Sub Push Btn!!";
	vtkSmartPointer<vtkTransform> transformPanoModuleAll = vtkSmartPointer<vtkTransform>::New();
	for (int i = 360; i > 0; i--)
	{
		Sleep(10);
		transformPanoModuleAll->RotateWXYZ(i, 0.0, 1.0, 0.0);
		m_actor_Main[2]->SetUserTransform(transformPanoModuleAll);
		transformPanoModuleAll->Update();
		m_renderwindow[1]->Render();
		ui->openGLWidget_Main->update();
	}
}

void MainWindow::resizeEvent(QResizeEvent* event)
{
   //auto Size = event->size();
   //ui->openGLWidget_All->resize();

}

/* 상하 이동 시 LowBodyModule 제외 모든 모듈이 같은 좌표대로 이동 */
void ModuleAscending()
{

}

void ModuleDescending()
{

}

/* 앞뒤 이동 시 CephModule 만 이동 */
void CephBackandForth()
{

}

/* 회전 이동 시 PanoModule 만 이동 */
void PanoRotation()
{

}

