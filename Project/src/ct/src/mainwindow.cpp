#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "cbctfiletransfer.h"
#include "cbctlogthread.h"
#include "cbctrawimageviewer.h"

#include <QFile>
#include <QGraphicsPixmapItem>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QList>
#include <QTransform>

/* 3D .Obj Visualization */
#include <vtkSmartPointer.h>
#include <vtkRenderWindow.h>
#include <vtkGenericOpenGLRenderWindow.h>
#include <vtkPolyData.h>
#include <vtkPolyDataMapper.h>
#include <vtkPolyDataNormals.h>

#include <vtkActor.h>
#include <vtkRenderer.h>

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

#include <QVTKOpenGLStereoWidget.h>
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

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    /* 가시화 파이프라인 순서도 */
    /* PolyData -> Mapper -> Actor -> Renderer -> RenderWindow */

    /* 상부, 하부, 파노라마, 세팔로좌+우 총 4개 모듈 */


    /* Load Source */
    vtkSmartPointer<vtkOBJReader> readerObjAll = vtkSmartPointer<vtkOBJReader>::New();
    //readerObjAll->SetFileName("CTObj.obj");
    readerObjAll->SetFileName("cephmodule.obj");
    readerObjAll->SetFileName("panomodule.obj");
    readerObjAll->SetFileName("lowerbody.obj");
    readerObjAll->SetFileName("upperbody.obj");

    readerObjAll->Update();
    vtkSmartPointer<vtkOBJReader> readerObjMain = vtkSmartPointer<vtkOBJReader>::New();
    readerObjMain->SetFileName("CTObj.obj");
    readerObjMain->Update();
    vtkSmartPointer<vtkOBJReader> readerObjSub = vtkSmartPointer<vtkOBJReader>::New();
    readerObjSub->SetFileName("CTObj.obj");
    readerObjSub->Update();

    vtkSmartPointer<vtkOBJReader> readerOBJLowerBody = vtkSmartPointer<vtkOBJReader>::New();
    readerOBJLowerBody->SetFileName("lowerbody.obj");
    readerOBJLowerBody->Update();
    vtkSmartPointer<vtkOBJReader> readerOBJUpperBody = vtkSmartPointer<vtkOBJReader>::New();
    readerOBJUpperBody->SetFileName("upperbody.obj");
    readerOBJUpperBody->Update();
    vtkSmartPointer<vtkOBJReader> readerOBJCephModule = vtkSmartPointer<vtkOBJReader>::New();
    readerOBJCephModule->SetFileName("cephmodule.obj");
    readerOBJCephModule->Update();
    vtkSmartPointer<vtkOBJReader> readerOBJPanoModule = vtkSmartPointer<vtkOBJReader>::New();
    readerOBJPanoModule->SetFileName("panomodule.obj");
    readerOBJPanoModule->Update();

    vtkSmartPointer<vtkNamedColors> colorsAll = vtkSmartPointer<vtkNamedColors>::New();
    vtkColor3d backgroundColorAll = colorsAll->GetColor3d("Black");
//    vtkColor3d actorColorAll = colorsAll->GetColor3d("HoneyDew");
    vtkSmartPointer<vtkNamedColors> colorsMain = vtkSmartPointer<vtkNamedColors>::New();
    vtkColor3d backgroundColorMain = colorsMain->GetColor3d("Black");
//    vtkColor3d actorColorMain = colorsMain->GetColor3d("HoneyDew");
    vtkSmartPointer<vtkNamedColors> colorsSub = vtkSmartPointer<vtkNamedColors>::New();
    vtkColor3d backgroundColorSub = colorsSub->GetColor3d("Black");
//    vtkColor3d actorColorSub = colorsSub->GetColor3d("HoneyDew");


//    vtkSmartPointer<vtkPointData> pointDataAll = readerObjAll->GetOutput()->GetPointData();
//    vtkSmartPointer<vtkPointData> pointDataMain = readerObjMain->GetOutput()->GetPointData();
//    vtkSmartPointer<vtkPointData> pointDataSub = readerObjSub->GetOutput()->GetPointData();

        vtkSmartPointer<vtkPointData> pointDataLowerBody = readerOBJLowerBody->GetOutput()->GetPointData();
        vtkSmartPointer<vtkPointData> pointDataUpperBody = readerOBJUpperBody->GetOutput()->GetPointData();
        vtkSmartPointer<vtkPointData> pointDataPanoModule = readerOBJPanoModule->GetOutput()->GetPointData();
        vtkSmartPointer<vtkPointData> pointDataCephModule = readerOBJCephModule->GetOutput()->GetPointData();

    vtkSmartPointer<vtkPolyDataNormals> normalsAll = vtkSmartPointer<vtkPolyDataNormals>::New();
