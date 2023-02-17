/*
 * 프로그램명 : PatientManagementSW
 * 파일명 : mainWindow.h
 * 설명 : 로그인과 PMS의 메인화면
 * 작성자 : 김유선
 * 최종 수정일 : 2023.02.16
 */


#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <QtTest/QtTest>
#include <QTest>

namespace Ui { class MainWindow ; }

class EnrollManager;
class ImageManager;
class MedicalRecordManager;
class PatientInfoManager;
class PatientStatusManager;
class NetworkManager;
class ChoosePatientManager;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void enrollClient();
    void on_pushButton_clicked();

    void changeScreenSlot(int);

private:
    Ui::MainWindow *ui;

    EnrollManager *enrollManager;
    ImageManager *imageManager;
    MedicalRecordManager *medicalRecordManager;
    PatientInfoManager *patientInfoManager;
    PatientStatusManager *patientStatusManager;
    NetworkManager *networkManager;
    ChoosePatientManager *choosePatientManager;

    QPixmap *pixmap;



signals:
    void requestPID(QString);
    void sendIP(QString, int);
};
#endif // MAINWINDOW_H
