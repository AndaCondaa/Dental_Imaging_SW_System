#include "enrollmanager.h"
#include "ui_enrollmanager.h"

EnrollManager::EnrollManager(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::EnrollManager)
{
    ui->setupUi(this);
    this->setFixedSize(550, 250);
    ui->birthDateEdit->setCalendarPopup(true);
    ui->birthDateEdit->show();
}

EnrollManager::~EnrollManager()
{
    delete ui;
}
