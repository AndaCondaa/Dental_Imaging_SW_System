/*
 * 프로그램명 : PatientManagementSW
 * 파일명 : patientStatusManager.h
 * 설명 : 대기 환자 목록 확인, 촬영요청, 수납처리
 * 작성자 : 김유선
 * 최종 수정일 : 2023.02.21
 */


#ifndef PATIENTSTATUSMANAGER_H
#define PATIENTSTATUSMANAGER_H

#include <QWidget>
#include <QTreeWidget>
#include <QMessageBox>

#include <QtTest/QtTest>
#include <QTest>

namespace Ui {
class PatientStatusManager;
}

class PatientStatusManager : public QWidget
{
    Q_OBJECT

public:
    explicit PatientStatusManager(QWidget *parent = nullptr);
    ~PatientStatusManager();

private:
    Ui::PatientStatusManager *ui;
    QString treatPID, treatName, payPID, payName;

    QTreeWidgetItem* selectedTreatRow, *selectedPayRow;

    QMap<int,QString> oldList;
    QString header;

    QMessageBox *qmsgBox;

private slots:
    void waitInfoSended(QString, QString);
    void on_waitPaymentTreeWidget_itemClicked(QTreeWidgetItem *item, int column);
    void on_waitTreatmentTreeWidget_itemClicked(QTreeWidgetItem *item, int column);
    void on_shootRequestPushButton_clicked();

    void statusRequestSended(QString);

    void on_paymentPushButton_clicked();

    void PIDsendedtoWaitList(QString);

    void delPIDSended(QString);

    void oldListSended(QString);

    void waitListClearSlot();

signals:
    void sendRequest(QString);
    void inWaitListSignal(int);
    void sendDelFlag(int);
    void sendPayInfo(QString);
};

#endif // PATIENTSTATUSMANAGER_H
