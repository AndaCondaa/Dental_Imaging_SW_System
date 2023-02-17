/*
 * 프로그램명 : PatientManagementSW
 * 파일명 : imageManager.cpp
 * 설명 : 환자 검색 시 해당 환자의 촬영 이미지를 띄워줌
 * 작성자 : 김유선
 * 최종 수정일 : 2023.02.16
 */


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

    // 라벨스타일 설정
    QString labelStyle = "QLabel { "
                              "background-color: rgb(150, 150, 150);"
                            "border-radius:10px;"
                              "color:#ffffff;"
                              "outline: 0; "
                          "}";
    ui->label_4->setStyleSheet(labelStyle);

    // 라벨에 그림자 설정
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

// 이미지를 불러오는 함수
void ImageManager::reloadImages(QString id)
{
    QDir dir("./Image");

    QStringList filters;
    filters << "*.png" << "*.jpg" << "*.bmp" << "*.gif";

    QFileInfoList fileInfoList = dir.entryInfoList(filters, QDir::Files | QDir::NoDotAndDotDot);

    ui->imageListWidget->clear();

    for(int i = 0; i < fileInfoList.count(); i++) {
        QListWidgetItem* item = new QListWidgetItem(QIcon(dir.path() + "/" + fileInfoList.at(i).fileName()), NULL, ui->imageListWidget); // , QListWidgetItem::UserType);

        item->setStatusTip(fileInfoList.at(i).fileName());

        ui->imageListWidget->addItem(item);

    };

    dir.removeRecursively();    // 화면에 사진 띄웠으면 해당 폴더 삭제

}

// 검색한 환자가 DB에 있는지 확인하고, 있으면 해당 pid를 받아 이미지를 띄우는 함수를 호출
void ImageManager::PSEDataSended(QString id)
{
    ui->imageListWidget->clear();
    qDebug() << "id: " << id;
    pid = id;

    reloadImages(id);
}

// 환자 삭제를 하면 이미지 리스트에 띄워져있던 이미지를 clear해주는 부분
void ImageManager::delPIDSendedToImg(QString id)
{
    ui->imageListWidget->clear();
}

// search버튼 클릭되면 이전 환자의 이미지가 보이지 않도록 imageManager쪽으로 signal 보낸 것을 받음
void ImageManager::cleanImageSlot()
{
    ui->imageListWidget->clear();
}
