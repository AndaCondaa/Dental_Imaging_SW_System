#include "imagingmanager.h"
#include "ui_imagingmanager.h"

ImagingManager::ImagingManager(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ImagingManager)
{
    ui->setupUi(this);
}

ImagingManager::~ImagingManager()
{
    delete ui;
}
