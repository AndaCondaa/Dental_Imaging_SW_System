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

ImageAlbum::ImageAlbum(QWidget *parent)
    : QWidget(parent), ui(new Ui::ImageAlbum)
{
    ui->setupUi(this);

    ui->ZoomIn->setIcon(QIcon("./Icon/zoomin.png"));
    ui->ZoomOut->setIcon(QIcon("./Icon/zoomout.png"));
    ui->Delete->setIcon(QIcon("./Icon/delete.png"));
    ui->Ellipse->setIcon(QIcon("./Icon/ellipse.png"));
    ui->Rectangle->setIcon(QIcon("./Icon/rectangle.png"));
    ui->Lines->setIcon(QIcon("./Icon/brush.png"));
    ui->Implant->setIcon(QIcon("./Icon/Implant.png"));
    ui->Brush->setIcon(QIcon("./Icon/color.png"));
    ui->Capture->setIcon(QIcon("./Icon/capture.png"));
    ui->LeftRotate->setIcon(QIcon("./Icon/leftrotate.png"));
    ui->RightRotate->setIcon(QIcon("./Icon/rightrotate.png"));
    ui->VReverse->setIcon(QIcon("./Icon/vertical.png"));
    ui->HReverse->setIcon(QIcon("./Icon/horizontal.png"));
    ui->Cursor->setIcon(QIcon("./Icon/cursor.png"));
    ui->Laser->setIcon(QIcon("./Icon/laser.png"));
    ui->Freehand->setIcon(QIcon("./Icon/freehand.png"));
    ui->OrigImage->setIcon(QIcon("./Icon/reset.png"));
    ui->OrigImage2->setIcon(QIcon("./Icon/reset.png"));

    ui->ImageInfo->setReadOnly(true);
    ui->PhotoDate->setReadOnly(true);

    //화면이 처음 켜지면 진료 종료, 처방전 작성 버튼 비활성화
    ui->Prescription->setDisabled(true);
    ui->EndTreatment->setDisabled(true);

    imageView = new QGraphicsView(this);
    imageScene = new ImageScene(this);
    m_prescription = new Prescription(0);

    chart = new QChart;
    view = new QChartView;
    view->setChart(chart);

    QVBoxLayout *lay = new QVBoxLayout(ui->widget);
    lay->addWidget(view);
    chart->setContentsMargins(0, 0, 0, 0);
    view->setContentsMargins(0, 0, 0, 0);

    //초깃값 설정
    prescriptionCheck = false;
    //서버로부터 이미지 파일 수신 완료되면 true로 변경
    AllSendImageCheck = false;
    paintColor = Qt::white;
    ui->LengthResult->setReadOnly(true);
    ui->AngleResult->setReadOnly(true);
    ui->lineEdit->setPlaceholderText("텍스트를 입력해주세요");

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

    imageView->setGeometry(6, 6, 600, 600);
    imageView->setSizeAdjustPolicy(QAbstractScrollArea::AdjustToContents);
    imageView->setDragMode(QGraphicsView::NoDrag);
    ui->gridLayout->addWidget(imageView);
    imageView->setAlignment(Qt::AlignCenter);
    imageView->setScene(imageScene);

    connect(ui->listWidget, SIGNAL(itemDoubleClicked(QListWidgetItem*)), this, SLOT(selectItem(QListWidgetItem*)));

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

    /*GraphicsView에 펜 색상, 펜 두께, 선인지 도형인지를 구분하여 시그널 전송*/
    connect(this, SIGNAL(SendThickness(int)), imageScene, SLOT(ReceiveThickness(int)));
    connect(this, SIGNAL(SendBrushColor(QColor)), imageScene, SLOT(ReceiveBrushColor(QColor)));
    connect(this, SIGNAL(SendType(int)), imageScene, SLOT(ReceiveType(int)));
    connect(this, SIGNAL(SendText(QString)), imageScene, SLOT(ReceiveText(QString)));
    connect(this, SIGNAL(SendLength(int, int, int, int, QString)), imageScene, SLOT(ReceiveLength(int, int, int, int, QString)));

    //텍스트 사이즈를 변경하기 위해 시그널 전송
    connect(this, SIGNAL(SendFontSize(int)), imageScene, SLOT(ReceiveFontSize(int)));

    //GraphicsSCene에서 측정한 길이를 위젯 화면에 보여주기 위한 시그널-슬롯
    connect(imageScene, SIGNAL(SendMeasurement(QString, double)), this, SLOT(ReceiveMeasurement(QString, double)));

    //GraphicsScene에서 측정한 캡쳐되는 영역의 좌표를 불러오는 시그널-슬롯
    connect(imageScene, SIGNAL(SendCapturePos(QPointF, QPointF)), this, SLOT(ReceiveCapturePos(QPointF, QPointF)));

    //처방전 작성 버튼 클릭 시 처방전 클래스로 의사 정보, 환자 정보를 전송
    connect(this, SIGNAL(sendPrescription(QString, QString, QString, QString, QString)),
            m_prescription, SLOT(receivePrescription(QString, QString, QString, QString, QString)));

    //처방전 클래스에서 처방전 작성 완료 되면 해당 내용을 서버로 보내주기 위한 과정
    connect(m_prescription, SIGNAL(sendPrescriptionFinish(QString)), this, SLOT(receivePrescriptionFinish(QString)));

    //콤보박스를 통해 임플란트 타입 선택 시 해당 타입에 따라 다른 임플란트가 식립되도록 하기 위한 시그널-슬롯
    connect(this, SIGNAL(sendImplantType(int)), imageScene, SLOT(ReceiveImplantType(int)));

//    reloadImages();
}

