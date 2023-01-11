/*
 * 프로그램명 : ImagingSW
 * 파일명 : mainwindow.h
 * 설명 : 촬영SW의 메인GUI
 * 작성자 : 안다미로
 * 최종 수정일 : 2023.01.09
 */

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "patientmanager.h"
#include "controlpanel.h"
#include "imagingmanager.h"
#include "networkmanager.h"

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;

    PatientManager *patientManager;
    ControlPanel *controlPanel;
    ImagingManager *imagingManager;
    NetworkManager *networkManager;
};
#endif // MAINWINDOW_H
