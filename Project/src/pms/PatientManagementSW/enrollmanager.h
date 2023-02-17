/*
 * 프로그램명 : PatientManagementSW
 * 파일명 : enrollManager.h
 * 설명 : 신규 환자 등록
 * 작성자 : 김유선
 * 최종 수정일 : 2023.02.16
 */


#ifndef ENROLLMANAGER_H
#define ENROLLMANAGER_H

#include <QWidget>
#include <QMessageBox>
#include <QtTest/QtTest>
#include <QTest>


namespace Ui {
class EnrollManager;
}

class EnrollManager : public QWidget
{
    Q_OBJECT

public:
    explicit EnrollManager(QWidget *parent = nullptr);
    ~EnrollManager();

private slots:
    void enrollFinished();
    void newPIDSended(QString);

    void on_selectFilePushButton_clicked();

private:
    Ui::EnrollManager *ui;
    QString newPID;


    QString pidPhoto;
    QString fileName;


    QPixmap *pixmap;

    QMessageBox *qmsgBox;

signals:
    void sendNewData(QString);
    void sendNewDataForShow(QString, QString);

};

#endif // ENROLLMANAGER_H
