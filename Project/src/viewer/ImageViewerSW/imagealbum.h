#ifndef IMAGEALBUM_H
#define IMAGEALBUM_H

#include <QWidget>
#include <QSplitter>
#include <QGraphicsSceneMouseEvent>

class ImageView;
class QGraphicsScene;
class QListWidget;
class QListWidgetItem;
class QGroupBox;

namespace Ui {
class ImageAlbum;
}

class ImageAlbum : public QSplitter
{
    Q_OBJECT

public:
    explicit ImageAlbum(QWidget *parent = nullptr);
    ~ImageAlbum();
    QString currentImage();

private:
    Ui::ImageAlbum *ui;
    QGraphicsScene* graphicsScene;
    QListWidget* listWidget;
    QGroupBox* groupBox;
    ImageView* imageView;
    QListWidgetItem* origImage;

    QColor paintColor;

public slots:
    void reset();
    void reloadImages();
    void selectItem(QListWidgetItem*);
    void setImage(QString);
    void ZoomIn();
    void ZoomOut();
    void LeftRotate();
    void RightRotate();
    void Brush();
    void OrigImage();
    QImage Gamma();
    void Brightness(int value);
};

#endif // IMAGEALBUM_H

