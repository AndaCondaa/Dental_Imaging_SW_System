#include "patientinfomanager.h"
#include "ui_patientinfomanager.h"

PatientInfoManager::PatientInfoManager(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PatientInfoManager)
{
    ui->setupUi(this);
}

PatientInfoManager::~PatientInfoManager()
{
    delete ui;
}

void PatientInfoManager::on_modifyPatientInfo_pushButton_clicked()
{
//    if(fd_flag)
//    {
        //이거는 mainwindow쪽에 보내면 될 듯
//        QString textData = QString("Modify_PatientInfo Button click: %1\n").arg(pid);
//        QString sendData = QString("Socket data: patient ID(%1) info changed").arg(pid);

//        ui->textBrowser->insertPlainText(textData);
//        send_flag = writeData(sendData.toStdString().c_str());

//        if(!send_flag)
//            ui->textBrowser->insertPlainText("Socket send fail\n");
//        else

//    }

    //emit modifyPInfo(pid, textData, sendData); //수정된 정보들 보내줘야 함
}

void PatientInfoManager::on_pushButton_clicked()
{
    int comboBoxIndex = ui->searchComboBox->currentIndex();     //i에 검색콤보박스의 현재인덱스 번호를 저장
    QString searchInfo = ui->searchLineEdit->text();

    QString searchData = "PSE<CR>" + QString::number(comboBoxIndex) + "<CR>" + searchInfo;

    emit sendSearchData(searchData);
}