ImageAlbum::~ImageAlbum()
{

}

void ImageAlbum::reloadImages()
{
//    QDir dir("./Photo");
    QDir dir("./Image");
    QStringList filters;
    filters << "*.png" << "*.jpg" << "*.bmp" << "*.gif";
    QFileInfoList fileInfoList = dir.entryInfoList(filters, QDir::Files | QDir::NoDotAndDotDot);

    ui->listWidget->clear();
    for(int i = 0; i < fileInfoList.count(); i++) {
        QListWidgetItem* item = new QListWidgetItem(QIcon(dir.path() + "/" + fileInfoList.at(i).fileName()), NULL, ui->listWidget); //, QListWidgetItem::UserType);
        item->setStatusTip(dir.path() + "/" + fileInfoList.at(i).fileName());
        ui->listWidget->addItem(item);
    };

    //진료 시작 버튼 클릭 시 처방전 작성 버튼, 진료 종료 버튼 활성화
    ui->Prescription->setDisabled(false);
    ui->EndTreatment->setDisabled(false);
}

void ImageAlbum::ReceiveMeasurement(QString type, double length)
{
    if(type == "Angle"){
        QString Result = QString::number(length);
        ui->AngleResult->setText(Result + "°");
    }
    else {
        QString Result = QString::number(length);
        ui->LengthResult->setText(type + " " + Result + " mm");
    }
}

void ImageAlbum::Implant()
{
    if(selectImage.isNull()){
        QMessageBox:: critical(this, "경고", "이미지를 선택하세요");
        return;
    }
    emit SendType(ImageScene::DrawType::Implant);
//    emit SendType(14);
}

void ImageAlbum::Capture()
{
    if(selectImage.isNull()){
        QMessageBox:: critical(this, "경고", "이미지를 선택하세요");
        return;
    }
    emit SendType(ImageScene::DrawType::Capture);
//    emit SendType(12);
}

void ImageAlbum::Angle()
{
    //이미지가 선택되지 않았다면 예외처리
    if(selectImage.isNull()){
        QMessageBox:: critical(this, "경고", "이미지를 선택하세요");
        return;
    }
    emit SendType(ImageScene::DrawType::Angle);
//    emit SendType(11);
}

void ImageAlbum::Length()
{
    //이미지가 선택되지 않았다면 예외처리
    if(selectImage.isNull()){
        QMessageBox:: critical(this, "경고", "이미지를 선택하세요");
        return;
    }
    ui->LengthResult->clear();
    emit SendType(ImageScene::DrawType::Length);
//    emit SendType(9);

    int origWidth = selectImage.width();
    int origHeight = selectImage.height();
    int sceneWidth = imageScene->width();
    int sceneHeight = imageScene->height();
    emit SendLength(origWidth, origHeight, sceneWidth, sceneHeight, imageType);
}

