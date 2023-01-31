#include "ui_prescription.h"
#include <QSplitter>
#include <QListWidget>
#include <QDir>
#include <QFileInfo>
#include <QGraphicsScene>
#include <QGroupBox>
#include <QColorDialog>
#include <QMouseEvent>
#include <QMessageBox>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc.hpp>

using namespace cv;

#include "imagealbum.h"
#include "ui_imagealbum.h"
#include "imagescene.h"
#include "prescription.h"

#define LIMIT_UBYTE(n) (n > UCHAR_MAX) ? UCHAR_MAX:(n<0) ? 0 : n

ImageAlbum::ImageAlbum(QWidget *parent)
    : QWidget(parent), ui(new Ui::ImageAlbum)
{
    ui->setupUi(this);

    imageView = new QGraphicsView(this);
    imageScene = new ImageScene(this);
    m_prescription = new Prescription(0);

    //초깃값 설정
    prescriptionCheck = false;
    ui->LengthResult->setReadOnly(true);
    ui->AngleResult->setReadOnly(true);

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
    connect(ui->horizontalSlider, SIGNAL(valueChanged(int)), this, SLOT(Brightness(int)));
    connect(ui->VReverse, SIGNAL(clicked()), this, SLOT(VReverse()));
    connect(ui->HReverse, SIGNAL(clicked()), this, SLOT(HReverse()));
    connect(ui->Blur, SIGNAL(clicked()), this, SLOT(Blur()));
    connect(ui->Reverse, SIGNAL(clicked()), this, SLOT(Reverse()));
    connect(ui->Contrast, SIGNAL(valueChanged(double)), this, SLOT(Contrast(double)));
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

    /*GraphicsView에 펜 색상, 펜 두께, 선인지 도형인지를 구분하여 시그널 전송*/
    connect(this, SIGNAL(SendThickness(int)), imageScene, SLOT(ReceiveThickness(int)));
    connect(this, SIGNAL(SendBrushColor(QColor)), imageScene, SLOT(ReceiveBrushColor(QColor)));
    connect(this, SIGNAL(SendType(int)), imageScene, SLOT(ReceiveType(int)));
    connect(this, SIGNAL(SendText(QString)), imageScene, SLOT(ReceiveText(QString)));
    connect(this, SIGNAL(SendLength(int, int, int, int)), imageScene, SLOT(ReceiveLength(int, int, int, int)));

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

    reloadImages("P00004");
}

ImageAlbum::~ImageAlbum()
{

}

