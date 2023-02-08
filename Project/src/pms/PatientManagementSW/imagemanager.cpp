#include "imagemanager.h"
#include "ui_imagemanager.h"
#include <QDir>
#include <QListWidget>
#include <windows.h>
#include <QGraphicsEffect>

ImageManager::ImageManager(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ImageManager)
{
    ui->setupUi(this);

    QString labelStyle = "QLabel { "
                              "background-color: rgb(150, 150, 150);"
                            "border-radius:10px;"
                              "color:#ffffff;"
                              "outline: 0; "
                          "}";
    ui->label_4->setStyleSheet(labelStyle);

    QGraphicsDropShadowEffect *effect = new QGraphicsDropShadowEffect;
    effect->setBlurRadius(5);
    effect->setXOffset(5);
    effect->setYOffset(5);
    effect->setColor(QColor(220,220,220));
    ui->label_4->setGraphicsEffect(effect);




}

ImageManager::~ImageManager()
{
    delete ui;
}

void ImageManager::reloadImages(QString id)
{
//    QDir dir(QString("./Image/%1").arg(id));
    QDir dir("./Image");

    QStringList filters;
    filters << "*.png" << "*.jpg" << "*.bmp" << "*.gif";
    //filters << id + "*.*";
    QFileInfoList fileInfoList = dir.entryInfoList(filters, QDir::Files | QDir::NoDotAndDotDot);

    ui->imageListWidget->clear();
    qDebug() << "####################" << fileInfoList.count();

    for(int i = 0; i < fileInfoList.count(); i++) {
        QListWidgetItem* item = new QListWidgetItem(QIcon(dir.path() + "/" + fileInfoList.at(i).fileName()), NULL, ui->imageListWidget); //, QListWidgetItem::UserType);

        item->setStatusTip(fileInfoList.at(i).fileName());

        ui->imageListWidget->addItem(item);

    };

    dir.removeRecursively();    //화면에 사진 띄웠으면 해당 폴더 삭제

}


void ImageManager::PSEDataSended(QString id)
{
    ui->imageListWidget->clear();
    qDebug() << "id: " << id;
    pid = id;

    reloadImages(id);
}

void ImageManager::delPIDSendedToImg(QString id)
{
    ui->imageListWidget->clear();
}


void ImageManager::cleanImageSlot()
{
    ui->imageListWidget->clear();
}