void ImageAlbum::TextBox()
{
    //이미지가 선택되지 않았다면 예외처리
    if(selectImage.isNull()){
        QMessageBox:: critical(this, "경고", "이미지를 선택하세요");
        return;
    }

    ui->lineEdit->text();
    emit SendText(ui->lineEdit->text());
    emit SendType(ImageScene::DrawType::Text);
//    emit SendType(6);
    ui->lineEdit->clear();
}

void ImageAlbum::RectangleItem()
{
    //이미지가 선택되지 않았다면 예외처리
    if(selectImage.isNull()){
        QMessageBox:: critical(this, "경고", "이미지를 선택하세요");
        return;
    }

    emit SendType(ImageScene::DrawType::Rectangle);
//    emit SendType(5);
}

void ImageAlbum::Cursor()
{
    //이미지가 선택되지 않았다면 예외처리
    if(selectImage.isNull()){
        QMessageBox:: critical(this, "경고", "이미지를 선택하세요");
        return;
    }

    emit SendType(ImageScene::DrawType::Cursor);
//    emit SendType(3);
}

void ImageAlbum::DeleteItem()
{
    //이미지가 선택되지 않았다면 예외처리
    if(selectImage.isNull()){
        QMessageBox:: critical(this, "경고", "이미지를 선택하세요");
        return;
    }

    emit SendType(ImageScene::DrawType::Delete);
//    emit SendType(7);
}

void ImageAlbum::Ellipse()
{
    //이미지가 선택되지 않았다면 예외처리
    if(selectImage.isNull()){
        QMessageBox:: critical(this, "경고", "이미지를 선택하세요");
        return;
    }

    emit SendType(ImageScene::DrawType::Ellipse);
//    emit SendType(4);
}

void ImageAlbum::Laser()
{
    //이미지가 선택되지 않았다면 예외처리
    if(selectImage.isNull()){
        QMessageBox:: critical(this, "경고", "이미지를 선택하세요");
        return;
    }

    emit SendType(ImageScene::DrawType::Laser);
//    emit SendType(2);
}

void ImageAlbum::Lines()
{
    //이미지가 선택되지 않았다면 예외처리
    if(selectImage.isNull()){
        QMessageBox:: critical(this, "경고", "이미지를 선택하세요");
        return;
    }

    emit SendType(ImageScene::DrawType::Lines);
//    emit SendType(0);
}

void ImageAlbum::Freehand()
{
    //이미지가 선택되지 않았다면 예외처리
    if(selectImage.isNull()){
        QMessageBox:: critical(this, "경고", "이미지를 선택하세요");
        return;
    }

    emit SendType(ImageScene::DrawType::FreeHand);
//    emit SendType(1);
}

void ImageAlbum::Thickness(int value)
{
    penThickness = value;
    emit SendThickness(penThickness);
}

void ImageAlbum::BrushColor()
{
    paintColor = QColorDialog::getColor(paintColor, this);
    emit SendBrushColor(paintColor);
}

void ImageAlbum::ZoomIn()
{
    imageView->scale(1.25, 1.25);
}

void ImageAlbum::ZoomOut()
{
    imageView->scale(0.8, 0.8);
}

void ImageAlbum::LeftRotate()
{
    imageView->rotate(-90);
}

void ImageAlbum::RightRotate()
{
    imageView->rotate(90);
}

