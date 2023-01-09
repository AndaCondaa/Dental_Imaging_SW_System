#include "searchenrollmanager.h"
#include "ui_searchenrollmanager.h"

SearchEnrollManager::SearchEnrollManager(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SearchEnrollManager)
{
    ui->setupUi(this);
}

SearchEnrollManager::~SearchEnrollManager()
{
    delete ui;
}
