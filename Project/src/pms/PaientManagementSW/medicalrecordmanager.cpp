#include "medicalrecordmanager.h"
#include "ui_medicalrecordmanager.h"

MedicalRecordManager::MedicalRecordManager(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MedicalRecordManager)
{
    ui->setupUi(this);
}

MedicalRecordManager::~MedicalRecordManager()
{
    delete ui;
}