void ImageAlbum::OrigImage()
{
    if(selectImage.isNull()){
        QMessageBox:: critical(this, "경고", "이미지를 선택하세요");
        return;
    }

    emit SendType(ImageScene::DrawType::Clear);
//    emit SendType(8);
    imageView->resetTransform();
    imageScene->setBackgroundBrush(Qt::white);
    ui->Brightness->setSliderPosition(0);
    ui->Contrast->setSliderPosition(10);
    ui->Gamma->setSliderPosition(10);
    ui->LengthResult->clear();
    ui->AngleResult->clear();

    selectImage = QPixmap(orignal->statusTip()).toImage();
    Histogram();

    imageScene->clear();
    QSize size = imageView->viewport()->size();
    QGraphicsItem *i = imageScene->addPixmap(QPixmap(orignal->statusTip()).scaled(size, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    i->setToolTip("GraphicsScene");
    imageView->setAlignment(Qt::AlignCenter);
    imageScene->setSceneRect(i->sceneBoundingRect());
}

void ImageAlbum::selectItem(QListWidgetItem* item)
{
    emit SendType(ImageScene::DrawType::Clear);
//    emit SendType(8);
    imageView->resetTransform();
    ui->LengthResult->clear();
    imageView->setBackgroundBrush(Qt::white);
    imageScene->setBackgroundBrush(Qt::white);
    ui->Brightness->setSliderPosition(0);
    ui->Contrast->setSliderPosition(10);
    ui->Gamma->setSliderPosition(10);
    ui->AngleResult->clear();

    orignal = item;
    origImage = new QImage(ui->listWidget->currentItem()->statusTip());
    selectImage = QImage(ui->listWidget->currentItem()->statusTip());

    Histogram();

    //이미지를 선택하면 해당 이미지에 대한 날짜 및 종류가 보여짐
    QString Date = ui->listWidget->currentItem()->statusTip().split("/")[2].first(8);
    Date = Date.first(4) + "년 " + Date.left(6).right(2) + "월 " + Date.last(2) + "일";
    ui->ImageInfo->setText(" " + ui->listWidget->currentItem()->statusTip().split("/")[2].last(8).first(4));
    ui->PhotoDate->setText(" " + Date);
    imageType = ui->listWidget->currentItem()->statusTip().split("/")[2].last(8).first(4);
    emit SendLength(selectImage.width(), selectImage.height(), imageScene->width(), imageScene->height(), imageType);

    imageScene->clear();
    QSize size = imageView->viewport()->size();
    QGraphicsItem *i = imageScene->addPixmap(QPixmap(item->statusTip()).scaled(size, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    i->setToolTip("GraphicsScene");
    imageView->setAlignment(Qt::AlignCenter);
    imageScene->setSceneRect(i->sceneBoundingRect());

    qDebug() << selectImage.width();
    qDebug() << imageScene->width();
}

void ImageAlbum::VReverse()
{
    if(selectImage.isNull()){
        QMessageBox:: critical(this, "경고", "이미지를 선택하세요");
        return;
    }

    selectImage.mirror(true, false);
    Histogram();

    QSize size = imageView->viewport()->size();
    QPixmap buf = QPixmap::fromImage(selectImage);
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

void ImageAlbum::HReverse()
{
    if(selectImage.isNull()){
        QMessageBox:: critical(this, "경고", "이미지를 선택하세요");
        return;
    }

    selectImage.mirror(false, true);
    Histogram();

    QSize size = imageView->viewport()->size();
    QPixmap buf = QPixmap::fromImage(selectImage);
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

void ImageAlbum::ReceiveCapturePos(QPointF startPos, QPointF endPos)
{
    if(selectImage.isNull()){
        QMessageBox:: critical(this, "경고", "이미지를 선택하세요");
        return;
    }

    QPoint topLeft = imageView->mapFromScene(startPos);
    QPoint bottomRight = imageView->mapFromScene(endPos);
    QRect rect = QRect(topLeft, bottomRight).normalized();

    //원본 이미지에서 주어진 사각형의 사이즈만큼 잘라서 확인
    QPixmap buf = imageView->grab(rect);

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
//    emit SendType(8);
}

void ImageAlbum::Brightness(int value)
{
    if(selectImage.isNull()){
        QMessageBox:: critical(this, "경고", "이미지를 선택하세요");
        return;
    }

    Mat out;
    QImage image = selectImage.convertToFormat(QImage::Format_Grayscale8);    

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
    in.convertTo(out, -1, 1, value - Brightvalue);

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

void ImageAlbum::on_Brightness_sliderReleased()
{
    Brightvalue = ui->Brightness->value();
    selectImage = image_brightness.convertToFormat(selectImage.format()).copy();
    Histogram();
}

void ImageAlbum::HistEqual()
{
    if(selectImage.isNull()){
        QMessageBox:: critical(this, "경고", "이미지를 선택하세요");
        return;
    }

    QImage image = selectImage.convertToFormat(QImage::Format_Grayscale8);

    cv::Mat in = cv::Mat(
                image.height(),
                image.width(),
                CV_8UC1,
                image.bits(),
                image.bytesPerLine());
    Mat out, Hist;
    equalizeHist(in, Hist);

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

void ImageAlbum::Histogram()
{
    view->resetTransform();
    chart->resetTransform();
    chart->removeAllSeries();

    QImage image = selectImage.convertToFormat(QImage::Format_Grayscale8);

    int hbins = 255; //histogram x axis size, that is histSize,
    //ex) 2 -> 0~128, 129~256, ex)16 -> 0 ~ 15, 16 ~ 31...,
    int channels[] = { 0 }; //index of channel
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

    calcHist(&in, 1, channels, Mat(), //MaskForHisto, // // do use mask
             Hist, 1, histSize, ranges,
             true, // the histogram is uniform
             false);
    normalize(Hist, Hist, 0, 255, NORM_MINMAX);

    QLineSeries *series = new QLineSeries;
    series->setName("Image");
    series->setColor(QColor(150, 150, 150));

    for(int j = 0; j < Hist.rows; ++j){
        *series << QPoint(j, Hist.at<float>(j));
    }

//    view->setContentsMargins(-10, -10, -10, -10);
    chart->setAnimationOptions(QChart::AllAnimations);
    chart->setFont(QFont("", 3, 3, false));
    chart->addSeries(series);
    chart->createDefaultAxes();
    chart->setDropShadowEnabled(true);
    chart->setBackgroundVisible(false);
//    chart->setTheme(QChart::ChartThemeBlueCerulean);
    chart->legend()->hide();
}

void ImageAlbum::Reverse()
{
    if(selectImage.isNull()){
        QMessageBox:: critical(this, "경고", "이미지를 선택하세요");
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

void ImageAlbum::Contrast(int value)
{
    if(selectImage.isNull()){
        QMessageBox:: critical(this, "경고", "이미지를 선택하세요");
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
    double beta = (double)value / 10;
    in.convertTo(out, -1, beta, 0);

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

void ImageAlbum::on_Contrast_sliderReleased()
{
    selectImage = image_Contrast.convertToFormat(selectImage.format()).copy();
    Histogram();
}

void ImageAlbum::Blur()
{
    if(selectImage.isNull()){
        QMessageBox:: critical(this, "경고", "이미지를 선택하세요");
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
    blur(in, out, cv::Size(5, 5));

    //다시 볼 수 있는 형태로 이미지를 복구시킵니다.
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

void ImageAlbum::Sharpening()
{
    if(selectImage.isNull()){
        QMessageBox:: critical(this, "경고", "이미지를 선택하세요");
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
    float alpha = 1.f;
    Mat out = (1 + alpha) * in - alpha * blurred;

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

void ImageAlbum::Gamma(int value)
{
    if(selectImage.isNull()){
        QMessageBox:: critical(this, "경고", "이미지를 선택하세요");
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
        p[i] = saturate_cast<uchar>(pow(i / 255.0, gamma) * 255.0);
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

void ImageAlbum::on_Gamma_sliderReleased()
{
    selectImage = image_Gamma.convertToFormat(selectImage.format()).copy();
    Histogram();
}

void ImageAlbum::on_Prescription_clicked()
{
    //처방전 작성 버튼 클릭 시 의사 정보, 환자 정보를 처방전 클래스로 전송
    emit sendPrescription(DoctorID, DoctorName, PatientID, PatientName, PatientSex);
    m_prescription->show();
}

//환자 정보 클래스에서 받아온 환자 정보를 처방전 클래스로 보내기 위해 멤버 변수에 저장
void ImageAlbum::receivePatientInfo(QString ID, QString Name, QString Sex)
{
    PatientID = ID;
    PatientName = Name;
    PatientSex = Sex;
}

//환자 정보 클래스에서 받아온 의사 정보를 처방전 클래스로 보내기 위해 멤버 변수에 저장
void ImageAlbum::receiveDoctorInfo(QString ID, QString Name)
{
    DoctorID = ID;
    DoctorName = Name;
}

void ImageAlbum::receivePrescriptionFinish(QString Data)
{
    emit sendPrescriptiontoServer(Data);
    prescriptionCheck = true;
    m_prescription->close();
}

void ImageAlbum::receiveAllImageFileA(bool AllImageFile)
{
    AllSendImageCheck = AllImageFile;
}

//진료 종료 버튼 클릭 시 해당 환자 정보를 서버에 전송 및 해당 환자의 이미지 파일 삭제
void ImageAlbum::on_EndTreatment_clicked()
{
    if(!prescriptionCheck){
        QMessageBox:: critical(this, "경고", "처방전을 입력해주세요");
        return;
    }

    if(!AllSendImageCheck){
        QMessageBox:: critical(this, "경고", "이미지 업로드 중입니다. "
                                           "잠시만 기다려주세요");
        return;
    }

    //진료 종료 버튼 클릭 시 해당 버튼과 처방전 작성 비활성화
    ui->EndTreatment->setDisabled(true);
    ui->Prescription->setDisabled(true);

    //진료 종료 시 해당하는 환자 정보(ID, 이름)를 서버로 전송
    QString Data = "VTF<CR>" + PatientID + "<CR>" + PatientName;
    emit sendEndTreatment(Data);

    //진료 종료 버튼 클릭 시 해당 환자의 이미지 파일 삭제
    QDir dir("./Image/");
    dir.removeRecursively();

    //진료 종료 버튼 클릭 시 환자 관리 클래스로 해당 신호 전송
    emit sendEndSignal();

    //진료 종료 버튼 클릭 시 기존 환자의 사진 담고 있는 listWidget + graphicsView + graphicsScene 클리어
    ui->listWidget->clear();
    prescriptionCheck = false;

    //다음 환자의 이미지 파일 수신이 완료 되면 다시 true로 변경
    AllSendImageCheck = false;

    ui->Brightness->setSliderPosition(0);
    ui->Contrast->setSliderPosition(10);
    ui->Gamma->setSliderPosition(10);
    ui->LengthResult->clear();
    ui->AngleResult->clear();
    imageView->resetTransform();
    imageScene->clear();
}

//스핀박스를 활용하여 폰트 사이즈 변경하는 슬롯
void ImageAlbum::on_Fontsize_valueChanged(int size)
{
    emit SendFontSize(size);
}

//촬영 의뢰 버튼 클릭 시 listWidget에 기존 환자 사진 클리어
void ImageAlbum::receiveCameraStart()
{
    ui->listWidget->clear();

    ui->Brightness->setSliderPosition(0);
    ui->Contrast->setSliderPosition(10);
    ui->Gamma->setSliderPosition(10);
    ui->LengthResult->clear();
    ui->AngleResult->clear();
    imageView->resetTransform();
    imageScene->clear();

    //촬영 의뢰 버튼 클릭 시 진료 종료 버튼, 처방전 작성 버튼 비활성화
    ui->EndTreatment->setDisabled(true);
    ui->Prescription->setDisabled(true);
}

//메인 윈도우에서 프린터 버튼 클릭 시 프린트 다이얼로그 출력 및 프린트 기능 실행
void ImageAlbum::receivePrintStart()
{
    QPrinter printer(QPrinter::HighResolution);
       printer.setFullPage(true);
       printer.setPageSize(QPageSize::A4);

       QPrintDialog* printDialog = new QPrintDialog(&printer, this);
       if (printDialog->exec() == QDialog::Accepted) {
           QPainter painter(&printer);
           //이미지에 해당하는 부분을 캡쳐
           QPixmap buffer = imageView->grab();
           QRect rect = printer.pageRect(QPrinter::DevicePixel).toRect();
           buffer.setDevicePixelRatio(1);

           //프린트 시 중앙에 맞춰서 출력하기 위해 좌표 설정 및 resize
           buffer = buffer.scaled(rect.size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
           painter.drawPixmap(rect.center().x()-buffer.width()/2, rect.center().y()-buffer.height()/2, buffer);
           const QRect rectangle = QRect(10, 10, 1000, 10000);
           painter.setPen(QPen(QColor(80, 239, 0)));
           painter.drawText(rectangle, 0, "발행 일자 : " + QDate::currentDate().toString("yyyy-MM-dd"));
           painter.end();
       }
}

//콤보박스가 선택되면 해당 선택된 임플란트 종류를 전송
void ImageAlbum::on_ImplantcomboBox_activated(int index)
{
    emit sendImplantType(index);
    qDebug() << index;
}

//탭이 바뀌면 각도, 길이 측정 도중에 남아있던 도형 데이터들을 삭제 및 커서모드로 변경
void ImageAlbum::on_tabWidget_currentChanged(int index)
{
    emit SendType(999);
}


