#include "enrollmanager.h"
#include "ui_enrollmanager.h"
#include <QMessageBox>

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
    QString /*pid, */name, sex, date, tel, address, memo;



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

    QString newData = "PER<CR>" + newPID + "<CR>" + name + '|' + sex + '|' + date + '|' + tel + '|' + address + '|' + memo;
    emit sendNewData(newData);

    QString showdata = name + '|' + sex + '|' + date + '|' + tel + '|' + address + '|' + memo;
    emit sendNewDataForShow(newPID, showdata);

    QMessageBox::information(this, tr("신규환자 등록"), \
                          tr("신규환자 등록이 완료되었습니다."));

    this->close();

    ui->nameLineEdit->clear();

    ui->maleRadioButton->clearFocus();
    ui->maleRadioButton->setAutoExclusive(false);
    ui->maleRadioButton->setChecked(false);
    ui->maleRadioButton->setAutoExclusive(true);

    ui->femaleRadioButton->clearFocus();
    ui->femaleRadioButton->setAutoExclusive(false);
    ui->femaleRadioButton->setChecked(false);
    ui->femaleRadioButton->setAutoExclusive(true);

    ui->birthDateEdit->clear();
    ui->telLineEdit->clear();
    ui->addressTextEdit->clear();
    ui->memoTextEdit->clear();
}

void EnrollManager::newPIDSended(QString sendedPID)
{
    newPID = sendedPID;
    qDebug()<<"enroll box - pid : " << newPID;
    ui->pidLineEdit->setText(newPID);
}