//    normalsAll->SetInputData(readerObjAll->GetOutput());
        normalsAll->SetInputData(readerOBJLowerBody->GetOutput());
        normalsAll->SetInputData(readerOBJUpperBody->GetOutput());
        normalsAll->SetInputData(readerOBJPanoModule->GetOutput());
        normalsAll->SetInputData(readerOBJCephModule->GetOutput());
    normalsAll->ComputePointNormalsOn();
    normalsAll->ComputeCellNormalsOn();
    normalsAll->Update();

    vtkSmartPointer<vtkPolyDataNormals> normalsMain = vtkSmartPointer<vtkPolyDataNormals>::New();
//    normalsMain->SetInputData(readerObjMain->GetOutput());
        normalsMain->SetInputData(readerOBJLowerBody->GetOutput());
        normalsMain->SetInputData(readerOBJUpperBody->GetOutput());
        normalsMain->SetInputData(readerOBJPanoModule->GetOutput());
        normalsMain->SetInputData(readerOBJCephModule->GetOutput());
    normalsMain->ComputePointNormalsOn();
    normalsMain->ComputeCellNormalsOn();
    normalsMain->Update();

    vtkSmartPointer<vtkPolyDataNormals> normalsSub = vtkSmartPointer<vtkPolyDataNormals>::New();
//    normalsSub->SetInputData(readerObjSub->GetOutput());
        normalsSub->SetInputData(readerOBJLowerBody->GetOutput());
        normalsSub->SetInputData(readerOBJUpperBody->GetOutput());
        normalsSub->SetInputData(readerOBJPanoModule->GetOutput());
        normalsSub->SetInputData(readerOBJCephModule->GetOutput());
    normalsSub->ComputePointNormalsOn();
    normalsSub->ComputeCellNormalsOn();
    normalsSub->Update();

    /* Create Mapper & Actor */
//    vtkSmartPointer<vtkPolyDataMapper> mapperAll = vtkSmartPointer<vtkPolyDataMapper>::New();
//    mapperAll->SetInputConnection(readerObjAll->GetOutputPort());

        vtkSmartPointer<vtkPolyDataMapper> mapperLowerBody = vtkSmartPointer<vtkPolyDataMapper>::New();
        mapperLowerBody->SetInputData(readerOBJLowerBody->GetOutput());
//        mapperAll->SetInputData(readerOBJUpperBody->GetOutput());
//        mapperAll->SetInputData(readerOBJPanoModule->GetOutput());
//        mapperAll->SetInputData(readerOBJCephModule->GetOutput());
//    vtkSmartPointer<vtkPolyDataMapper> mapperMain = vtkSmartPointer<vtkPolyDataMapper>::New();
//    mapperMain->SetInputConnection(readerObjMain->GetOutputPort());

        vtkSmartPointer<vtkPolyDataMapper> mapperUpperBody = vtkSmartPointer<vtkPolyDataMapper>::New();
//        mapperMain->SetInputData(readerOBJLowerBody->GetOutput());
        mapperUpperBody->SetInputData(readerOBJUpperBody->GetOutput());
//        mapperMain->SetInputData(readerOBJPanoModule->GetOutput());
//        mapperMain->SetInputData(readerOBJCephModule->GetOutput());

//        vtkSmartPointer<vtkPolyDataMapper> mapperSub = vtkSmartPointer<vtkPolyDataMapper>::New();
//    mapperSub->SetInputConnection(readerObjSub->GetOutputPort());

        vtkSmartPointer<vtkPolyDataMapper> mapperPanoModule = vtkSmartPointer<vtkPolyDataMapper>::New();
//        mapperSub->SetInputData(readerOBJLowerBody->GetOutput());
//        mapperSub->SetInputData(readerOBJUpperBody->GetOutput());
        mapperPanoModule->SetInputData(readerOBJPanoModule->GetOutput());
//        mapperSub->SetInputData(readerOBJCephModule->GetOutput());

        vtkSmartPointer<vtkPolyDataMapper> mapperCephModule = vtkSmartPointer<vtkPolyDataMapper>::New();
        mapperCephModule->SetInputData(readerOBJCephModule->GetOutput());

//    vtkSmartPointer<vtkActor> actorAll = vtkSmartPointer<vtkActor>::New();
//    actorAll->SetMapper(mapperAll);
    //    actorAll->GetProperty()->SetDiffuseColor(colorsMain->GetColor3d("LightGray").GetData());
