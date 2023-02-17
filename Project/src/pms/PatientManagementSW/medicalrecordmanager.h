/*
 * 프로그램명 : PatientManagementSW
 * 파일명 : medicalRecordManager.h
 * 설명 : 검색한 환자에 대한 진료기록 목록을 확인
 * 작성자 : 김유선
 * 최종 수정일 : 2023.02.16
 */


#ifndef MEDICALRECORDMANAGER_H
#define MEDICALRECORDMANAGER_H

#include <QWidget>
#include <QTreeWidget>

#include <QtTest/QtTest>
#include <QTest>

namespace Ui {
class MedicalRecordManager;
}

class MedicalChart;

class MedicalRecordManager : public QWidget
{
    Q_OBJECT

public:
    explicit MedicalRecordManager(QWidget *parent = nullptr);
    ~MedicalRecordManager();

private slots:
    void recordDataSended(QString, QString);

    void on_recordTreeWidget_itemDoubleClicked(QTreeWidgetItem *item, int column);

    void addNewRecord(QString);

private:
    Ui::MedicalRecordManager *ui;
    MedicalChart *medicalChart;
    QMap <int, QString> reportInfo;

    int totalRowCount = 0;

    QString patientDetail, reportDetail;

signals:
    void sendPatientReportInfo(QString, QString);
    void sendReSearchData(QString);
};

#endif // MEDICALRECORDMANAGER_H
