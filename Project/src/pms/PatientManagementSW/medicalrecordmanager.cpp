#include "medicalrecordmanager.h"
#include "ui_medicalrecordmanager.h"

#include "medicalchart.h"

MedicalRecordManager::MedicalRecordManager(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MedicalRecordManager)
{
    ui->setupUi(this);
    medicalChart = new MedicalChart(0);

    connect(this, SIGNAL(sendPatientReportInfo(QString, QString)), medicalChart, SLOT(patientReportInfoSended(QString, QString)));
}

MedicalRecordManager::~MedicalRecordManager()
{
    delete ui;
}


void MedicalRecordManager::recordDataSended(QString sendedID, QString sendedData)   // id는 필요없을수도 있겠다
{
    ui->recordTreeWidget->clear();

    QString patientName, patientSex, patientBirth, patientTel, patientAddress, patientMemo;
    patientName = sendedData.split("|")[0];
    patientSex = sendedData.split("|")[1];
    patientBirth = sendedData.split("|")[2];
    patientTel = sendedData.split("|")[3];
    patientAddress = sendedData.split("|")[4];
    patientMemo = sendedData.split("|")[5];


    patientDetail = sendedID + "|" + sendedData.split("<NEL>")[0];
qDebug() << "patientDetail" << patientDetail;


    QString rowData, reportID, doctorID, reportDate, dentistName;
    qDebug()<<"<NEL> count: " <<sendedData.count("<NEL>");
    for(int i=1; i<sendedData.count("<NEL>"); i++)
    {
        rowData = sendedData.split("<NEL>")[i];
        reportID = rowData.split("|")[0];
        //QString patientID = rowData.split("|")[1];
        doctorID = rowData.split("|")[2];
        reportDate = rowData.split("|")[3];
        QString patientNote = rowData.split("|")[4];
        dentistName = rowData.split("|")[5];

        QTreeWidgetItem* row = new QTreeWidgetItem;
        ui->recordTreeWidget->addTopLevelItem(row);
        row->setText(0, reportID);
        row->setText(1, reportDate);
        row->setText(2, dentistName);

        reportInfo.insert(i-1, rowData);

        totalRowCount += 1;
    }


}

void MedicalRecordManager::on_recordTreeWidget_itemDoubleClicked(QTreeWidgetItem *item, int column)
{
    qDebug()<<"item row: " << ui->recordTreeWidget->currentIndex().row() <<"/ column: "<<column;
    int currentRow = ui->recordTreeWidget->currentIndex().row();

    qDebug() <<"ddd: "<<reportInfo.find(currentRow).value();

    reportDetail = reportInfo.find(currentRow).value();

//    for(int i=0; i<totalRowCount;i++)
//    {
//        if(i == reportInfo.firstKey())
//    }

    medicalChart->show();

    emit sendPatientReportInfo(patientDetail, reportDetail);
}

void MedicalRecordManager::addNewRecord(QString newRecordInfo)
{
    qDebug() << "newRecordInfo" << newRecordInfo;




    //다시 서치한거같은 효과 주기
    QString searchData = "PSE<CR>0<CR>" + newRecordInfo.split("<CR>")[1]; //pid담아서 pse를 써서 보냄
    emit sendReSearchData(searchData);



}