//    actorAll->SetMapper(mapperLowerBody);
//    actorAll->SetMapper(mapperUpperBody);
//    actorAll->SetMapper(mapperCephModule);
//    actorAll->SetMapper(mapperPanoModule);
//    actorAll->GetProperty()->SetSpecular(5.0);
//    actorAll->GetProperty()->SetSpecularPower(5.0);

//      vtkSmartPointer<vtkActor> actorMain = vtkSmartPointer<vtkActor>::New();
//    actorMain->SetMapper(mapperMain);
    //    actorMain->GetProperty()->SetDiffuseColor(colorsMain->GetColor3d("LightGray").GetData());
//    actorMain->SetMapper(mapperLowerBody);
//    actorMain->SetMapper(mapperUpperBody);
//    actorMain->SetMapper(mapperCephModule);
//    actorMain->SetMapper(mapperPanoModule);
//    actorMain->GetProperty()->SetSpecular(5.0);
//    actorMain->GetProperty()->SetSpecularPower(5.0);
//    vtkSmartPointer<vtkActor> actorSub = vtkSmartPointer<vtkActor>::New();
//    actorSub->SetMapper(mapperSub);
    //    actorSub->GetProperty()->SetDiffuseColor(colorsSub->GetColor3d("Red").GetData());
//    actorSub->SetMapper(mapperLowerBody);
//    actorSub->SetMapper(mapperUpperBody);
//    actorSub->SetMapper(mapperCephModule);
//    actorSub->SetMapper(mapperPanoModule);
//    actorSub->GetProperty()->SetSpecular(5.0);
//    actorSub->GetProperty()->SetSpecularPower(5.0);

    vtkSmartPointer<vtkActor> actorLowerBody = vtkSmartPointer<vtkActor>::New();
    actorLowerBody->SetMapper(mapperLowerBody);
    vtkSmartPointer<vtkActor> actorUpperBody = vtkSmartPointer<vtkActor>::New();
    actorUpperBody->SetMapper(mapperUpperBody);
    vtkSmartPointer<vtkActor> actorCephModule = vtkSmartPointer<vtkActor>::New();
    actorCephModule->SetMapper(mapperCephModule);
    vtkSmartPointer<vtkActor> actorPanoModule = vtkSmartPointer<vtkActor>::New();
    actorPanoModule->SetMapper(mapperPanoModule);


    /* Visualize with Cam */
//    vtkSmartPointer<vtkRenderer> rendererAll = vtkSmartPointer<vtkRenderer>::New();
//    rendererAll->AddActor(actorAll);
//    rendererAll->SetBackground(backgroundColorAll.GetData());
//    rendererAll->ResetCamera();
//    rendererAll->LightFollowCameraOn();
//    rendererAll->GetActiveCamera()->Azimuth(320);
//    rendererAll->GetActiveCamera()->Elevation(20);
//    rendererAll->GetActiveCamera()->Zoom(1.6);
//    rendererAll->ResetCameraClippingRange();
//    vtkSmartPointer<vtkRenderer> rendererMain = vtkSmartPointer<vtkRenderer>::New();
//    rendererMain->AddActor(actorMain);
//    rendererMain->SetBackground(backgroundColorAll.GetData());
//    rendererMain->ResetCamera();
//    rendererMain->LightFollowCameraOn();
//    rendererMain->GetActiveCamera()->Azimuth(60);
//    rendererMain->GetActiveCamera()->Elevation(40);
//    rendererMain->GetActiveCamera()->Zoom(2.5);
//    rendererMain->ResetCameraClippingRange();
//    vtkSmartPointer<vtkRenderer> rendererSub = vtkSmartPointer<vtkRenderer>::New();
//    rendererSub->AddActor(actorSub);
//    rendererSub->SetBackground(backgroundColorAll.GetData());
//    rendererSub->ResetCamera();
//    rendererSub->LightFollowCameraOn();\
//    rendererSub->GetActiveCamera()->Azimuth(240);
//    rendererSub->GetActiveCamera()->Elevation(15);
//    rendererSub->GetActiveCamera()->Zoom(2.2);
//    rendererSub->ResetCameraClippingRange();

    vtkSmartPointer<vtkRenderer> rendererLowerBody = vtkSmartPointer<vtkRenderer>::New();
    rendererLowerBody->AddActor(actorLowerBody);
    vtkSmartPointer<vtkRenderer> rendererUpperBody = vtkSmartPointer<vtkRenderer>::New();
    rendererUpperBody->AddActor(actorUpperBody);
    vtkSmartPointer<vtkRenderer> rendererCephModule = vtkSmartPointer<vtkRenderer>::New();
    rendererCephModule->AddActor(actorCephModule);
    vtkSmartPointer<vtkRenderer> rendererPanoModule = vtkSmartPointer<vtkRenderer>::New();
    rendererPanoModule->AddActor(actorPanoModule);

