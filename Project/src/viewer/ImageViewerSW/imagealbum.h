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

using namespace cv;

class ImageView;
class QGraphicsScene;
class QListWidget;
class QListWidgetItem;
class QGroupBox;


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
    ImageView* imageView;
    QListWidgetItem* origImage;

    QImage* selectImage;
    QImage* origBrightness;
    QColor paintColor;
    int penThickness;

public slots:
    void reloadImages();
    void selectItem(QListWidgetItem*);
    void ZoomIn();
    void ZoomOut();
    void LeftRotate();
    void RightRotate();
    void BrushColor();
    void OrigImage();
    void Contrast(double value);
    void Brightness(int value);
    void Sobel();
    void VReverse();
    void HReverse();
    void Blur();
    void Reverse();
    void HistEqual();
    void Sharpening();
    void Thickness(int);
    void Lines();
    void Freehand();


signals:
    void SendBrushColor(QColor);
    void SendThickness(int);
    void SendType(int);
};

#endif // IMAGEALBUM_H

