/*
 * 프로그램명 : PatientManagementSW
 * 파일명 : medicalChart.h
 * 설명 : 상세 진료기록을 확인, pdf로 저장/프린트
 * 작성자 : 김유선
 * 최종 수정일 : 2023.02.16
 */


#ifndef MEDICALCHART_H
#define MEDICALCHART_H

#include <QWidget>

#include <QtTest/QtTest>
#include <QTest>

namespace Ui {
class MedicalChart;
}

class MedicalChart : public QWidget
{
    Q_OBJECT

public:
    explicit MedicalChart(QWidget *parent = nullptr);
    ~MedicalChart();

private:
    Ui::MedicalChart *ui;

private slots:
    void patientReportInfoSended(QString, QString);
    void on_printPushButton_clicked();
};

#endif // MEDICALCHART_H
