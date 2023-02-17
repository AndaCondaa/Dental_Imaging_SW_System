/*프로그램명 : ImageViewerSW
파일명 : imagealbum.cpp
설명 : graphicsview, graphicsscene을 활용하여 다양한 영상처리(Sharpen, Blur, Contrast,
Brightness, 좌우반전, 상하반전, 회전, 줌인, 줌아웃 등의 기능을 담고있는 클래스
사용성을 위해 처방전 작성 버튼과 진료 종료 버튼은 해당 파일 내에 존재하고 있고, 진료 종료 버튼 클릭 시
다음 환자의 진료 시작을 시작할 수 있음
작성자 : 이정연
최종 수정 날짜 : 2023.02.11*/


#include "ui_imagealbum.h"
#include "imagescene.h"
#include "imagealbum.h"
#include "prescription.h"

#include <QListWidget>
#include <QDir>
#include <QFileInfo>
#include <QColorDialog>
#include <QMessageBox>
#include <QChart>
#include <QChartView>
#include <QLineSeries>
#include <QPrinter>
#include <QPrintDialog>

#include <opencv2/imgcodecs.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc.hpp>

using namespace cv;


#include <QtTest/QtTest>
#include <QTest>

ImageAlbum::ImageAlbum(QWidget *parent)
    : QWidget(parent), ui(new Ui::ImageAlbum)
{
    ui->setupUi(this);

    // 이미지 정보 및 촬영일자는 읽기 전용으로 설정
    ui->ImageInfo->setReadOnly(true);
    ui->PhotoDate->setReadOnly(true);

    // 화면이 처음 켜지면 진료 종료, 처방전 작성 버튼 비활성화
    ui->Prescription->setDisabled(true);
    ui->EndTreatment->setDisabled(true);

    imageView = new QGraphicsView(this);
    imageScene = new ImageScene(this);
    m_prescription = new Prescription(0);
    chart = new QChart;

    messagebox = new QMessageBox();
    messagebox->setStyleSheet("QMessageBox { "
                              "background-color: rgb(255, 255, 255);"
                              "color:#000000;"
                              "}"
                              "QPushButton { "
                              "background-color: rgb(250, 250, 250);"
                              "border-radius:2px;"
                              "font: bold;"
                              "color:rgb(0, 0, 0);"
                              "border-color: blue"
                              "outline: 0;"
                              "}");



    //    QTest::qExec(imageView);
    //    QTest::qExec(imageScene);
    //    QTest::qExec(m_prescription);

    view = new QChartView;
    view->setChart(chart);

    QVBoxLayout *lay = new QVBoxLayout(ui->widget);
    lay->addWidget(view);
    chart->setContentsMargins(0, 0, 0, 0);
    view->setContentsMargins(0, 0, 0, 0);

    // 초깃값 설정
    prescriptionCheck = false;
    // 서버로부터 이미지 파일 수신 완료되면 true로 변경
    AllSendImageCheck = false;
    paintColor = Qt::white;
    // 텍스트 데이터 입력란에 입력하기 전의 텍스트 값을 설정
    ui->lineEdit->setPlaceholderText("텍스트를 입력해주세요...");

    ui->ImplantcomboBox->setStyleSheet("QComboBox {"
                                       "border : 1px solid rgba(190, 190, 190, 150);"
                                       "border-radius: 0px;"
                                       "}"
                                       "QComboBox QAbstractItemView {"
                                       "background-color: white;"
                                       "selection-background-color: rgb(0, 0, 0);"
                                       "border: 1px solid rgba(190, 190, 190, 150);"
                                       "border-radius: 0px;"
                                       "padding: 2px;"
                                       "outline: 0;"
                                       "color: black;"
                                       "}");

    ui->tabWidget->setStyleSheet("QTabBar {"
                                 "border: 0px solid #31363B;"
                                 "}"
                                 "QTabBar::tab:top:selected {"
                                 "color: black;"
                                 "background-color: #ED8817;"
                                 "}"
                                 "QTabWidget::pane {"
                                 "border-top: 2px solid #ED8817;"
                                 "border-left: 2px solid #ED8817;"
                                 "border-right: 2px solid #ED8817;"
                                 "border-bottom: 2px solid #ED8817;"
                                 "}");
    ui->Prescription->setStyleSheet("QPushButton { "
                                    "background-color: #ED8817;"
                                    "border-radius:10px;"
                                    "color:#ffffff;"
                                    "outline: 0; "
                                    "font-family:Malgun Gothic;"
                                    "font-size:15px;"
                                    "font-weight:bold;"
                                    "text-decoration:none;"
                                    "min-height: 50px;"
                                    "min-width: 80px;"
                                    "}"
                                    "QPushButton:hover { "
                                    "background-color: #F2A754;"
                                    "border-radius:10px;"
                                    "color:#ffffff;"
                                    "outline: 0; "
                                    "font-family:Malgun Gothic;"
                                    "font-size:15px;"
                                    "font-weight:bold;"
                                    "text-decoration:none;"
                                    "min-height: 50px;"
                                    "min-width: 80px;"
                                    "}"
                                    "QPushButton:disabled { "
                                    "background-color: rgb(132,132,132); "      // 회색
                                    "border-radius:10px;"
                                    "border:1px solid rgb(132,132,132);"
                                    "color:#ffffff;"
                                    "font-family:Malgun Gothic;"
                                    "font-size:15px;"
                                    "font-weight:bold;"
                                    "text-decoration:none;"
                                    "min-width: 80px;"
                                    "outline: 0; "
                                    "}");
    ui->EndTreatment->setStyleSheet("QPushButton { "
                                    "background-color: #ED8817;"
                                    "border-radius:10px;"
                                    "color:#ffffff;"
                                    "outline: 0; "
                                    "font-family:Malgun Gothic;"
                                    "font-size:15px;"
                                    "font-weight:bold;"
                                    "text-decoration:none;"
                                    "min-height: 50px;"
                                    "min-width: 80px;"
                                    "}"
                                    "QPushButton:hover { "
                                    "background-color: #F2A754;"
                                    "border-radius:10px;"
                                    "color:#ffffff;"
                                    "outline: 0; "
                                    "font-family:Malgun Gothic;"
                                    "font-size:15px;"
                                    "font-weight:bold;"
                                    "text-decoration:none;"
                                    "min-height: 50px;"
                                    "min-width: 80px;"
                                    "}"
                                    "QPushButton:disabled { "
                                    "background-color: rgb(132,132,132); "      // 회색
                                    "border-radius:10px;"
                                    "border:1px solid rgb(132,132,132);"
                                    "color:#ffffff;"
                                    "font-family:Malgun Gothic;"
                                    "font-size:15px;"
                                    "font-weight:bold;"
                                    "text-decoration:none;"
                                    "min-width: 80px;"
                                    "outline: 0; "
                                    "}");
    ui->OrigImage->setStyleSheet("QToolButton { "
                                 "background-color: qlineargradient(spread:pad, x1:0.0, y1:0.0, x2:1, "
                                 "y2:1, stop:0 rgba(150, 150, 150, 50), stop:1 rgba(30, 30, 30, 100));"
                                 "border-radius:10px;"
                                 "}"

                                 "QToolButton:hover { "
                                 "background-color: qlineargradient(spread:pad, x1:0.0, y1:0.0, x2:1, "
                                 "y2:1, stop:0 rgba(150, 150, 150, 50), stop:1 rgba(90, 90, 90, 100));"
                                 "border-radius:10px;"
                                 "}");
    ui->ZoomIn->setStyleSheet("QToolButton { "
                              "background-color: qlineargradient(spread:pad, x1:0.0, y1:0.0, x2:1, "
                              "y2:1, stop:0 rgba(150, 150, 150, 50), stop:1 rgba(30, 30, 30, 100));"
                              "border-radius:10px;"
                              "}"

                              "QToolButton:hover { "
                              "background-color: qlineargradient(spread:pad, x1:0.0, y1:0.0, x2:1, "
                              "y2:1, stop:0 rgba(150, 150, 150, 50), stop:1 rgba(90, 90, 90, 100));"
                              "border-radius:10px;"
                              "}");
    ui->ZoomOut->setStyleSheet("QToolButton { "
                               "background-color: qlineargradient(spread:pad, x1:0.0, y1:0.0, x2:1, "
                               "y2:1, stop:0 rgba(150, 150, 150, 50), stop:1 rgba(30, 30, 30, 100));"
                               "border-radius:10px;"
                               "}"

                               "QToolButton:hover { "
                               "background-color: qlineargradient(spread:pad, x1:0.0, y1:0.0, x2:1, "
                               "y2:1, stop:0 rgba(150, 150, 150, 50), stop:1 rgba(90, 90, 90, 100));"
                               "border-radius:10px;"
                               "}");
    ui->VReverse->setStyleSheet("QToolButton { "
                                "background-color: qlineargradient(spread:pad, x1:0.0, y1:0.0, x2:1, "
                                "y2:1, stop:0 rgba(150, 150, 150, 50), stop:1 rgba(30, 30, 30, 100));"
                                "border-radius:10px;"
                                "}"

                                "QToolButton:hover { "
                                "background-color: qlineargradient(spread:pad, x1:0.0, y1:0.0, x2:1, "
                                "y2:1, stop:0 rgba(150, 150, 150, 50), stop:1 rgba(90, 90, 90, 100));"
                                "border-radius:10px;"
                                "}");
    ui->HReverse->setStyleSheet("QToolButton { "
                                "background-color: qlineargradient(spread:pad, x1:0.0, y1:0.0, x2:1, "
                                "y2:1, stop:0 rgba(150, 150, 150, 50), stop:1 rgba(30, 30, 30, 100));"
                                "border-radius:10px;"
                                "}"

                                "QToolButton:hover { "
                                "background-color: qlineargradient(spread:pad, x1:0.0, y1:0.0, x2:1, "
                                "y2:1, stop:0 rgba(150, 150, 150, 50), stop:1 rgba(90, 90, 90, 100));"
                                "border-radius:10px;"
                                "}");
    ui->OrigImage2->setStyleSheet("QToolButton { "
                                  "background-color: qlineargradient(spread:pad, x1:0.0, y1:0.0, x2:1, "
                                  "y2:1, stop:0 rgba(150, 150, 150, 50), stop:1 rgba(30, 30, 30, 100));"
                                  "border-radius:10px;"
                                  "}"

                                  "QToolButton:hover { "
                                  "background-color: qlineargradient(spread:pad, x1:0.0, y1:0.0, x2:1, "
                                  "y2:1, stop:0 rgba(150, 150, 150, 50), stop:1 rgba(90, 90, 90, 100));"
                                  "border-radius:10px;"
                                  "}");
    ui->Brush->setStyleSheet("QToolButton { "
                             "background-color: qlineargradient(spread:pad, x1:0.0, y1:0.0, x2:1, "
                             "y2:1, stop:0 rgba(150, 150, 150, 50), stop:1 rgba(30, 30, 30, 100));"
                             "border-radius:10px;"
                             "}"

                             "QToolButton:hover { "
                             "background-color: qlineargradient(spread:pad, x1:0.0, y1:0.0, x2:1, "
                             "y2:1, stop:0 rgba(150, 150, 150, 50), stop:1 rgba(90, 90, 90, 100));"
                             "border-radius:10px;"
                             "}");
    ui->RightRotate->setStyleSheet("QToolButton { "
                                   "background-color: qlineargradient(spread:pad, x1:0.0, y1:0.0, x2:1, "
                                   "y2:1, stop:0 rgba(150, 150, 150, 50), stop:1 rgba(30, 30, 30, 100));"
                                   "border-radius:10px;"
                                   "}"

                                   "QToolButton:hover { "
                                   "background-color: qlineargradient(spread:pad, x1:0.0, y1:0.0, x2:1, "
                                   "y2:1, stop:0 rgba(150, 150, 150, 50), stop:1 rgba(90, 90, 90, 100));"
                                   "border-radius:10px;"
                                   "}");
    ui->LeftRotate->setStyleSheet("QToolButton { "
                                  "background-color: qlineargradient(spread:pad, x1:0.0, y1:0.0, x2:1, "
                                  "y2:1, stop:0 rgba(150, 150, 150, 50), stop:1 rgba(30, 30, 30, 100));"
                                  "border-radius:10px;"
                                  "}"

                                  "QToolButton:hover { "
                                  "background-color: qlineargradient(spread:pad, x1:0.0, y1:0.0, x2:1, "
                                  "y2:1, stop:0 rgba(150, 150, 150, 50), stop:1 rgba(90, 90, 90, 100));"
                                  "border-radius:10px;"
                                  "}");
    ui->Blur->setStyleSheet("QToolButton { "
                            "background-color: qlineargradient(spread:pad, x1:0.0, y1:0.0, x2:1, "
                            "y2:1, stop:0 rgba(150, 150, 150, 50), stop:1 rgba(30, 30, 30, 100));"
                            "border-radius:10px;"
                            "}"

                            "QToolButton:hover { "
                            "background-color: qlineargradient(spread:pad, x1:0.0, y1:0.0, x2:1, "
                            "y2:1, stop:0 rgba(150, 150, 150, 50), stop:1 rgba(90, 90, 90, 100));"
                            "border-radius:10px;"
                            "}");
    ui->Lines->setStyleSheet("QToolButton { "
                             "background-color: qlineargradient(spread:pad, x1:0.0, y1:0.0, x2:1, "
                             "y2:1, stop:0 rgba(150, 150, 150, 50), stop:1 rgba(30, 30, 30, 100));"
                             "border-radius:10px;"
                             "}"

                             "QToolButton:hover { "
                             "background-color: qlineargradient(spread:pad, x1:0.0, y1:0.0, x2:1, "
                             "y2:1, stop:0 rgba(150, 150, 150, 50), stop:1 rgba(90, 90, 90, 100));"
                             "border-radius:10px;"
                             "}");
    ui->Sharpening->setStyleSheet("QToolButton { "
                                  "background-color: qlineargradient(spread:pad, x1:0.0, y1:0.0, x2:1, "
                                  "y2:1, stop:0 rgba(150, 150, 150, 50), stop:1 rgba(30, 30, 30, 100));"
                                  "border-radius:10px;"
                                  "}"

                                  "QToolButton:hover { "
                                  "background-color: qlineargradient(spread:pad, x1:0.0, y1:0.0, x2:1, "
                                  "y2:1, stop:0 rgba(150, 150, 150, 50), stop:1 rgba(90, 90, 90, 100));"
                                  "border-radius:10px;"
                                  "}");
    ui->HistEqual->setStyleSheet("QToolButton { "
                                 "background-color: qlineargradient(spread:pad, x1:0.0, y1:0.0, x2:1, "
                                 "y2:1, stop:0 rgba(150, 150, 150, 50), stop:1 rgba(30, 30, 30, 100));"
                                 "border-radius:10px;"
                                 "}"

                                 "QToolButton:hover { "
                                 "background-color: qlineargradient(spread:pad, x1:0.0, y1:0.0, x2:1, "
                                 "y2:1, stop:0 rgba(150, 150, 150, 50), stop:1 rgba(90, 90, 90, 100));"
                                 "border-radius:10px;"
                                 "}");
    ui->Reverse->setStyleSheet("QToolButton { "
                               "background-color: qlineargradient(spread:pad, x1:0.0, y1:0.0, x2:1, "
                               "y2:1, stop:0 rgba(150, 150, 150, 50), stop:1 rgba(30, 30, 30, 100));"
                               "border-radius:10px;"
                               "}"

                               "QToolButton:hover { "
                               "background-color: qlineargradient(spread:pad, x1:0.0, y1:0.0, x2:1, "
                               "y2:1, stop:0 rgba(150, 150, 150, 50), stop:1 rgba(90, 90, 90, 100));"
                               "border-radius:10px;"
                               "}");
    ui->Freehand->setStyleSheet("QToolButton { "
                                "background-color: qlineargradient(spread:pad, x1:0.0, y1:0.0, x2:1, "
                                "y2:1, stop:0 rgba(150, 150, 150, 50), stop:1 rgba(30, 30, 30, 100));"
                                "border-radius:10px;"
                                "}"

                                "QToolButton:hover { "
                                "background-color: qlineargradient(spread:pad, x1:0.0, y1:0.0, x2:1, "
                                "y2:1, stop:0 rgba(150, 150, 150, 50), stop:1 rgba(90, 90, 90, 100));"
                                "border-radius:10px;"
                                "}");
    ui->Laser->setStyleSheet("QToolButton { "
                             "background-color: qlineargradient(spread:pad, x1:0.0, y1:0.0, x2:1, "
                             "y2:1, stop:0 rgba(150, 150, 150, 50), stop:1 rgba(30, 30, 30, 100));"
                             "border-radius:10px;"
                             "}"

                             "QToolButton:hover { "
                             "background-color: qlineargradient(spread:pad, x1:0.0, y1:0.0, x2:1, "
                             "y2:1, stop:0 rgba(150, 150, 150, 50), stop:1 rgba(90, 90, 90, 100));"
                             "border-radius:10px;"
                             "}");
    ui->Rectangle->setStyleSheet("QToolButton { "
                                 "background-color: qlineargradient(spread:pad, x1:0.0, y1:0.0, x2:1, "
                                 "y2:1, stop:0 rgba(150, 150, 150, 50), stop:1 rgba(30, 30, 30, 100));"
                                 "border-radius:10px;"
                                 "}"

                                 "QToolButton:hover { "
                                 "background-color: qlineargradient(spread:pad, x1:0.0, y1:0.0, x2:1, "
                                 "y2:1, stop:0 rgba(150, 150, 150, 50), stop:1 rgba(90, 90, 90, 100));"
                                 "border-radius:10px;"
                                 "}");
    ui->Ellipse->setStyleSheet("QToolButton { "
                               "background-color: qlineargradient(spread:pad, x1:0.0, y1:0.0, x2:1, "
                               "y2:1, stop:0 rgba(150, 150, 150, 50), stop:1 rgba(30, 30, 30, 100));"
                               "border-radius:10px;"
                               "}"

                               "QToolButton:hover { "
                               "background-color: qlineargradient(spread:pad, x1:0.0, y1:0.0, x2:1, "
                               "y2:1, stop:0 rgba(150, 150, 150, 50), stop:1 rgba(90, 90, 90, 100));"
                               "border-radius:10px;"
                               "}");
    ui->Delete->setStyleSheet("QToolButton { "
                              "background-color: qlineargradient(spread:pad, x1:0.0, y1:0.0, x2:1, "
                              "y2:1, stop:0 rgba(150, 150, 150, 50), stop:1 rgba(30, 30, 30, 100));"
                              "border-radius:10px;"
                              "}"

                              "QToolButton:hover { "
                              "background-color: qlineargradient(spread:pad, x1:0.0, y1:0.0, x2:1, "
                              "y2:1, stop:0 rgba(150, 150, 150, 50), stop:1 rgba(90, 90, 90, 100));"
                              "border-radius:10px;"
                              "}");
    ui->Cursor->setStyleSheet("QToolButton { "
                              "background-color: qlineargradient(spread:pad, x1:0.0, y1:0.0, x2:1, "
                              "y2:1, stop:0 rgba(150, 150, 150, 50), stop:1 rgba(30, 30, 30, 100));"
                              "border-radius:10px;"
                              "}"

                              "QToolButton:hover { "
                              "background-color: qlineargradient(spread:pad, x1:0.0, y1:0.0, x2:1, "
                              "y2:1, stop:0 rgba(150, 150, 150, 50), stop:1 rgba(90, 90, 90, 100));"
                              "border-radius:10px;"
                              "}");
    ui->LengthMeasurement->setStyleSheet("QToolButton { "
                                         "background-color: qlineargradient(spread:pad, x1:0.0, y1:0.0, x2:1, "
                                         "y2:1, stop:0 rgba(150, 150, 150, 50), stop:1 rgba(30, 30, 30, 100));"
                                         "border-radius:10px;"
                                         "}"

                                         "QToolButton:hover { "
                                         "background-color: qlineargradient(spread:pad, x1:0.0, y1:0.0, x2:1, "
                                         "y2:1, stop:0 rgba(150, 150, 150, 50), stop:1 rgba(90, 90, 90, 100));"
                                         "border-radius:10px;"
                                         "}");
    ui->Angle->setStyleSheet("QToolButton { "
                             "background-color: qlineargradient(spread:pad, x1:0.0, y1:0.0, x2:1, "
                             "y2:1, stop:0 rgba(150, 150, 150, 50), stop:1 rgba(30, 30, 30, 100));"
                             "border-radius:10px;"
                             "}"

                             "QToolButton:hover { "
                             "background-color: qlineargradient(spread:pad, x1:0.0, y1:0.0, x2:1, "
                             "y2:1, stop:0 rgba(150, 150, 150, 50), stop:1 rgba(90, 90, 90, 100));"
                             "border-radius:10px;"
                             "}");
    ui->Capture->setStyleSheet("QToolButton { "
                               "background-color: qlineargradient(spread:pad, x1:0.0, y1:0.0, x2:1, "
                               "y2:1, stop:0 rgba(150, 150, 150, 50), stop:1 rgba(30, 30, 30, 100));"
                               "border-radius:10px;"
                               "}"

                               "QToolButton:hover { "
                               "background-color: qlineargradient(spread:pad, x1:0.0, y1:0.0, x2:1, "
                               "y2:1, stop:0 rgba(150, 150, 150, 50), stop:1 rgba(90, 90, 90, 100));"
                               "border-radius:10px;"
                               "}");
    ui->Implant->setStyleSheet("QToolButton { "
                               "background-color: qlineargradient(spread:pad, x1:0.0, y1:0.0, x2:1, "
                               "y2:1, stop:0 rgba(150, 150, 150, 50), stop:1 rgba(30, 30, 30, 100));"
                               "border-radius:10px;"
                               "}"

                               "QToolButton:hover { "
                               "background-color: qlineargradient(spread:pad, x1:0.0, y1:0.0, x2:1, "
                               "y2:1, stop:0 rgba(150, 150, 150, 50), stop:1 rgba(90, 90, 90, 100));"
                               "border-radius:10px;"
                               "}");

    imageView->setGeometry(6, 6, 600, 600);
    imageView->setSizeAdjustPolicy(QAbstractScrollArea::AdjustToContents);
    imageView->setDragMode(QGraphicsView::NoDrag);
    ui->gridLayout->addWidget(imageView);
    imageView->setAlignment(Qt::AlignCenter);
    imageView->setScene(imageScene);

    // 리스트 위젯에서 이미지 클릭 시 GraphicsView에 해당 이미지 추가
    connect(ui->listWidget, SIGNAL(itemDoubleClicked(QListWidgetItem*)), this, SLOT(selectItem(QListWidgetItem*)));

    // 아이콘 클릭 시 해당 함수 실행
    connect(ui->ZoomIn, SIGNAL(clicked()), this, SLOT(ZoomIn()));
    connect(ui->ZoomOut, SIGNAL(clicked()), this, SLOT(ZoomOut()));
    connect(ui->LeftRotate, SIGNAL(clicked()), this, SLOT(LeftRotate()));
    connect(ui->RightRotate, SIGNAL(clicked()), this, SLOT(RightRotate()));
    connect(ui->Brush, SIGNAL(clicked()), this, SLOT(BrushColor()));
    connect(ui->OrigImage, SIGNAL(clicked()), this, SLOT(OrigImage()));
    connect(ui->OrigImage2, SIGNAL(clicked()), this, SLOT(OrigImage()));
    connect(ui->Brightness, SIGNAL(valueChanged(int)), this, SLOT(Brightness(int)));
    connect(ui->VReverse, SIGNAL(clicked()), this, SLOT(VReverse()));
    connect(ui->HReverse, SIGNAL(clicked()), this, SLOT(HReverse()));
    connect(ui->Blur, SIGNAL(clicked()), this, SLOT(Blur()));
    connect(ui->Reverse, SIGNAL(clicked()), this, SLOT(Reverse()));
    connect(ui->Contrast, SIGNAL(valueChanged(int)), this, SLOT(Contrast(int)));
    connect(ui->HistEqual, SIGNAL(clicked()), this, SLOT(HistEqual()));
    connect(ui->Sharpening, SIGNAL(clicked()), this, SLOT(Sharpening()));
    connect(ui->Thickness, SIGNAL(valueChanged(int)), this, SLOT(Thickness(int)));
    connect(ui->Lines, SIGNAL(clicked()), this, SLOT(Lines()));
    connect(ui->Freehand, SIGNAL(clicked()), this, SLOT(Freehand()));
    connect(ui->Laser, SIGNAL(clicked()), this, SLOT(Laser()));
    connect(ui->Cursor, SIGNAL(clicked()), this, SLOT(Cursor()));
    connect(ui->Delete, SIGNAL(clicked()), this, SLOT(DeleteItem()));
    connect(ui->Ellipse, SIGNAL(clicked()), this, SLOT(Ellipse()));
    connect(ui->Rectangle, SIGNAL(clicked()), this, SLOT(RectangleItem()));
    connect(ui->TextBox, SIGNAL(clicked()), this, SLOT(TextBox()));
    connect(ui->LengthMeasurement, SIGNAL(clicked()), this, SLOT(Length()));
    connect(ui->Angle, SIGNAL(clicked()), this, SLOT(Angle()));
    connect(ui->Capture, SIGNAL(clicked()), this, SLOT(Capture()));
    connect(ui->Implant, SIGNAL(clicked()), this, SLOT(Implant()));
    connect(ui->Gamma, SIGNAL(valueChanged(int)), this, SLOT(Gamma(int)));

    // GraphicsView에 펜 색상, 펜 두께, 선인지 도형인지를 구분하여 시그널 전송
    connect(this, SIGNAL(SendThickness(int)), imageScene, SLOT(ReceiveThickness(int)));
    connect(this, SIGNAL(SendBrushColor(QColor)), imageScene, SLOT(ReceiveBrushColor(QColor)));
    connect(this, SIGNAL(SendType(int)), imageScene, SLOT(ReceiveType(int)));

    // 입력한 텍스트 데이터를 ImageScene 클래스로 전송
    connect(this, SIGNAL(SendText(QString)), imageScene, SLOT(ReceiveText(QString)));

    // 실제 길이를 측정하기 위해 이미지 원본 사이즈와 scaled된 이미지 사이즈를 ImageScene 클래스로 전송
    connect(this, SIGNAL(SendLength(int, int, int, int, QString)), imageScene, SLOT(ReceiveLength(int, int, int, int, QString)));

    // 텍스트 사이즈를 변경하기 위해 시그널 전송
    connect(this, SIGNAL(SendFontSize(int)), imageScene, SLOT(ReceiveThickness(int)));

    // GraphicsScene에서 측정한 캡쳐되는 영역의 좌표를 불러오는 시그널-슬롯
    connect(imageScene, SIGNAL(SendCapturePos(QPointF, QPointF)), this, SLOT(receiveCapturePos(QPointF, QPointF)));

    // 처방전 작성 버튼 클릭 시 처방전 클래스로 의사 정보, 환자 정보를 전송
    connect(this, SIGNAL(sendPrescription(QString, QString, QString, QString, QString)),
            m_prescription, SLOT(receivePrescription(QString, QString, QString, QString, QString)));

    // 처방전 클래스에서 처방전 작성 완료 되면 해당 내용을 서버로 보내주기 위한 과정
    connect(m_prescription, SIGNAL(sendPrescriptionFinish(QString)), this, SLOT(receivePrescriptionFinish(QString)));

    // 콤보박스를 통해 임플란트 타입 선택 시 해당 타입에 따라 다른 임플란트가 식립되도록 하기 위한 시그널-슬롯
    connect(this, SIGNAL(sendImplantType(int)), imageScene, SLOT(ReceiveImplantType(int)));

    // 서버로 전송한 처방전이 DB에 저장이 안전하게 되었는지 여부를 알려주기 위한 시그널-슬롯
    connect(this, SIGNAL(sendPrescriptionYesOrNot(QString)), m_prescription, SLOT(receivePrescriptionYesOrNot(QString)));

//        reloadImages();
}

