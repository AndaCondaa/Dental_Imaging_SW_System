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
    //sex = ui->sexComboBox->currentText();                 //라디오버튼으로바꿈
    if(ui->maleRadioButton->isChecked()==1)
        sex = "남성";
    else if(ui->femaleRadioButton->isChecked()==1)
        sex = "여성";

    date = ui->birthDateEdit->date().toString("yyyy-MM-dd");
    tel = ui->telLineEdit->text();
    address = ui->addressTextEdit->toPlainText();
    memo = ui->memoTextEdit->toPlainText();

    //qDebug() << sex << "/" << date << "/" << address;

    QString newData = "PER<CR>" + pid + "<CR>" + name + '|' + sex + '|' + date + '|' + tel + '|' + address + '|' + memo;

    emit sendNewData(newData);

}

