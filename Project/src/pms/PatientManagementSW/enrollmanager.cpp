#include "enrollmanager.h"
#include "ui_enrollmanager.h"
#include <QMessageBox>
#include <QFileDialog>
#include <QPixmap>

EnrollManager::EnrollManager(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::EnrollManager)
{
    ui->setupUi(this);
    this->setFixedSize(782, 680);


    QString enrollButtonStyle = "QPushButton { "
                                "background-color: #ED8817;"
                                "border-radius:10px;"
                                "color:#ffffff;"
                                "outline: 0; "
                                "}"
                                "QPushButton:hover { "
                                "background-color: #F2A754;"
                                "border-radius:10px;"
                                "color:#ffffff;"
                                "outline: 0; "
                                "}";

    QString selectFileButtonStyle = "QPushButton { "
                                    "background-color: rgb(150, 150, 150);"
                                    "border-radius:10px;"
                                    "color:#ffffff;"
                                    "outline: 0; "
                                    "}"
                                    "QPushButton:hover { "
                                    "background-color: #F2A754;"
                                    "border-radius:10px;"
                                    "color:#ffffff;"
                                    "outline: 0; "
                                    "}";

    ui->enrollPushButton->setStyleSheet(enrollButtonStyle);
    ui->selectFilePushButton->setStyleSheet(selectFileButtonStyle);

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



    //필수사항 입력하지 않았을 때 환자 등록되지 않도록 만들기(메모 빼고는 모두 필수 입력사항)
    if(name == "" || sex == "" || date == "" || tel == "" || address == "")
    {
        QMessageBox::critical(this, tr("경고"), tr("필수 환자 정보(*)를 모두 입력해주세요."));
        return;
    }




    QString editFlag = "0";
    QString saveFileName;

    //선택했던 파일을 디버그 파일의 Face폴더에 저장
    if(fileName.count()>0)
    {
        saveFileName = QString("./Face/%1.png").arg(pidPhoto);
        QFile::copy(fileName, saveFileName);
        editFlag ="1";

    }


    //qDebug() << sex << "/" << date << "/" << address;

    QString newData = "PER<CR>" + newPID + "<CR>" + name + '|' + sex + '|' + date + '|' + tel + '|' + address + '|' + memo;
    emit sendNewData(newData);

    QString showdata = name + '|' + sex + '|' + date + '|' + tel + '|' + address + '|' + memo + '|' + saveFileName + "|"+ editFlag;
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
    ui->enrollImageLabel->clear();
}

void EnrollManager::newPIDSended(QString sendedPID)
{
    newPID = sendedPID;
    qDebug()<<"enroll box - pid : " << newPID;
    ui->pidLineEdit->setText(newPID);
}



void EnrollManager::on_selectFilePushButton_clicked()
{
    pidPhoto = ui->pidLineEdit->text();


    fileName = QFileDialog::getOpenFileName(this,
                                            tr("Open Image"), QString("./Face/%1").arg(pidPhoto), tr("Image Files (*.png *.jpg *.bmp)"));

    qDebug() << "fileName"<<fileName;



    QFileDialog dialog(this);
    dialog.setNameFilter(tr("Images (*.png *.xpm *.jpg)"));

    QPixmap pix(QString("%1").arg(fileName));

    pix.scaled(150,180,Qt::IgnoreAspectRatio);
    ui->enrollImageLabel->setPixmap(pix.scaled(ui->enrollImageLabel->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));




}