ImageAlbum::~ImageAlbum()
{
    delete ui;
}

// 서버로부터 다운받은 환자의 이미지들을 listWidget에 띄워주는 함수
void ImageAlbum::reloadImages()
{
//        QDir dir("./Photo");
    QDir dir("./Image");
    QStringList filters;
    filters << "*.png" << "*.jpg" << "*.bmp" << "*.gif";
    QFileInfoList fileInfoList = dir.entryInfoList(filters, QDir::Files | QDir::NoDotAndDotDot);

    ui->listWidget->clear();

    // listWidget에 서버로부터 받아온 환자의 이미지들을 모두 띄워줌
    for(int i = 0; i < fileInfoList.count(); i++) {
        QListWidgetItem* item = new QListWidgetItem(QIcon(dir.path() + "/" + fileInfoList.at(i).fileName()), NULL, ui->listWidget); //, QListWidgetItem::UserType);
        item->setStatusTip(dir.path() + "/" + fileInfoList.at(i).fileName());
        ui->listWidget->addItem(item);
    };

    // 진료 시작 버튼 클릭 시 처방전 작성 버튼, 진료 종료 버튼 활성화
    ui->Prescription->setDisabled(false);
    ui->EndTreatment->setDisabled(false);
}

// 임플란트 식립 버튼 클릭 시 ImageScene으로 해당 시그널 전송
void ImageAlbum::Implant()
{
    // 이미지가 선택되지 않았다면 예외처리
    if(selectImage.isNull()){
        QMessageBox:: critical(messagebox, "경고", "이미지를 선택하세요");
        return;
    }
    emit SendType(ImageScene::DrawType::Implant);
}

