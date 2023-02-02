#include "imagemanager.h"
#include "ui_imagemanager.h"
#include <QDir>
#include <QListWidget>
#include <windows.h>


ImageManager::ImageManager(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ImageManager)
{
    ui->setupUi(this);

}

ImageManager::~ImageManager()
{
    delete ui;
}

void ImageManager::reloadImages(QString id)
{
    QDir dir(QString("./Image/%1").arg(id));

    QStringList filters;
    filters << "*.png" << "*.jpg" << "*.bmp" << "*.gif";
    //filters << id + "*.*";
    QFileInfoList fileInfoList = dir.entryInfoList(filters, QDir::Files | QDir::NoDotAndDotDot);

    ui->imageListWidget->clear();
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
