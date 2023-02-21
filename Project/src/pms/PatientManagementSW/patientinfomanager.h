/*
 * 프로그램명 : PatientManagementSW
 * 파일명 : patientInfoManager.h
 * 설명 : 환자 정보를 검색하고 확인, 수정/삭제, 검색한 환자를 대기 목록에 추가
 * 작성자 : 김유선
 * 최종 수정일 : 2023.02.21
 */


#ifndef PATIENTINFOMANAGER_H
#define PATIENTINFOMANAGER_H

#include <QWidget>
#include <QtCore>
#include <QtNetwork>
#include <QMessageBox>

#include <QtTest/QtTest>
#include <QTest>


namespace Ui {
class PatientInfoManager;
}

class PatientInfoManager : public QWidget
{
    Q_OBJECT

public:
    explicit PatientInfoManager(QWidget *parent = nullptr);
    ~PatientInfoManager();

private:
    Ui::PatientInfoManager *ui;

    QString pid, name, sex, birthdate, tel, address, memo, fileName;
    QString pidPhoto, currentFileName, changeFileName;

    QPixmap *pixmap;

    int fileSendedFlag =0;
    int searchButtonClicked=0;
    int patientInDB =0;
    int waitError=0;

    QMessageBox *qmsgBox;

private slots:
    void on_searchPushButton_clicked();
    void searchDataSended(QString, QString);
    void on_deletePushButton_clicked();
    void on_WaitPushButton_clicked();
    void on_modifyPushButton_clicked();
    void inWaitListSlot(int);
    void delFlagSended(int);
    void fileSendedSlot(int);
    void sendedNewDataForShow(QString, QString);
    void on_changePhotoPushButton_clicked();
    void sendedAWLRequest(QString);

signals:
    void sendSearchData(QString);
    void sendDelData(QString);
    void sendWaitInfo(QString);
    void sendModifyData(QString);
    void sendPIDtoWaitList(QString);
    void sendDelPID(QString);
    void downloadOrNotSignal();
    void cleanImageSignal();
    void sendTempData(QString, QString);
    void sendWaitInfoToWaitList(QString, QString);
};

#endif // PATIENTINFOMANAGER_H