// 캡처 버튼 클릭 시 ImageScene으로 해당 시그널 전송
void ImageAlbum::Capture()
{
    // 이미지가 선택되지 않았다면 예외처리
    if(selectImage.isNull()){
        QMessageBox:: critical(messagebox, "경고", "이미지를 선택하세요");
        return;
    }
    emit SendType(ImageScene::DrawType::Capture);
}

// 각도 측정 버튼 클릭 시 ImageScene으로 해당 시그널 전송
void ImageAlbum::Angle()
{
    // 이미지가 선택되지 않았다면 예외처리
    if(selectImage.isNull()){
        QMessageBox:: critical(messagebox, "경고", "이미지를 선택하세요");
        return;
    }
    emit SendType(ImageScene::DrawType::Angle);
}

// 길이 측정 버튼 클릭 시 ImageScene으로 해당 시그널 및 선택된 이미지의 길이 전송
void ImageAlbum::Length()
{
    // 이미지가 선택되지 않았다면 예외처리
    if(selectImage.isNull()){
        QMessageBox:: critical(messagebox, "경고", "이미지를 선택하세요");
        return;
    }
    emit SendType(ImageScene::DrawType::Length);

    // 이미지의 원본 사이즈와 scaled된 사이즈를 ImageScene 클래스로 전송
    int origWidth = selectImage.width();
    int origHeight = selectImage.height();
    int sceneWidth = imageScene->width();
    int sceneHeight = imageScene->height();
    emit SendLength(origWidth, origHeight, sceneWidth, sceneHeight, imageType);
}

