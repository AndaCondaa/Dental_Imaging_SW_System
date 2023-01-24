#include "patientinfomanager.h"
#include "ui_patientinfomanager.h"

PatientInfoManager::PatientInfoManager(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PatientInfoManager)
{
    ui->setupUi(this);
    ui->clientInfoTableWidget->setColumnWidth(0,285);

    QPixmap *pixmap = new QPixmap();
        pixmap->load("user.png");
        pixmap->scaled(200,180,Qt::IgnoreAspectRatio);

    ui->patientFace->setPixmap(pixmap->scaled(ui->patientFace->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
    //ui->patientFace->setAlignment(Qt::AlignBottom | Qt::AlignCenter);
}

PatientInfoManager::~PatientInfoManager()
{
    delete ui;
}



void PatientInfoManager::on_searchPushButton_clicked()
{
    int comboBoxIndex = ui->searchComboBox->currentIndex();     //i에 검색콤보박스의 현재인덱스 번호를 저장
    //qDebug()<< comboBoxIndex;
    QString searchInfo = ui->searchLineEdit->text();
    qDebug()<< searchInfo;
    QString searchData = "PSE<CR>" + QString::number(comboBoxIndex) + "<CR>" + searchInfo;

    ui->clientInfoTableWidget->setEditTriggers(QAbstractItemView::DoubleClicked);   //검색하면 테이블 정보들 더블클릭해서 수정가능하도록 만듦
    qDebug() <<"searchData: " << searchData;
    emit sendSearchData(searchData);
}

void PatientInfoManager::searchDataSended(QString id, QString data)
{

    pid = id;
    name = data.split("|")[0];
    qDebug() << "name in searchDataSended: " << name;
    sex = data.split("|")[1];
    birthdate = data.split("|")[2];
    tel = data.split("|")[3];
    address = data.split("|")[4];
    memo = data.split("|")[5];


    //    QTableWidgetItem *item = new QTableWidgetItem;
    //    item->setText(pid);
    ui->clientInfoTableWidget->setItem(0, 0, new QTableWidgetItem(pid)/*item*/);
    ui->clientInfoTableWidget->setItem(1, 0, new QTableWidgetItem(name));
    //    qDebug() << "name in searchDataSended(in table):" << ui->clientInfoTableWidget->itemAt(1)->currentItem()->row()->text();
    qDebug() << "name in searchDataSended(in table):"<< ui->clientInfoTableWidget->item(1, 0)->text();
    ui->clientInfoTableWidget->setItem(2, 0, new QTableWidgetItem(sex));
    ui->clientInfoTableWidget->setItem(3, 0, new QTableWidgetItem(birthdate));
    ui->clientInfoTableWidget->setItem(4, 0, new QTableWidgetItem(tel));
    ui->clientInfoTableWidget->setItem(5, 0, new QTableWidgetItem(address));
    ui->clientInfoTableWidget->setItem(6, 0, new QTableWidgetItem(memo));




    //ui->tableWidget->setItem(0,0,QTableWidgetItem(pid));
}


void PatientInfoManager::on_deletePushButton_clicked()
{
    QString delData = "PDE<CR>" + pid + "<CR> "; //지울 pid를 emit으로 네트워크 매니저로 보냄/네트워크 매니저는 서버로 삭제할 데이터 전달
    emit sendDelData(delData);
}


void PatientInfoManager::on_WaitPushButton_clicked()
{
    //emit sendWaitInfo(pid, name);   //대기 명단에 추가

    //이름과 pid는 바뀌지 않는 정보지만 나머지 정보는 검색 후에 수정했을 수도 있으니 현재 테이블에 저장되어있던 값을 가지고 와 저장해준후 서버로 보내기
    qDebug() << "대기명단에 추가한 pid: " << ui->clientInfoTableWidget->itemAt(0,0)->text();
    pid = ui->clientInfoTableWidget->item(0, 0)->text();
    name = ui->clientInfoTableWidget->item(1, 0)->text();
    qDebug() << "name: " << ui->clientInfoTableWidget->itemAt(5,0)->text();

    sex = ui->clientInfoTableWidget->item(2,0)->text();
    birthdate = ui->clientInfoTableWidget->item(3,0)->text();
    tel = ui->clientInfoTableWidget->item(4,0)->text();
    address = ui->clientInfoTableWidget->item(5,0)->text();
    memo = ui->clientInfoTableWidget->item(6,0)->text();

    QString sendData = "AWL<CR>" + pid + "<CR>" + name + "|" + sex + "|" + birthdate + "|" + tel + "|" + address + "|" + memo;
    qDebug() << "서버로 보낼 대기환자 데이터: " <<sendData;
    //emit sendWaitToServer(sendData);
    emit sendWaitInfo(sendData);
}


void PatientInfoManager::on_modifyPushButton_clicked()
{
    //네트워크쪽으로 정보 넘겨주도록 signal emit하고 mainwindow에서 연결하고 서버에 넘겨서 update문으로 db 테이블 수정
    qDebug()<< "modify Button clicked";
    pid = ui->clientInfoTableWidget->item(0, 0)->text();
    name = ui->clientInfoTableWidget->item(1, 0)->text();
    sex = ui->clientInfoTableWidget->item(2,0)->text();
    birthdate = ui->clientInfoTableWidget->item(3,0)->text();
    tel = ui->clientInfoTableWidget->item(4,0)->text();
    address = ui->clientInfoTableWidget->item(5,0)->text();
    memo = ui->clientInfoTableWidget->item(6,0)->text();

    QString modifyData = "PMO<CR>" + pid + "<CR>" + name + "|" + sex + "|" + birthdate + "|" + tel + "|" + address + "|" + memo;
    qDebug()<< "modifyData: " << modifyData;
    emit sendModifyData(modifyData);

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