void ImageAlbum::reloadImages(QString ID)
{
    QDir dir(".");
//    QDir dir(QString("./Image/%1").arg(ID));
    QStringList filters;
    filters << "*.png" << "*.jpg" << "*.bmp" << "*.gif";
    QFileInfoList fileInfoList = dir.entryInfoList(filters, QDir::Files | QDir::NoDotAndDotDot);

    ui->listWidget->clear();
    for(int i = 0; i < fileInfoList.count(); i++) {
        QListWidgetItem* item = new QListWidgetItem(QIcon(dir.path() + "/" + fileInfoList.at(i).fileName()), NULL, ui->listWidget); //, QListWidgetItem::UserType);
        item->setStatusTip(dir.path() + "/" + fileInfoList.at(i).fileName());
        ui->listWidget->addItem(item);
    };
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

void ImageAlbum::Capture()
{
    if(selectImage.isNull()){
        QMessageBox:: critical(this, "경고", "이미지를 선택하세요");
        return;
    }

    emit SendType(12);
}

void ImageAlbum::Angle()
{
    //이미지가 선택되지 않았다면 예외처리
    if(selectImage.isNull()){
        QMessageBox:: critical(this, "경고", "이미지를 선택하세요");
        return;
    }

    emit SendType(11);
}

void ImageAlbum::Length()
{
    //이미지가 선택되지 않았다면 예외처리
    if(selectImage.isNull()){
        QMessageBox:: critical(this, "경고", "이미지를 선택하세요");
        return;
    }
    ui->LengthResult->clear();

    if(ui->Ceph->isChecked())
        emit SendType(9);
    else if(ui->Pano->isChecked())
        emit SendType(10);

    int origWidth = selectImage.width();
    int origHeight = selectImage.height();
    int sceneWidth = imageScene->width();
    int sceneHeight = imageScene->height();
    emit SendLength(origWidth, origHeight, sceneWidth, sceneHeight);
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
    emit SendType(6);
    ui->lineEdit->clear();
}

void ImageAlbum::RectangleItem()
{
    //이미지가 선택되지 않았다면 예외처리
    if(selectImage.isNull()){
        QMessageBox:: critical(this, "경고", "이미지를 선택하세요");
        return;
    }

    emit SendType(5);
}

void ImageAlbum::Cursor()
{
    //이미지가 선택되지 않았다면 예외처리
    if(selectImage.isNull()){
        QMessageBox:: critical(this, "경고", "이미지를 선택하세요");
        return;
    }

    emit SendType(3);
}

void ImageAlbum::DeleteItem()
{
    //이미지가 선택되지 않았다면 예외처리
    if(selectImage.isNull()){
        QMessageBox:: critical(this, "경고", "이미지를 선택하세요");
        return;
    }

    emit SendType(7);
}

void ImageAlbum::Ellipse()
{
    //이미지가 선택되지 않았다면 예외처리
    if(selectImage.isNull()){
        QMessageBox:: critical(this, "경고", "이미지를 선택하세요");
        return;
    }

    emit SendType(4);
}

void ImageAlbum::Laser()
{
    //이미지가 선택되지 않았다면 예외처리
    if(selectImage.isNull()){
        QMessageBox:: critical(this, "경고", "이미지를 선택하세요");
        return;
    }

    emit SendType(2);
}

void ImageAlbum::Lines()
{
    //이미지가 선택되지 않았다면 예외처리
    if(selectImage.isNull()){
        QMessageBox:: critical(this, "경고", "이미지를 선택하세요");
        return;
    }

    emit SendType(0);
}

void ImageAlbum::Freehand()
{
    //이미지가 선택되지 않았다면 예외처리
    if(selectImage.isNull()){
        QMessageBox:: critical(this, "경고", "이미지를 선택하세요");
        return;
    }

    emit SendType(1);
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
    imageView->scale(1.2, 1.2);
}

void ImageAlbum::ZoomOut()
{
    imageView->scale(0.8, 0.8);
}

void ImageAlbum::LeftRotate()
{
    imageView->rotate(-45);
}

void ImageAlbum::RightRotate()
{
    imageView->rotate(45);
}

void ImageAlbum::OrigImage()
{
    if(selectImage.isNull()){
        QMessageBox:: critical(this, "경고", "이미지를 선택하세요");
        return;
    }

    emit SendType(8);

    imageView->resetTransform();
    imageScene->clear();
    imageScene->setBackgroundBrush(Qt::white);
    ui->horizontalSlider->setSliderPosition(0);
    ui->Contrast->setValue(1.0);
    ui->LengthResult->clear();
    ui->AngleResult->clear();

    selectImage = QPixmap(orignal->statusTip()).toImage();
    QSize size = imageView->viewport()->size();
    QGraphicsItem *i = imageScene->addPixmap(QPixmap(orignal->statusTip()).scaled(size, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    imageView->setAlignment(Qt::AlignCenter);
    imageScene->setSceneRect(i->sceneBoundingRect());

}

void ImageAlbum::selectItem(QListWidgetItem* item)
{
    emit SendType(8);
    imageView->resetTransform();
    ui->LengthResult->clear();
    imageView->setBackgroundBrush(Qt::white);
    imageScene->setBackgroundBrush(Qt::white);
    ui->horizontalSlider->setSliderPosition(0);
    ui->Contrast->setValue(1.0);
    ui->AngleResult->clear();

    orignal = item;
    origImage = new QImage(ui->listWidget->currentItem()->statusTip());
    selectImage = QImage(ui->listWidget->currentItem()->statusTip());


    imageScene->clear();
    QSize size = imageView->viewport()->size();
    QGraphicsItem *i = imageScene->addPixmap(QPixmap(item->statusTip()).scaled(size, Qt::KeepAspectRatio, Qt::SmoothTransformation));
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

    imageScene->clear();
    selectImage.mirror(true, false);

    QSize size = imageView->viewport()->size();
    QPixmap buf = QPixmap::fromImage(selectImage);
    QGraphicsItem *i = imageScene->addPixmap(buf.scaled(size, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    imageView->setAlignment(Qt::AlignCenter);
    imageScene->setSceneRect(i->sceneBoundingRect());

}

void ImageAlbum::HReverse()
{
    if(selectImage.isNull()){
        QMessageBox:: critical(this, "경고", "이미지를 선택하세요");
        return;
    }

    imageScene->clear();
    selectImage.mirror(false, true);

    QSize size = imageView->viewport()->size();
    QPixmap buf = QPixmap::fromImage(selectImage);
    QGraphicsItem *i = imageScene->addPixmap(buf.scaled(size, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    imageView->setAlignment(Qt::AlignCenter);
    imageScene->setSceneRect(i->sceneBoundingRect());
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
    imageScene->clear();

    QImage image_capture = buf.toImage();
    QSize size = imageView->viewport()->size();
    QGraphicsItem *i = imageScene->addPixmap(buf.scaled(size, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    imageView->setAlignment(Qt::AlignCenter);
    imageScene->setSceneRect(i->sceneBoundingRect());
    selectImage = image_capture.convertToFormat(QImage::Format_Grayscale8);

    emit SendType(2);
}

void ImageAlbum::Brightness(int value)
{
    if(selectImage.isNull()){
        QMessageBox:: critical(this, "경고", "이미지를 선택하세요");
        return;
    }

    imageScene->clear();
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
    QGraphicsItem *i = imageScene->addPixmap(buf.scaled(size, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    imageView->setAlignment(Qt::AlignCenter);
    imageScene->setSceneRect(i->sceneBoundingRect());
}

void ImageAlbum::on_horizontalSlider_sliderReleased()
{
    Brightvalue = ui->horizontalSlider->value();
    selectImage = image_brightness.convertToFormat(selectImage.format());
}

void ImageAlbum::HistEqual()
{
//    ui->Contrast->editingFinished();

    if(selectImage.isNull()){
        QMessageBox:: critical(this, "경고", "이미지를 선택하세요");
        return;
    }

    imageScene->clear();
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
    QGraphicsItem *i = imageScene->addPixmap(buf.scaled(size, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    imageView->setAlignment(Qt::AlignCenter);
    imageScene->setSceneRect(i->sceneBoundingRect());
    selectImage = image_Histogram.convertToFormat(selectImage.format());   
}

void ImageAlbum::Reverse()
{
    if(selectImage.isNull()){
        QMessageBox:: critical(this, "경고", "이미지를 선택하세요");
        return;
    }

    imageScene->clear();
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
    QGraphicsItem *i = imageScene->addPixmap(buf.scaled(size, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    imageView->setAlignment(Qt::AlignCenter);
    imageScene->setSceneRect(i->sceneBoundingRect());
    selectImage = image_Reverse.convertToFormat(selectImage.format());
}

void ImageAlbum::Contrast(double value)
{
    if(selectImage.isNull()){
        QMessageBox:: critical(this, "경고", "이미지를 선택하세요");
        return;
    }

    imageScene->clear();
    QImage image = selectImage.convertToFormat(QImage::Format_Grayscale8);
    cv::Mat in = cv::Mat(
                image.height(),
                image.width(),
                CV_8UC1,
                image.bits(),
                image.bytesPerLine());
    Mat out;
    in.convertTo(out, -1, value, 0);

    image_Contrast = QImage(
                out.data,
                out.cols,
                out.rows,
                out.step,
                QImage::Format_Grayscale8).copy();

    QSize size = imageView->viewport()->size();
    QPixmap buf = QPixmap::fromImage(image_Contrast);
    QGraphicsItem *i = imageScene->addPixmap(buf.scaled(size, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    imageView->setAlignment(Qt::AlignCenter);
    imageScene->setSceneRect(i->sceneBoundingRect());
}

void ImageAlbum::on_Contrast_editingFinished()
{
    selectImage = image_Contrast.convertToFormat(selectImage.format());
}

void ImageAlbum::Blur()
{
    if(selectImage.isNull()){
        QMessageBox:: critical(this, "경고", "이미지를 선택하세요");
        return;
    }

    imageScene->clear();
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
    blur(in, out, cv::Size(18, 18));

    //다시 볼 수 있는 형태로 이미지를 복구시킵니다.
    QImage image_Blur(
                out.data,
                out.cols,
                out.rows,
                out.step,
                QImage::Format_Grayscale8); 

    QSize size = imageView->viewport()->size();
    QPixmap buf = QPixmap::fromImage(image_Blur);
    QGraphicsItem *i = imageScene->addPixmap(buf.scaled(size, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    imageView->setAlignment(Qt::AlignCenter);
    imageScene->setSceneRect(i->sceneBoundingRect());
    selectImage = image_Blur.convertToFormat(selectImage.format());
}

void ImageAlbum::Sharpening()
{
    if(selectImage.isNull()){
        QMessageBox:: critical(this, "경고", "이미지를 선택하세요");
        return;
    }

    imageScene->update();
    QImage image = selectImage.convertToFormat(QImage::Format_Grayscale8);
    cv::Mat in = cv::Mat(
                image.height(),
                image.width(),
                CV_8UC1,
                image.bits(),
                image.bytesPerLine());

    Mat blurred;
//    cvtColor(in, out, cv::COLOR_BGR2GRAY);
    GaussianBlur(in, blurred, Size(), 8);

    float alpha = 1.f;
    Mat out = (1 + alpha) * in - alpha * blurred;

    QImage image_Sharpen(
                out.data,
                out.cols,
                out.rows,
                out.step,
                QImage::Format_Grayscale8);

    QSize size = imageView->viewport()->size();
    QPixmap buf = QPixmap::fromImage(image_Sharpen);
    QGraphicsItem *i = imageScene->addPixmap(buf.scaled(size, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    imageView->setAlignment(Qt::AlignCenter);
    imageScene->setSceneRect(i->sceneBoundingRect());
    selectImage = image_Sharpen.convertToFormat(selectImage.format());
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

//진료 종료 버튼 클릭 시 해당 환자 정보를 서버에 전송 및 해당 환자의 이미지 파일 삭제
void ImageAlbum::on_EndTreatment_clicked()
{
    if(!prescriptionCheck){
        QMessageBox:: critical(this, "경고", "처방전을 입력해주세요");
        return;
    }

    QString Data = "VTF<CR>" + PatientID + "<CR>" + PatientName;
    emit sendEndTreatment(Data);

    QDir dir(QString("./Image/%1").arg(PatientID));
    dir.removeRecursively();

    //진료 종료 버튼 클릭 시 기존 환자의 사진 담고 있는 listWidget + graphicsView + graphicsScene 클리어
    ui->listWidget->clear();
    prescriptionCheck = false;
    imageView->resetTransform();
    imageScene->clear();
}


void ImageAlbum::on_tabWidget_tabBarClicked(int index)
{
    //도형, 텍스트, 펜 그리기 이후 도구 탭 이동 시 원본으로 초기화
    if(index == 0){
        if(selectImage.isNull()){
            return;
        }
        emit SendType(8);

        imageView->resetTransform();
        imageScene->clear();
        imageScene->setBackgroundBrush(Qt::white);
        ui->horizontalSlider->setSliderPosition(0);
        ui->Contrast->setValue(1.0);
        ui->LengthResult->clear();
        ui->AngleResult->clear();

        selectImage = QPixmap(orignal->statusTip()).toImage();
        QSize size = imageView->viewport()->size();
        QGraphicsItem *i = imageScene->addPixmap(QPixmap(orignal->statusTip()).scaled(size, Qt::KeepAspectRatio, Qt::SmoothTransformation));
        imageView->setAlignment(Qt::AlignCenter);
        imageScene->setSceneRect(i->sceneBoundingRect());
    }

    //필터링 후 탭 이동 시 그려져있던 레이저 삭제
    else if(index == 1) {
        emit SendType(8);
    }
}

//스핀박스를 활용하여 폰트 사이즈 변경하는 슬롯
void ImageAlbum::on_Fontsize_valueChanged(int size)
{
    emit SendFontSize(size);
}