// 텍스트 입력 버튼 클릭 시 ImageScene으로 해당 시그널 및 입력된 텍스트 전송
void ImageAlbum::TextBox()
{
    // 이미지가 선택되지 않았다면 예외처리
    if(selectImage.isNull()){
        QMessageBox:: critical(messagebox, "경고", "이미지를 선택하세요");
        return;
    }

    // lineEdit에 입력한 텍스트 데이터를 ImageScene 클래스로 전송
    emit SendText(ui->lineEdit->text());
    emit SendType(ImageScene::DrawType::Text);
    ui->lineEdit->clear();
}

// 사각형 삽입 버튼 클릭 시 ImageScene으로 해당 시그널 전송
void ImageAlbum::RectangleItem()
{
    // 이미지가 선택되지 않았다면 예외처리
    if(selectImage.isNull()){
        QMessageBox:: critical(messagebox, "경고", "이미지를 선택하세요");
        return;
    }

    emit SendType(ImageScene::DrawType::Rectangle);
}

// 커서 버튼 클릭 시 ImageScene으로 해당 시그널 전송
void ImageAlbum::Cursor()
{
    // 이미지가 선택되지 않았다면 예외처리
    if(selectImage.isNull()){
        QMessageBox:: critical(messagebox, "경고", "이미지를 선택하세요");
        return;
    }

    emit SendType(ImageScene::DrawType::Cursor);
}

