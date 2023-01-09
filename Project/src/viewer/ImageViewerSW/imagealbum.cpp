#include <QSplitter>
#include <QListWidget>
#include <QDir>
#include <QFileInfo>
#include <QGraphicsScene>
#include <QGroupBox>
#include <QColorDialog>

#include "imagealbum.h"
#include "ui_imagealbum.h"
#include "imageview.h"

#define LIMIT_UBYTE(n) (n > UCHAR_MAX) ? UCHAR_MAX:(n<0) ? 0 : n

ImageAlbum::ImageAlbum(QWidget *parent)
    : QSplitter(parent), ui(new Ui::ImageAlbum)
{
    imageView = new ImageView(this);
    imageView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    imageView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    imageView->setScene(imageView->graphicsScene);
    imageView->graphicsScene->setSceneRect(0, 0,
    imageView->graphicsScene->width(), imageView->graphicsScene->height());
    imageView->grabGesture(Qt::PinchGesture);

    listWidget = new QListWidget(this);
    listWidget->setViewMode(QListWidget::IconMode);
    listWidget->setIconSize(QSize(120, 80));
    listWidget->setResizeMode(QListWidget::Adjust);
    listWidget->setFlow(QListWidget::TopToBottom);
    listWidget->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    connect(listWidget, SIGNAL(itemDoubleClicked(QListWidgetItem*)), SLOT(selectItem(QListWidgetItem*)));


    setOrientation(Qt::Horizontal);
    addWidget(listWidget);
    addWidget(imageView);
//    setStretchFactor(0, 3);

    QList<int> list;
    list << 520 << 100;
    setSizes(list);

    reloadImages();

    ui->setupUi(this);

    connect(ui->ZoomIn, SIGNAL(clicked()), this, SLOT(ZoomIn()));
    connect(ui->ZoomOut, SIGNAL(clicked()), this, SLOT(ZoomOut()));
    connect(ui->LeftRotate, SIGNAL(clicked()), this, SLOT(LeftRotate()));
    connect(ui->RightRotate, SIGNAL(clicked()), this, SLOT(RightRotate()));
    connect(ui->Brush, SIGNAL(clicked()), this, SLOT(Brush()));
    connect(ui->OrigImage, SIGNAL(clicked()), this, SLOT(OrigImage()));
    connect(ui->horizontalSlider, SIGNAL(sliderMoved(int)), this, SLOT(Brightness(int)));
}

ImageAlbum::~ImageAlbum()
{

}

void ImageAlbum::reloadImages()
{
    QDir dir(".");
    QStringList filters;
    filters << "*.png" << "*.jpg" << "*.bmp";
    QFileInfoList fileInfoList = dir.entryInfoList(filters, QDir::Files | QDir::NoDotAndDotDot);
    imageView->graphicsScene->clear();

    listWidget->clear();
    for(int i = 0; i < fileInfoList.count(); i++) {
        QListWidgetItem* item = new QListWidgetItem(QIcon(fileInfoList.at(i).fileName()), NULL, listWidget); //, QListWidgetItem::UserType);
        item->setStatusTip(fileInfoList.at(i).fileName());
        listWidget->addItem(item);
    };


    //    QFileDialog dlg;
    //    QString filePath = dlg.getOpenFileName(this, "Load Image", "", "Image Files (*.png *.jpg *.bmp)");
    //    QString fileName = filePath.section("/", -1);
    //    QImage *img = new QImage(filePath);

    //    QSize size = img->size();
    //    img->scaled(size);

    //    QPixmap buf = QPixmap::fromImage(*img);
    //    buf = buf.scaled(ui->graphicsView->width(), ui->graphicsView->height());

    //    QGraphicsScene* scene = new QGraphicsScene;

    //    ui->graphicsView->setScene(scene);
    //    scene->addPixmap(buf);
}

