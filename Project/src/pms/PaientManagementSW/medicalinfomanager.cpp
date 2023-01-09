#include "medicalinfomanager.h"
#include "ui_medicalinfomanager.h"

MedicalInfoManager::MedicalInfoManager(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MedicalInfoManager)
{
    ui->setupUi(this);
}

MedicalInfoManager::~MedicalInfoManager()
{
    delete ui;
}
