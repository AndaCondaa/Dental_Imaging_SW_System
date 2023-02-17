/*
 * 프로그램명 : PatientManagementSW
 * 파일명 : enrollManager.cpp
 * 설명 : 신규 환자 등록
 * 작성자 : 김유선
 * 최종 수정일 : 2023.02.16
 */


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
    this->setFixedSize(782, 650);

    // 스타일시트 설정
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

    ui->telLineEdit->setInputMask("000-0000-0000");
    ui->enrollPushButton->setStyleSheet(enrollButtonStyle);
    ui->selectFilePushButton->setStyleSheet(selectFileButtonStyle);
    ui->birthDateEdit->setCalendarPopup(true);
    ui->birthDateEdit->show();

    connect(ui->enrollPushButton, SIGNAL(clicked()), this, SLOT(enrollFinished()));


    // 기본 이미지 설정
    pixmap = new QPixmap();
    pixmap->load(":/img/man.png");
    pixmap->scaled(180,180,Qt::IgnoreAspectRatio);
    ui->enrollImageLabel->setPixmap(pixmap->scaled(ui->enrollImageLabel->size(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation));



    QString msgBoxStyle = "QMessageBox { "
                          "background-color: rgb(255, 255, 255);"
                          "color:#000000;"
                          "}"
                          "QPushButton { "
                          "background-color: rgb(250, 250, 250);"
                          "border-radius:2px;"
                          "color:#ffffff;"
                          "border-color: blue"
                          "font-weight: bold;"
                          "padding-top: 3px;"
                          "padding-bottom: 3px;"
                          "outline: 0; "
                          "}";
    qmsgBox = new QMessageBox();
    qmsgBox->setStyleSheet(msgBoxStyle);

}

EnrollManager::~EnrollManager()
{
    delete ui;
}

// 환자등록을 완료했을 때. DB로 적은 정보를 보내 새로운 환자 정보가 저장되도록 한다
void EnrollManager::enrollFinished()
{
    QString name, sex, date, tel, address, memo;

    name = ui->nameLineEdit->text();

    if(ui->maleRadioButton->isChecked()==1)
        sex = "남성";
    else if(ui->femaleRadioButton->isChecked()==1)
        sex = "여성";

    date = ui->birthDateEdit->date().toString("yyyy-MM-dd");
    tel = ui->telLineEdit->text();
    address = ui->addressTextEdit->toPlainText();
    memo = ui->memoTextEdit->toPlainText();


    // 필수사항 입력하지 않았을 때 환자 등록되지 않도록 만들기(메모 빼고는 모두 필수 입력사항)
    if(name == "" || sex == "" || date == "" || tel == "" || address == "")
    {
        QMessageBox::critical(qmsgBox, tr("경고"), tr("필수 환자 정보(*)를 모두 입력해주세요."));
        return;
    }

    QString editFlag = "1";
    QString saveFileName;

    // 선택했던 파일을 디버그 파일의 Face폴더에 저장
    if(fileName.length()>0)
    {
        QDir dir(QString("./Face"));
        if (!dir.exists())
            dir.mkpath(".");


        saveFileName = QString("./Face/%1.png").arg(pidPhoto);
        QFile::copy(fileName, saveFileName);
    }

    QString newData = "SEN^PER<CR>" + newPID + "<CR>" + name + '|' + sex + '|' + date + '|' + tel + '|' + address + '|' + memo;
    emit sendNewData(newData);

    QString showdata = name + '|' + sex + '|' + date + '|' + tel + '|' + address + '|' + memo + '|' + pidPhoto + "|"+ editFlag;
    emit sendNewDataForShow(newPID, showdata);


    QMessageBox::information(qmsgBox, tr("신규환자 등록"), \
                             tr("신규환자 등록이 완료되었습니다."));


    // 환자 등록 후에는 enrollmanager를 숨김
    this->hide();

    // 등록 화면 초기화
    ui->nameLineEdit->clear();
    ui->birthDateEdit->clear();
    ui->telLineEdit->clear();
    ui->addressTextEdit->clear();
    ui->memoTextEdit->clear();
    ui->enrollImageLabel->clear();

    ui->maleRadioButton->clearFocus();
    ui->maleRadioButton->setAutoExclusive(false);
    ui->maleRadioButton->setChecked(false);
    ui->maleRadioButton->setAutoExclusive(true);

    ui->femaleRadioButton->clearFocus();
    ui->femaleRadioButton->setAutoExclusive(false);
    ui->femaleRadioButton->setChecked(false);
    ui->femaleRadioButton->setAutoExclusive(true);
}

// mainWindow에서 등록버튼 클릭하면 서버에서 새로운 환자의 pid를 반환해주는데, 이 떄 이 pid를 받는 부분
void EnrollManager::newPIDSended(QString sendedPID)
{
    newPID = sendedPID;
    qDebug()<<"enroll box - pid : " << newPID;

    ui->pidLineEdit->setText(newPID);
}

// 이미지 파일 선택하는 부분
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