// 삭제 버튼 클릭 시 ImageScene으로 해당 시그널 전송
void ImageAlbum::DeleteItem()
{
    // 이미지가 선택되지 않았다면 예외처리
    if(selectImage.isNull()){
        QMessageBox:: critical(messagebox, "경고", "이미지를 선택하세요");
        return;
    }

    emit SendType(ImageScene::DrawType::Delete);
}

// 원 삽입 버튼 클릭 시 ImageScene으로 해당 시그널 전송
void ImageAlbum::Ellipse()
{
    // 이미지가 선택되지 않았다면 예외처리
    if(selectImage.isNull()){
        QMessageBox:: critical(messagebox, "경고", "이미지를 선택하세요");
        return;
    }

    emit SendType(ImageScene::DrawType::Ellipse);
}

// 레이저 버튼 클릭 시 ImageScene으로 해당 시그널 전송
void ImageAlbum::Laser()
{
    // 이미지가 선택되지 않았다면 예외처리
    if(selectImage.isNull()){
        QMessageBox:: critical(messagebox, "경고", "이미지를 선택하세요");
        return;
    }

    emit SendType(ImageScene::DrawType::Laser);
}


// 그리기 버튼 클릭 시 ImageScene으로 해당 시그널 전송
void ImageAlbum::Lines()
{
    // 이미지가 선택되지 않았다면 예외처리
    if(selectImage.isNull()){
        QMessageBox::critical(messagebox, "경고", "이미지를 선택하세요");
        return;
    }

    emit SendType(ImageScene::DrawType::Lines);
}

// 폐곡선 버튼 클릭 시 ImageScene으로 해당 시그널 전송
void ImageAlbum::Freehand()
{
    // 이미지가 선택되지 않았다면 예외처리
    if(selectImage.isNull()){
        QMessageBox::critical(messagebox, "경고", "이미지를 선택하세요");
        return;
    }

    emit SendType(ImageScene::DrawType::FreeHand);
}

// ColorDialog를 통해 색상이 선택되면 ImageScene으로 색상값 전송
void ImageAlbum::BrushColor()
{
    paintColor = QColorDialog::getColor(paintColor, this);
    emit SendBrushColor(paintColor);
}

// 줌인 기능
void ImageAlbum::ZoomIn()
{
    imageView->scale(1.25, 1.25);
}

// 줌아웃 기능
void ImageAlbum::ZoomOut()
{
    imageView->scale(0.8, 0.8);
}

// 왼쪽 방향으로 90도 회전 기능
void ImageAlbum::LeftRotate()
{
    imageView->rotate(-90);
}

// 오른쪽 방향으로 90도 회전 기능
void ImageAlbum::RightRotate()
{
    imageView->rotate(90);
}