//    vtkSmartPointer<vtkTransform> transformAll = vtkSmartPointer<vtkTransform>::New();
//    transformAll->Translate(0.0, -350.0, 10.0);
//    transformAll->RotateWXYZ(5, 0.0, 1.0, 0.0);
//    actorAll->SetUserTransform(transformAll);

//    vtkSmartPointer<vtkTransform> transformMain = vtkSmartPointer<vtkTransform>::New();
//    transformMain->Translate(0.0, -200.0, 100.0);
//    transformMain->RotateWXYZ(10, 0.0, 1.0, 1.0);
//    actorMain->SetUserTransform(transformMain);

//    vtkSmartPointer<vtkTransform> transformSub = vtkSmartPointer<vtkTransform>::New();
//    transformSub->Translate(100.0, -100.0, -300.0);
//    transformSub->RotateWXYZ(10, 5.0, 10.0, 5.0);
//    actorSub->SetUserTransform(transformSub);

//    vtkSmartPointer<vtkTransformPolyDataFilter> transformFilterAll = vtkSmartPointer<vtkTransformPolyDataFilter>::New();
//    transformFilterAll->SetInputConnection(readerObjAll->GetOutputPort());
    //    transformFilterAll->SetInputConnection(readerOBJLowBody->GetOutputPort());
    //    transformFilterAll->SetInputConnection(readerOBJMainBody->GetOutputPort());
    //    transformFilterAll->SetInputConnection(readerOBJPano->GetOutputPort());
    //    transformFilterAll->SetInputConnection(readerOBJCeph->GetOutputPort());

//    transformFilterAll->SetTransform(transformAll);
//    transformFilterAll->Update();

//    vtkSmartPointer<vtkTransformPolyDataFilter> transformFilterMain = vtkSmartPointer<vtkTransformPolyDataFilter>::New();
//    transformFilterMain->SetInputConnection(readerObjMain->GetOutputPort());
    //    transformFilterMain->SetInputConnection(readerOBJLowBody->GetOutputPort());
    //    transformFilterMain->SetInputConnection(readerOBJMainBody->GetOutputPort());
    //    transformFilterMain->SetInputConnection(readerOBJPano->GetOutputPort());
    //    transformFilterMain->SetInputConnection(readerOBJCeph->GetOutputPort());
//    transformFilterMain->SetTransform(transformMain);
//    transformFilterMain->Update();

//    vtkSmartPointer<vtkTransformPolyDataFilter> transformFilterSub = vtkSmartPointer<vtkTransformPolyDataFilter>::New();
//    transformFilterSub->SetInputConnection(readerObjSub->GetOutputPort());
    //    transformFilterSub->SetInputConnection(readerOBJLowBody->GetOutputPort());
    //    transformFilterSub->SetInputConnection(readerOBJMainBody->GetOutputPort());
    //    transformFilterSub->SetInputConnection(readerOBJPano->GetOutputPort());
    //    transformFilterSub->SetInputConnection(readerOBJCeph->GetOutputPort());
//    transformFilterSub->SetTransform(transformSub);
//    transformFilterSub->Update();


    /* Rendering */
//    vtkSmartPointer<vtkGenericOpenGLRenderWindow> renderWindowAll = vtkSmartPointer<vtkGenericOpenGLRenderWindow>::New();
//    renderWindowAll->AddRenderer(rendererAll);
    //    renderWindowAll->Render();

//    renderWindowAll->SetWindowName("readAll");
//    vtkSmartPointer<vtkGenericOpenGLRenderWindow> renderWindowMain = vtkSmartPointer<vtkGenericOpenGLRenderWindow>::New();
//    renderWindowMain->AddRenderer(rendererMain);
    //    renderWindowMain->Render();
//    renderWindowMain->SetWindowName("readMain");
//    vtkSmartPointer<vtkGenericOpenGLRenderWindow> renderWindowSub = vtkSmartPointer<vtkGenericOpenGLRenderWindow>::New();
//    renderWindowSub->AddRenderer(rendererSub);
    //    renderWindowSub->Render();
