#include "patientinfomanager.h"
#include "ui_patientinfomanager.h"

PatientInfoManager::PatientInfoManager(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PatientInfoManager)
{
    ui->setupUi(this);
    ui->clientInfoTableWidget->setColumnWidth(0,285);
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

void PatientInfoManager::on_searchPushButton_clicked()
{
    int comboBoxIndex = ui->searchComboBox->currentIndex();     //i에 검색콤보박스의 현재인덱스 번호를 저장
    //qDebug()<< comboBoxIndex;
    QString searchInfo = ui->searchLineEdit->text();
    qDebug()<< searchInfo;
    QString searchData = "PSE<CR>" + QString::number(comboBoxIndex) + "<CR>" + searchInfo;

    emit sendSearchData(searchData);
}

void PatientInfoManager::on_deletePushButton_clicked()
{
    QString delData = "PDE<CR>" + pid + "<CR>0"; //지울 pid를 emit으로 네트워크 매니저로 보냄/네트워크 매니저는 서버로 삭제할 데이터 전달
    emit sendDelData(delData);
}



void PatientInfoManager::searchDataSended(QString id, QString data)
{
    qDebug("%d", __LINE__);
    pid = id;
    qDebug("%d", __LINE__);
    name = data.split("|")[0];
    qDebug("%d", __LINE__);
    sex = data.split("|")[1];
    qDebug("%d", __LINE__);
    birthdate = data.split("|")[2];
    tel = data.split("|")[3];
    address = data.split("|")[4];
    memo = data.split("|")[5];
qDebug("%d", __LINE__);


//    QTableWidgetItem *item = new QTableWidgetItem;
//    item->setText(pid);
    ui->clientInfoTableWidget->setItem(0, 0, new QTableWidgetItem(pid)/*item*/);
    ui->clientInfoTableWidget->setItem(1, 0, new QTableWidgetItem(name));
    ui->clientInfoTableWidget->setItem(2, 0, new QTableWidgetItem(sex));
    ui->clientInfoTableWidget->setItem(3, 0, new QTableWidgetItem(birthdate));
    ui->clientInfoTableWidget->setItem(4, 0, new QTableWidgetItem(tel));
    ui->clientInfoTableWidget->setItem(5, 0, new QTableWidgetItem(address));
    ui->clientInfoTableWidget->setItem(6, 0, new QTableWidgetItem(memo));
qDebug("%d", __LINE__);



    //ui->tableWidget->setItem(0,0,QTableWidgetItem(pid));
}



