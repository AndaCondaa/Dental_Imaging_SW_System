#include "imagemanager.h"
#include "ui_imagemanager.h"

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
