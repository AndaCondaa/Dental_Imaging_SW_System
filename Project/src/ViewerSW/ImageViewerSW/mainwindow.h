/*프로그램명 : ImageViewerSW
파일명 : mainwindow.h
설명 : 각 클래스들 간의 signal-slot을 연결해주는 connect문을 작성한 부분
stackedWidget을 통해 login창과 viewer창으로 구분되어 있는 클래스
작성자 : 이정연
최종 수정 날짜 : 2023.02.11*/

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <QScrollArea>
#include <QWheelEvent>
#include <QGraphicsView>
#include <QObject>
#include <QListWidgetItem>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class ImageAlbum;
class PatientInfo;
class NetworkManager;
class LoginForm;


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    QImage *img;
    ImageAlbum* m_imageAlbum;
    PatientInfo* m_patientInfo;
    NetworkManager* m_networkManager;
    LoginForm* m_logIn;

private slots:
    void ViewerOpen(QString, QString);
    void on_actionPrinter_triggered();

signals:
    void sendPrintStart();
};
#endif // MAINWINDOW_H
