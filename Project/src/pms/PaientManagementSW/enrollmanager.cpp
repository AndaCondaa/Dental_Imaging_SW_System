#include "enrollmanager.h"
#include "ui_enrollmanager.h"

EnrollManager::EnrollManager(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::EnrollManager)
{
    ui->setupUi(this);
    this->setFixedSize(700, 350);
    ui->birthDateEdit->setCalendarPopup(true);
    ui->birthDateEdit->show();


    connect(ui->enrollPushButton, SIGNAL(clicked()), this, SLOT(on_pushButton_clicked()));



}

EnrollManager::~EnrollManager()
{
    delete ui;
}

void EnrollManager::on_pushButton_clicked()
{
    //int id = makeID(); //만들어줘야함
    QString pid, name, sex, date, tel, address, memo;


    pid = "P00001"; //makeID 함수 만들어서 바뀌게 만들어야함
    name = ui->nameLineEdit->text();
    sex = ui->sexComboBox->currentText();
    date = ui->birthDateEdit->date().toString("yyyy-MM-dd");
    tel = ui->telLineEdit->text();
    address = ui->addressTextEdit->toPlainText();
    memo = ui->memoTextEdit->toPlainText();

    //qDebug() << sex << "/" << date << "/" << address;

    QString newData = pid + '|' + name + '|' + sex + '|' + date + '|' + tel + '|' + address + '|' + memo;


    emit sendNewData(newData);



//    if(ui->nameLineEdit->text()=="" || ui->telLineEdit->text()="")
//    {
//        return;
//    }
    //qDebug()<<ui->sexComboBox->currentText(); //안됨 ㅠㅠ


//date = ui->dateEdit->date().toString("yyyy-MM-dd")
}