void ImageAlbum::Brush()
{
//    imageView->setDragMode(QGraphicsView::NoDrag);
//    paintColor = QColorDialog::getColor(paintColor, this);

//    QImage *img = new QImage(listWidget->currentItem()->statusTip());

    QImage *img = new QImage(Gamma());

    QSize size = img->size();
    img->scaled(size);

    QPixmap buf = QPixmap::fromImage(*img);
    buf = buf.scaled(ui->graphicsView->width(), ui->graphicsView->height());

    QGraphicsScene* scene = new QGraphicsScene;

    ui->graphicsView->setScene(scene);
    scene->addPixmap(buf);
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
    imageView->resetTransform();
    imageView->graphicsScene->clear();
    imageView->graphicsScene->addPixmap(QPixmap(origImage->statusTip()).scaled(640, 480,
               Qt::KeepAspectRatio, Qt::SmoothTransformation));
}

void ImageAlbum::selectItem(QListWidgetItem* item)
{
    origImage = item;
    imageView->resetTransform();
    imageView->graphicsScene->clear();
    imageView->graphicsScene->addPixmap(QPixmap(item->statusTip()).scaled(640, 480,
               Qt::KeepAspectRatio, Qt::SmoothTransformation));
}

void ImageAlbum::setImage(QString path)
{
    imageView->resetTransform();
    imageView->graphicsScene->clear();
    imageView->graphicsScene->addPixmap(QPixmap(path).scaled(640, 480, Qt::KeepAspectRatio, Qt::SmoothTransformation));

    QFileInfo fileInfo(path);
    for(int i = 0; i < listWidget->count(); i++) {
        if(listWidget->item(i)->statusTip() == fileInfo.fileName()) {
            listWidget->setCurrentRow(i);
            break;
        }
    }

}

QString ImageAlbum::currentImage()
{
    return (listWidget->currentRow() >=0) ? listWidget->currentItem()->statusTip() : "";
}

void ImageAlbum::reset()
{
    imageView->resetTransform();
}

void ImageAlbum::Brightness(int value)
{
    QImage *image = new QImage(listWidget->currentItem()->statusTip());;

    for(int x = 0; x < image->width(); ++x)
        for(int y = 0 ; y < image->height(); ++y) {
           const QRgb rgb = image->pixel(x, y);
           const double r = LIMIT_UBYTE(qRed(rgb) + value);
           const double g = LIMIT_UBYTE(qGreen(rgb) + value);
           const double b = LIMIT_UBYTE(qBlue(rgb) + value);
           image->setPixelColor(x, y,
             QColor(
               r,
               g,
               b));
        }

    QSize size = image->size();
    image->scaled(size);

    QPixmap buf = QPixmap::fromImage(*image);
    buf = buf.scaled(ui->graphicsView->width(), ui->graphicsView->height());

    QGraphicsScene* scene = new QGraphicsScene;

    ui->graphicsView->setScene(scene);
    scene->addPixmap(buf);
}

QImage ImageAlbum::Gamma()
{
    QImage *image = new QImage(listWidget->currentItem()->statusTip());

//    for(int x = 0; x < image->width(); ++x)
//        for(int y = 0 ; y < image->height(); ++y) {
//           const QRgb rgb = image->pixel(x, y);
//           const double r = qRed(rgb) / 255.0;
//           const double g = qGreen(rgb) / 255.0;
//           const double b = qBlue(rgb) / 255.0;
//           image->setPixelColor(x, y,
//             QColor(
//               255 * std::pow(r, 0.2),
//               255 * std::pow(g, 0.2),
//               255 * std::pow(b, 0.2)));
//        }


    for(int x = 0; x < image->width(); ++x)
        for(int y = 0 ; y < image->height(); ++y) {
           const QRgb rgb = image->pixel(x, y);
           const double r = LIMIT_UBYTE(qRed(rgb) - 50);
           const double g = LIMIT_UBYTE(qGreen(rgb) - 50);
           const double b = LIMIT_UBYTE(qBlue(rgb) - 50);
           image->setPixelColor(x, y,
             QColor(
               r,
               g,
               b));

//           qDebug() << "r : " << r;
//           qDebug() << "g : " << g;
//           qDebug() << "b : " << b;
        }
    return *image;
}

