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
class QGroupBox;
class ImageScene;
class Prescription;
class QChart;
class QChartView;


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
    QGroupBox* groupBox;
    QGraphicsView* imageView;
    QListWidgetItem* orignal;
    QImage* origImage;
    QImage selectImage;
    int Brightvalue = 0;
    QColor paintColor;
    int penThickness;
    QString imageType;

    ImageScene* imageScene;
    ImageScene::DrawType m_drawType;
    Prescription* m_prescription;
    QChart *chart;
    QChartView *view;

    bool prescriptionCheck;
    bool AllSendImageCheck;

    QString DoctorID;
    QString DoctorName;
    QString PatientID;
    QString PatientName;
    QString PatientSex;

    QImage image_Contrast;
    QImage image_brightness;
    QImage image_Gamma;

    void Histogram();


public slots:
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

    void receiveDoctorInfo(QString, QString);
    void receivePatientInfo(QString, QString, QString);
    void receivePrescriptionFinish(QString);
    void receiveAllImageFileA(bool);
    void receivePrintStart();


signals:
    void SendBrushColor(QColor);
    void SendThickness(int);
    void SendType(int);
    void sendPrescription(QString, QString, QString, QString, QString);
    void sendPrescriptiontoServer(QString);
    void sendEndTreatment(QString);
    void SendText(QString);
    void SendLength(int, int, int, int, QString);
    void SendFontSize(int);
    void sendEndSignal();
    void sendImplantType(int);

private slots:
    void on_Prescription_clicked();
    void on_EndTreatment_clicked();
    void ReceiveMeasurement(QString, double);
    void ReceiveCapturePos(QPointF, QPointF);
    void on_Fontsize_valueChanged(int arg1);
    void receiveCameraStart();
    void on_Brightness_sliderReleased();
    void on_Contrast_sliderReleased();
    void on_Gamma_sliderReleased();
    void on_ImplantcomboBox_activated(int index);
    void on_tabWidget_currentChanged(int index);
};

#endif // IMAGEALBUM_H

