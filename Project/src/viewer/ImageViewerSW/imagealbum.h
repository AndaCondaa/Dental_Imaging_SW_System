/*
프로그램명 : ImageViewerSW
파일명 : imagealbum.h
설명 : graphicsview, graphicsscene을 활용하여 다양한 영상처리(Sharpen, Blur, Contrast,
Brightness, 좌우반전, 상하반전, 회전, 줌인, 줌아웃 등의 기능을 담고있는 클래스
사용성을 위해 처방전 작성 버튼과 진료 종료 버튼은 해당 파일 내에 존재하고 있고, 진료 종료 버튼 클릭 시
다음 환자의 진료 시작을 시작할 수 있음
작성자 : 이정연
최종 수정 날짜 : 2023.02.11*/

#ifndef IMAGEALBUM_H
#define IMAGEALBUM_H

#include <QWidget>
#include <QGraphicsView>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsScene>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/opencv.hpp>
#include "imagescene.h"

using namespace cv;

class ImageView;
class QGraphicsScene;
class QListWidget;
class QListWidgetItem;
class ImageScene;
class Prescription;
class QChart;
class QChartView;
class QMessageBox;

namespace Ui {
class ImageAlbum;
}

class ImageAlbum : public QWidget
{
    Q_OBJECT

public:
    explicit ImageAlbum(QWidget *parent = nullptr);
    ~ImageAlbum();

private:
    Ui::ImageAlbum *ui;
    QListWidget* listWidget;
    QGraphicsView* imageView;
    QListWidgetItem* orignal;
    ImageScene* imageScene;
    Prescription* m_prescription;
    QChart *chart;
    QChartView *view;
    QMessageBox* messagebox;
    QString DoctorID;
    QString DoctorName;
    QString PatientID;
    QString PatientName;
    QString PatientSex;
    QString imageType;
    QColor paintColor;
    QImage selectImage;
    QImage image_Contrast;
    QImage image_brightness;
    QImage image_Gamma;
    bool prescriptionCheck;
    bool AllSendImageCheck;
    int Brightvalue = 0;
    int penThickness;
    void Histogram();

private slots:
    void reloadImages();
    void selectItem(QListWidgetItem*);
    void ZoomIn();
    void ZoomOut();
    void LeftRotate();
    void RightRotate();
    void BrushColor();
    void OrigImage();
    void Contrast(int);
    void Brightness(int);
    void VReverse();
    void HReverse();
    void Blur();
    void Reverse();
    void HistEqual();
    void Sharpening();
    void Thickness(int);
    void Lines();
    void Freehand();
    void Laser();
    void Cursor();
    void DeleteItem();
    void Ellipse();
    void RectangleItem();
    void TextBox();
    void Length();
    void Angle();
    void Capture();
    void Implant();
    void Gamma(int);
    void receiveCapturePos(QPointF, QPointF);
    void receivePatientInfo(QString, QString, QString);
    void receiveDoctorInfo(QString, QString);
    void receivePrescriptionFinish(QString);
    void receivePrescriptionCheck(QString);
    void receiveAllImageFileA(bool);
    void receivePrintStart();
    void receivePatientTreatmentEnd();
    void receiveCameraStart();
    void on_Prescription_clicked();
    void on_EndTreatment_clicked();
    void on_Brightness_sliderReleased();
    void on_Contrast_sliderReleased();
    void on_Gamma_sliderReleased();
    void on_ImplantcomboBox_activated(int);
    void on_tabWidget_currentChanged(int);

signals:
    void sendPrescription(QString, QString, QString, QString, QString);
    void SendLength(int, int, int, int, QString);
    void sendPrescriptiontoServer(QString);
    void sendEndTreatment(QString);
    void SendText(QString);
    void sendPrescriptionYesOrNot(QString);
    void SendBrushColor(QColor);
    void SendThickness(int);
    void SendType(int);
    void SendFontSize(int);
    void sendImplantType(int);
    void sendEndSignal();

};

#endif // IMAGEALBUM_H