// 리셋 버튼 클릭 시 원본 이미지 설정
void ImageAlbum::OrigImage()
{
    if(selectImage.isNull()){
        QMessageBox:: critical(messagebox, "경고", "이미지를 선택하세요");
        return;
    }

    // 리셋 버튼 클릭 시 설정 값들 모두 초기화
    emit SendType(ImageScene::DrawType::Clear);
    imageView->resetTransform();
    imageScene->setBackgroundBrush(Qt::white);
    ui->Brightness->setSliderPosition(0);
    ui->Contrast->setSliderPosition(0);
    ui->Gamma->setSliderPosition(10);
    ui->Thickness->setValue(6);

    // listWidget에서 더블 클릭 시 저장해놓았던 변수에서 원본 이미지 꺼내옴
    selectImage = QPixmap(orignal->statusTip()).toImage();

    // 이미지 픽셀의 Histogram 출력
    Histogram();

    imageScene->clear();
    QSize size = imageView->viewport()->size();
    QGraphicsItem *i = imageScene->addPixmap(QPixmap(orignal->statusTip()).scaled(size, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    i->setToolTip("GraphicsScene");
    imageView->setAlignment(Qt::AlignCenter);
    imageScene->setSceneRect(i->sceneBoundingRect());
}

// listWidget에서 이미지 더블클릭 시 해당 이미지가 graphicsView에 출력
void ImageAlbum::selectItem(QListWidgetItem* item)
{
    // 새 이미지 더블 클릭 시 설정 값들 모두 초기화
    emit SendType(ImageScene::DrawType::Clear);
    imageView->resetTransform();
    imageView->setBackgroundBrush(Qt::white);
    imageScene->setBackgroundBrush(Qt::white);
    ui->Brightness->setSliderPosition(0);
    ui->Contrast->setSliderPosition(0);
    ui->Gamma->setSliderPosition(10);
    ui->Thickness->setValue(6);

    // 원본 이미지(orignal), 누적될 이미지 따로 저장(selectImage)
    orignal = item;
    selectImage = QImage(ui->listWidget->currentItem()->statusTip());

    // 이미지 픽셀의 Histogram 출력
    Histogram();

    // 이미지를 선택하면 해당 이미지에 대한 날짜 및 종류가 보여짐
    QString Date = ui->listWidget->currentItem()->statusTip().split("/")[2].last(12).first(8);
    Date = Date.first(4) + "년 " + Date.left(6).right(2) + "월 " + Date.last(2) + "일";
    ui->ImageInfo->setText(" " + ui->listWidget->currentItem()->statusTip().split("/")[2].first(4));
    ui->PhotoDate->setText(" " + Date);

    // 이미지 타입(Ceph, Pano)을 변수에 저장
    imageType = ui->listWidget->currentItem()->statusTip().split("/")[2].first(4);

    imageScene->clear();
    QSize size = imageView->viewport()->size();
    QGraphicsItem *i = imageScene->addPixmap(QPixmap(item->statusTip()).scaled(size, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    i->setToolTip("GraphicsScene");
    imageView->setAlignment(Qt::AlignCenter);
    imageScene->setSceneRect(i->sceneBoundingRect());

    // 이미지의 원본 사이즈와 scaled된 사이즈, Ceph인지 Pano인지를 함께 전송
    emit SendLength(selectImage.width(), selectImage.height(), imageScene->width(), imageScene->height(), imageType);
}

// y축 방면으로 이미지 반전 기능
void ImageAlbum::VReverse()
{
    if(selectImage.isNull()){
        QMessageBox:: critical(messagebox, "경고", "이미지를 선택하세요");
        return;
    }
    imageView->scale(-1, 1);
}

// x축 방면으로 이미지 반전 기능
void ImageAlbum::HReverse()
{
    if(selectImage.isNull()){
        QMessageBox:: critical(messagebox, "경고", "이미지를 선택하세요");
        return;
    }
    imageView->scale(1, -1);

}

// GraphicsScene 클래스에서 드래그로 잡은 사각형의 영역에 맞게 캡쳐하여 출력하는 함수
void ImageAlbum::receiveCapturePos(QPointF startPos, QPointF endPos)
{
    if(selectImage.isNull()){
        QMessageBox:: critical(messagebox, "경고", "이미지를 선택하세요");
        return;
    }

    QPoint topLeft = imageView->mapFromScene(startPos);
    QPoint bottomRight = imageView->mapFromScene(endPos);

    // 원본 이미지에서 주어진 사각형의 사이즈만큼 잘라서 확인
    QPixmap buf = imageView->grab(QRect(topLeft, bottomRight).normalized());
    QImage image_capture = buf.toImage();
    QSize size = imageView->viewport()->size();

    imageScene->clear();
    buf.setDevicePixelRatio(1);
    QGraphicsItem *i = imageScene->addPixmap(buf.scaled(size, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    i->setToolTip("GraphicsScene");
    imageView->setAlignment(Qt::AlignCenter);
    imageScene->setSceneRect(i->sceneBoundingRect());
    selectImage = image_capture.convertToFormat(QImage::Format_Grayscale8).copy();
    Histogram();

    emit SendType(ImageScene::DrawType::Clear);
}

// 슬라이더에 따라 이미지의 밝기값이 변경되는 함수
void ImageAlbum::Brightness(int value)
{
    if(selectImage.isNull()){
        QMessageBox:: critical(messagebox, "경고", "이미지를 선택하세요");
        return;
    }

    cv::Mat out;

    // 선택된 이미지를 gray-scale 이미지로 변경
    QImage image = selectImage.convertToFormat(QImage::Format_Grayscale8);

    // 선택된 이미지를 1채널 Mat 배열로 설정
    cv::Mat in(image.height(),
               image.width(),
               CV_8UC1,    //uchar
               image.bits(),
               image.bytesPerLine());

    /*
    void Mat::convertTo( OutputArray m, int rtype, double alpha=1, double beta=0 ) const;
    • m
    출력 행렬. 만약 m 행렬이 적절한 크기와 타입이 아닌 경우 행렬 원소 데이터를 새로 할당합니다.
    • rtype
    원하는 출력 행렬의 타입. 만약 rtype이 음수이면 출력 행렬은 입력 행렬과 같은 타입을 갖습니다.
    • alpha
    추가적으로 곱할 값
    • beta
    추가적으로 더할 값*/


    // 선택된 이미지의 픽셀값에 슬라이더 변경값을 더해줌, 슬라이더를 놓고 다른 효과를 설정 후
    // 다시 밝기 조절 시 슬라이더를 놨을 때의 값을 빼줌
    in.convertTo(out, -1, 1, value - Brightvalue);

    // 설정한 Mat 클래스의 배열을 다시 이미지 형태로 변경
    image_brightness = QImage(
                out.data,
                out.cols,
                out.rows,
                out.step,
                QImage::Format_Grayscale8).copy();

    QSize size = imageView->viewport()->size();
    QPixmap buf = QPixmap::fromImage(image_brightness);
    imageView->setAlignment(Qt::AlignCenter);

    foreach(auto item, imageScene->items()) {
        QGraphicsPixmapItem *i = dynamic_cast<QGraphicsPixmapItem*>(item);
        if(i != nullptr && i->toolTip() == "GraphicsScene") {
            i->setScale(1.0);
            i->setPixmap(buf.scaled(size, Qt::KeepAspectRatio, Qt::SmoothTransformation));
            imageScene->setSceneRect(i->sceneBoundingRect());
        }
    }
}

// 슬라이더를 놓을 때 해당 슬라이더의 값을 Brightvalue에 저장
void ImageAlbum::on_Brightness_sliderReleased()
{
    Brightvalue = ui->Brightness->value();
    selectImage = image_brightness.convertToFormat(selectImage.format()).copy();
    Histogram();
}

// 이미지의 픽셀값에 히스토그램 평활화를 실행
void ImageAlbum::HistEqual()
{
    if(selectImage.isNull()){
        QMessageBox:: critical(messagebox, "경고", "이미지를 선택하세요");
        return;
    }

    // 선택된 이미지를 gray-scale 이미지로 변경
    QImage image = selectImage.convertToFormat(QImage::Format_Grayscale8);

    // 선택된 이미지를 1채널 Mat 배열로 설정
    cv::Mat in = cv::Mat(
                image.height(),
                image.width(),
                CV_8UC1,
                image.bits(),
                image.bytesPerLine());
    cv::Mat Hist;

    // 선택된 이미지에 히스토그램 평활화 실행
    equalizeHist(in, Hist);

    // 설정한 Mat 클래스의 배열을 다시 이미지 형태로 변경
    QImage image_Histogram(
                Hist.data,
                Hist.cols,
                Hist.rows,
                Hist.step,
                QImage::Format_Grayscale8);

    QSize size = imageView->viewport()->size();
    QPixmap buf = QPixmap::fromImage(image_Histogram);
    imageView->setAlignment(Qt::AlignCenter);

    foreach(auto item, imageScene->items()) {
        QGraphicsPixmapItem *i = dynamic_cast<QGraphicsPixmapItem*>(item);
        if(i != nullptr && i->toolTip() == "GraphicsScene") {
            i->setScale(1.0);
            i->setPixmap(buf.scaled(size, Qt::KeepAspectRatio, Qt::SmoothTransformation));
            imageScene->setSceneRect(i->sceneBoundingRect());
        }
    }
    selectImage = image_Histogram.convertToFormat(selectImage.format()).copy();
    Histogram();
}

// 이미지의 픽셀값을 히스토그램 형태로 Chart에 출력
void ImageAlbum::Histogram()
{
    // Chart 초기화
    view->resetTransform();
    chart->resetTransform();
    chart->removeAllSeries();

    QImage image = selectImage.convertToFormat(QImage::Format_Grayscale8);

    int hbins = 255;
    int channels[] = { 0 };
    int histSize[] = { hbins };
    float hranges[] = { 0, 255 };
    const float* ranges[] = { hranges };

    MatND Hist;
    cv::Mat in = cv::Mat(
                image.height(),
                image.width(),
                CV_8UC1,
                image.bits(),
                image.bytesPerLine());

    // 입력 이미지 배열의 픽셀값을 Histogram
    // calcHist(원본영상 배열, 원본 영상 개수, 히스토그램 계산에 사용되는 차원 목록,
    // 특정 영역만 계산하기 위한 마스크 행렬, 계산된 히스토그램이 출력되는 배열,
    // 히스토그램 차원 수, 각 차원의 히스토그램 배열 크기, 히스토그램 범위,
    // 히스토그램이 균일한지, 누적 플래그)
    // Mat() 전달 시 아무런 동작 X
    calcHist(&in, 1, channels, Mat(),
             Hist, 1, histSize, ranges,
             true,
             false);

    // 히스토그램 정규화
    normalize(Hist, Hist, 0, 255, NORM_MINMAX);

    QLineSeries *series = new QLineSeries;
    series->setName("Image");
    series->setColor(QColor(150, 150, 150));

    for(int j = 0; j < Hist.rows; ++j){
        *series << QPoint(j, Hist.at<float>(j));
    }

    chart->setAnimationOptions(QChart::AllAnimations);
    chart->setFont(QFont("", 3, 3, false));
    chart->addSeries(series);
    chart->createDefaultAxes();
    chart->setDropShadowEnabled(true);
    chart->setBackgroundVisible(false);
    chart->legend()->hide();
}

// 이미지를 반전시켜 밝으면 어둡게, 어두우면 밝게 변경해주는 함수
void ImageAlbum::Reverse()
{
    if(selectImage.isNull()){
        QMessageBox:: critical(messagebox, "경고", "이미지를 선택하세요");
        return;
    }

    QImage image = selectImage.convertToFormat(QImage::Format_Grayscale8);
    cv::Mat in = cv::Mat(
                image.height(),
                image.width(),
                CV_8UC1,
                image.bits(),
                image.bytesPerLine());
    Mat out;
    // 픽셀값을 최대값에서 빼주면 어두운 부분은 밝게, 밝은 부분은 어둡게 변경
    out = 255 - in;

    QImage image_Reverse(
                out.data,
                out.cols,
                out.rows,
                out.step,
                QImage::Format_Grayscale8);

    QSize size = imageView->viewport()->size();
    QPixmap buf = QPixmap::fromImage(image_Reverse);
    imageView->setAlignment(Qt::AlignCenter);

    foreach(auto item, imageScene->items()) {
        QGraphicsPixmapItem *i = dynamic_cast<QGraphicsPixmapItem*>(item);
        if(i != nullptr && i->toolTip() == "GraphicsScene") {
            i->setScale(1.0);
            i->setPixmap(buf.scaled(size, Qt::KeepAspectRatio, Qt::SmoothTransformation));
            imageScene->setSceneRect(i->sceneBoundingRect());
        }
    }
    selectImage = image_Reverse.convertToFormat(selectImage.format()).copy();
    Histogram();
}

// 슬라이더를 통해 이미지의 대비를 변경해주는 함수
void ImageAlbum::Contrast(int value)
{
    if(selectImage.isNull()){
        QMessageBox:: critical(messagebox, "경고", "이미지를 선택하세요");
        return;
    }

    QImage image = selectImage.convertToFormat(QImage::Format_Grayscale8);
    cv::Mat in = cv::Mat(
                image.height(),
                image.width(),
                CV_8UC1,
                image.bits(),
                image.bytesPerLine());

    double alpha = (double)value / 10;
    // 이미지 픽셀 값에 해당 슬라이더의 double 형 값을 곱해줌
    //    in.convertTo(out, -1, alpha, 0);
    Mat out = in + (in - 128) * alpha;

    image_Contrast = QImage(
                out.data,
                out.cols,
                out.rows,
                out.step,
                QImage::Format_Grayscale8).copy();

    QSize size = imageView->viewport()->size();
    QPixmap buf = QPixmap::fromImage(image_Contrast);
    imageView->setAlignment(Qt::AlignCenter);

    foreach(auto item, imageScene->items()) {
        QGraphicsPixmapItem *i = dynamic_cast<QGraphicsPixmapItem*>(item);
        if(i != nullptr && i->toolTip() == "GraphicsScene") {
            i->setScale(1.0);
            i->setPixmap(buf.scaled(size, Qt::KeepAspectRatio, Qt::SmoothTransformation));
            imageScene->setSceneRect(i->sceneBoundingRect());
        }
    }
}

// 슬라이더를 놓으면 누적 이미지에 대비가 변경된 값이 저장
void ImageAlbum::on_Contrast_sliderReleased()
{
    selectImage = image_Contrast.convertToFormat(selectImage.format()).copy();
    Histogram();
}

// 이미지에 블러 효과를 넣어주는 함수
void ImageAlbum::Blur()
{
    if(selectImage.isNull()){
        QMessageBox:: critical(messagebox, "경고", "이미지를 선택하세요");
        return;
    }

    QImage image = selectImage.convertToFormat(QImage::Format_Grayscale8);
    cv::Mat in = cv::Mat(
                image.height(),
                image.width(),
                CV_8UC1,
                image.bits(),
                image.bytesPerLine());

    // OpenCV에 있는 blur() 함수를 실행합니다. imgproc 모듈 안에 있는 기능입니다.
    // 첫 번째 인수는 원본 이미지, 두 번째 인수는 blur 된 이미지, 세 번째 인수는 커널의 크기입니다. 여기에서 커널은
    // OpenCV에게 주어진 픽셀의 값을 서로 다른 양의 인접 픽셀과 결합하여 값을 변경하는 방법을 알려줍니다.

    Mat out;
    /*void blur(InputArray src, OutputArray dst, Size ksize, Point anchor = Point(-1, -1), int borderType = BORDER_DEFAULT);

    src : 입력 영상
    src : 출력 영상으로 src와 같은 크기, 같은 채널 수를 갖는다.
    ksize : 블러링 커널 크기
    anchor : 고정점 좌표로 (-1, -1)을 지정하면 커널 중심점을 고정점으로 사용한다.
    borderType : 가장자리 픽셀 확장 방식*/

    // 커널의 크기를 5X5로 설정하여 블러 설정
    //    blur(in, out, cv::Size(5, 5));
    //    medianBlur(in, out, 5);
    GaussianBlur(in, out, Size(5, 5), 0);

    // 다시 볼 수 있는 형태로 이미지를 복구시킵니다.
    QImage image_Blur(
                out.data,
                out.cols,
                out.rows,
                out.step,
                QImage::Format_Grayscale8);

    QSize size = imageView->viewport()->size();
    QPixmap buf = QPixmap::fromImage(image_Blur);
    imageView->setAlignment(Qt::AlignCenter);

    foreach(auto item, imageScene->items()) {
        QGraphicsPixmapItem *i = dynamic_cast<QGraphicsPixmapItem*>(item);
        if(i != nullptr && i->toolTip() == "GraphicsScene") {
            i->setScale(1.0);
            i->setPixmap(buf.scaled(size, Qt::KeepAspectRatio, Qt::SmoothTransformation));
            imageScene->setSceneRect(i->sceneBoundingRect());
        }
    }
    selectImage = image_Blur.convertToFormat(selectImage.format()).copy();
    Histogram();
}

// 이미지에 선을 강조하는 샤프닝을 설정해주는 함수
void ImageAlbum::Sharpening()
{
    if(selectImage.isNull()){
        QMessageBox:: critical(messagebox, "경고", "이미지를 선택하세요");
        return;
    }
    QImage image = selectImage.convertToFormat(QImage::Format_Grayscale8);
    cv::Mat in = cv::Mat(
                image.height(),
                image.width(),
                CV_8UC1,
                image.bits(),
                image.bytesPerLine());

    Mat blurred;

    /*void GaussianBlur(InputArray src, OutputArray dst, Size kszie,
                  double sigmaX, double sigmaY = 0, int borderType = BORDER_DEFAULT);
    src : 입력 영상
    dst : 출력 영상
    ksize : 가우시안 커널 크기 (Size()를 지정하면 표준 편차로부터 커널 크기를 자동으로 결정)
    sigmaX : x 방향으로의 가우시안 커널 표준 편차
    sigmaY : y 방향으로의 가우시안 커널 표준 편차
    borderType : 가장자리 픽셀 확장 방식*/

    GaussianBlur(in, blurred, Size(), 8);
    float alpha = 0.5f;
    Mat out = (1 + alpha) * in - alpha * blurred;
    //    Mat out;
    //    addWeighted(in, 1.5, blurred, -0.5, 0, out);

    QImage image_Sharpen = QImage(
                out.data,
                out.cols,
                out.rows,
                out.step,
                QImage::Format_Grayscale8).copy();

    QSize size = imageView->viewport()->size();
    QPixmap buf = QPixmap::fromImage(image_Sharpen);
    imageView->setAlignment(Qt::AlignCenter);

    foreach(auto item, imageScene->items()) {
        QGraphicsPixmapItem *i = dynamic_cast<QGraphicsPixmapItem*>(item);
        if(i != nullptr && i->toolTip() == "GraphicsScene") {
            i->setScale(1.0);
            i->setPixmap(buf.scaled(size, Qt::KeepAspectRatio, Qt::SmoothTransformation));
            imageScene->setSceneRect(i->sceneBoundingRect());
        }
    }

    selectImage = image_Sharpen.convertToFormat(selectImage.format()).copy();
    Histogram();
}

// 슬라이더를 이용해 이미지의 감마 효과를 설정
void ImageAlbum::Gamma(int value)
{
    if(selectImage.isNull()){
        QMessageBox:: critical(messagebox, "경고", "이미지를 선택하세요");
        return;
    }

    QImage image = selectImage.convertToFormat(QImage::Format_Grayscale8);
    cv::Mat in = cv::Mat(
                image.height(),
                image.width(),
                CV_8UC1,
                image.bits(),
                image.bytesPerLine());

    Mat pixel(1, 256, CV_8U);
    uchar* p = pixel.ptr();
    double gamma = (double)value / 10;
    for (int i = 0; i < 256; ++i)
        p[i] = saturate_cast<uchar>(pow(i / 255.0, 1/gamma) * 255.0);
    Mat out;
    cv::LUT(in, pixel, out);

    image_Gamma = QImage(
                out.data,
                out.cols,
                out.rows,
                out.step,
                QImage::Format_Grayscale8).copy();

    QSize size = imageView->viewport()->size();
    QPixmap buf = QPixmap::fromImage(image_Gamma);
    imageView->setAlignment(Qt::AlignCenter);

    foreach(auto item, imageScene->items()) {
        QGraphicsPixmapItem *i = dynamic_cast<QGraphicsPixmapItem*>(item);
        if(i != nullptr && i->toolTip() == "GraphicsScene") {
            i->setScale(1.0);
            i->setPixmap(buf.scaled(size, Qt::KeepAspectRatio, Qt::SmoothTransformation));
            imageScene->setSceneRect(i->sceneBoundingRect());
        }
    }
}

// 슬라이더를 놓으면 누적되는 이미지에 감마가 적용된 이미지 저장
void ImageAlbum::on_Gamma_sliderReleased()
{
    selectImage = image_Gamma.convertToFormat(selectImage.format()).copy();
    Histogram();
}

// 처방전 작성 버튼 클릭 시 의사 정보, 환자 정보를 처방전 클래스로 전송
void ImageAlbum::on_Prescription_clicked()
{   
    emit sendPrescription(DoctorID, DoctorName, PatientID, PatientName, PatientSex);
    m_prescription->show();
}

// 환자 정보 클래스에서 받아온 환자 정보를 처방전 클래스로 보내기 위해 멤버 변수에 저장
void ImageAlbum::receivePatientInfo(QString ID, QString Name, QString Sex)
{
    PatientID = ID;
    PatientName = Name;
    PatientSex = Sex;
}

// 환자 정보 클래스에서 받아온 의사 정보를 처방전 클래스로 보내기 위해 멤버 변수에 저장
void ImageAlbum::receiveDoctorInfo(QString ID, QString Name)
{
    DoctorID = ID;
    DoctorName = Name;
}

// 처방전 작성 후 작성완료 버튼 클릭 시 서버로 해당 데이터를 보내주기 위한 함수
void ImageAlbum::receivePrescriptionFinish(QString Data)
{
    emit sendPrescriptiontoServer(Data);
    prescriptionCheck = true;
    m_prescription->close();
}

// 환자의 이미지 파일이 모두 수신되면 true, 아직 다운 받은 중이면 false 반환
void ImageAlbum::receiveAllImageFileA(bool AllImageFile)
{
    AllSendImageCheck = AllImageFile;
}

// 진료 종료 버튼 클릭 시 해당 환자 정보를 서버에 전송 및 해당 환자의 이미지 파일 삭제
void ImageAlbum::receivePatientTreatmentEnd()
{
    // 진료 종료 버튼 클릭 시 해당 버튼과 처방전 작성 비활성화
    ui->EndTreatment->setDisabled(true);
    ui->Prescription->setDisabled(true);

    // 진료 종료 버튼 클릭 시 해당 환자의 이미지 파일 삭제
    QDir dir("./Image/");
    dir.removeRecursively();

    // 진료 종료 버튼 클릭 시 환자 관리 클래스로 해당 신호 전송
    emit sendEndSignal();

    // 진료 종료 버튼 클릭 시 기존 환자의 사진 담고 있는 listWidget, graphicsView, graphicsScene 클리어
    emit SendType(ImageScene::DrawType::Cursor);
    imageView->resetTransform();
    ui->Brightness->setSliderPosition(0);
    ui->Contrast->setSliderPosition(0);
    ui->Gamma->setSliderPosition(10);
    ui->Thickness->setValue(6);
    ui->listWidget->clear();
    imageScene->clear();
    prescriptionCheck = false;

    // 다음 환자의 이미지 파일 수신이 완료 되면 다시 true로 변경
    AllSendImageCheck = false;
}

// 진료 종료 버튼 클릭 시 서버로 환자 정보를 전송
void ImageAlbum::on_EndTreatment_clicked()
{
    if(!prescriptionCheck){
        QMessageBox:: critical(messagebox, "경고", "처방전을 입력해주세요");
        return;
    }

    if(!AllSendImageCheck){
        QMessageBox:: critical(messagebox, "경고", "이미지 업로드 중입니다. "
                                                 "잠시만 기다려주세요");
        return;
    }

    // 진료 종료 시 해당하는 환자 정보(ID, 이름)를 서버로 전송
    QString Data = "SEN^VTF<CR>" + PatientID + "<CR>" + PatientName;
    emit sendEndTreatment(Data);
}

// 펜 두께의 spinBox 값이 변경되면 ImageScene으로 해당 값을 전송
void ImageAlbum::Thickness(int value)
{
    penThickness = value;
    emit SendThickness(penThickness);
    // 스핀박스를 활용하여 폰트 사이즈 변경하는 슬롯
    emit SendFontSize(value);
}

// 촬영 의뢰 버튼 클릭 시 listWidget에 기존 환자 사진 클리어
void ImageAlbum::receiveCameraStart()
{
    ui->listWidget->clear();
    ui->Brightness->setSliderPosition(0);
    ui->Contrast->setSliderPosition(0);
    ui->Gamma->setSliderPosition(10);
    imageView->resetTransform();
    imageScene->clear();

    // 촬영 의뢰 버튼 클릭 시 진료 종료 버튼, 처방전 작성 버튼 비활성화
    ui->EndTreatment->setDisabled(true);
    ui->Prescription->setDisabled(true);
}

// 처방전 작성 완료 버튼 클릭 시 서버에서 받아온 DB의 저장 성공 여부를 처방전 클래스로 전송
void ImageAlbum::receivePrescriptionCheck(QString signal)
{
    emit sendPrescriptionYesOrNot(signal);
}

// 메인 윈도우에서 프린터 버튼 클릭 시 프린트 다이얼로그 출력 및 프린트 기능 실행
void ImageAlbum::receivePrintStart()
{
    QPrinter printer(QPrinter::HighResolution);
    printer.setFullPage(true);
    printer.setPageSize(QPageSize::A4);

    QPrintDialog* printDialog = new QPrintDialog(&printer, this);
    if (printDialog->exec() == QDialog::Accepted) {
        QPainter painter(&printer);
        // 이미지에 해당하는 부분을 캡쳐
        QPixmap buffer = imageView->grab();
        QRect rect = printer.pageRect(QPrinter::DevicePixel).toRect();
        buffer.setDevicePixelRatio(1);

        // 프린트 시 중앙에 맞춰서 출력하기 위해 좌표 설정 및 resize
        buffer = buffer.scaled(rect.size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
        painter.drawPixmap(rect.center().x()-buffer.width()/2, rect.center().y()-buffer.height()/2, buffer);

        // 발행 일자를 텍스트 박스로 이미지와 함께 출력
        const QRect rectangle = QRect(10, 10, 1000, 10000);
        painter.setPen(QPen(QColor(80, 239, 0)));
        painter.drawText(rectangle, 0, "발행 일자 : " + QDate::currentDate().toString("yyyy-MM-dd"));
        painter.end();
    }
}

// 콤보박스가 선택되면 해당 선택된 임플란트 종류를 전송
void ImageAlbum::on_ImplantcomboBox_activated(int index)
{
    emit sendImplantType(index);
}

// 탭이 바뀌면 각도, 길이 측정 도중에 남아있던 도형 데이터들을 삭제 및 커서모드로 변경
void ImageAlbum::on_tabWidget_currentChanged(int index)
{
    Q_UNUSED(index);
    emit SendType(999);
}