//    renderWindowSub->SetWindowName("readSub");

    vtkSmartPointer<vtkGenericOpenGLRenderWindow> renderWindowLowerBody = vtkSmartPointer<vtkGenericOpenGLRenderWindow>::New();
    renderWindowLowerBody->AddRenderer(rendererLowerBody);
    vtkSmartPointer<vtkGenericOpenGLRenderWindow> renderWindowUpperBody = vtkSmartPointer<vtkGenericOpenGLRenderWindow>::New();
    renderWindowUpperBody->AddRenderer(rendererUpperBody);
    vtkSmartPointer<vtkGenericOpenGLRenderWindow> renderWindowPanoModule = vtkSmartPointer<vtkGenericOpenGLRenderWindow>::New();
    renderWindowPanoModule->AddRenderer(rendererPanoModule);
    vtkSmartPointer<vtkGenericOpenGLRenderWindow> renderWindowCephModule = vtkSmartPointer<vtkGenericOpenGLRenderWindow>::New();
    renderWindowCephModule->AddRenderer(rendererCephModule);


//    ui->openGLWidget_All->setRenderWindow(renderWindowAll);
//    ui->openGLWidget_Main->setRenderWindow(renderWindowMain);
//    ui->openGLWidget_Sub->setRenderWindow(renderWindowSub);

//    ui->openGLWidget_All->interactor()->SetRenderWindow(renderWindowAll);
//    ui->openGLWidget_Main->interactor()->SetRenderWindow(renderWindowMain);
//    ui->openGLWidget_Sub->interactor()->SetRenderWindow(renderWindowSub);

    ui->openGLWidget_All->setRenderWindow(renderWindowLowerBody);
//    ui->openGLWidget_All->setRenderWindow(renderWindowUpperBody);
//    ui->openGLWidget_All->setRenderWindow(renderWindowPanoModule);
//    ui->openGLWidget_All->setRenderWindow(renderWindowCephModule);

    ui->openGLWidget_Main->setRenderWindow(renderWindowPanoModule);
    ui->openGLWidget_Sub->setRenderWindow(renderWindowCephModule);

    ui->openGLWidget_All->interactor()->ProcessEvents();
    ui->openGLWidget_Main->interactor()->ProcessEvents();
    ui->openGLWidget_Sub->interactor()->ProcessEvents();

    ui->openGLWidget_All->interactor()->Start();
    ui->openGLWidget_Main->interactor()->Start();
    ui->openGLWidget_Sub->interactor()->Start();

    //connect(ui->MainPushButton, SIGNAL(clicked()), this, SLOT(on_MainPushButton_Clicked()));
    //connect(ui->SubPushButton, SIGNAL(clicked()), this, SLOT(on_SubPushButton_Clicked()));
    //    connect(ui->CaptureStartPushButton, SIGNAL(clicked()), this, SLOT(CaptureStartPushButton_clicked()));
    //    connect(ui->pushButton_12,SIGNAL(clicked(bool)), this, SLOT(pushbutton_12(bool)));
    connect(ui->CaptureStartPushButton, SIGNAL(clicked()), this, SLOT(on_CaptureStartPushButton_clicked()));
    //    connect(ui->pushButton_12, SIGNAL(clicked()), this, SLOT(on_CaptureStopPushButton_clicked()));


    m_rawImageViewer = new CBCTRawImageViewer();
    connect(m_rawImageViewer, SIGNAL(signals_panoImage(QImage*)), this, SLOT(slot_panoImage(QImage*)));
    connect(m_rawImageViewer, SIGNAL(signals_cephImage(QImage*)), this, SLOT(slot_cephImage(QImage*)));


}

MainWindow::~MainWindow()
{
    delete ui;
}

void rotateMain()
{

}

void MainWindow::on_CaptureStartPushButton_clicked()
{


    QGraphicsScene *panoScene = new QGraphicsScene();
    QGraphicsScene *cephScene = new QGraphicsScene();



    if(ui->PanoCheckBox->isChecked())
    {
        qDebug() << __FUNCTION__;
        m_rawImageViewer->startPanoTimer();
        //CBCTRawImageViewer m_rawImageViewer;
        //QPixmap panoPix = m_rawImageViewer.PanoImageViewer();
        //ui->PanoLabel->setPixmap(panoPix);
    }

    if(ui->CephCheckBox->isChecked())
    {
        qDebug() << __FUNCTION__;
        m_rawImageViewer->startCephTimer();
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
}

void MainWindow::slot_cephImage(QImage* cImg)
{
    qDebug() << __FUNCTION__;

    QImage ceph_Image(*cImg);
    QPixmap cephPix;
    cephPix = QPixmap::fromImage(ceph_Image, Qt::AutoColor);
    ui->CephLabel->setPixmap(cephPix);
}

void MainWindow::on_MainPushButton_clicked()
{

}
void MainWindow::on_SubPushButton_clicked()
{

}


void loadObj()
{

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

