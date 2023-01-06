/*
 * 프로그램명 : ImagingSW
 * 파일명 : mainwindow.cpp
 * 설명 : 촬영SW의 메인GUI
 * 작성자 : 안다미로
 * 최종 수정일 : 2023.01.06
 */

#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    controlSocket = new ControlSocket(this);
}

MainWindow::~MainWindow()
{
    delete ui;
    delete controlSocket;
}

