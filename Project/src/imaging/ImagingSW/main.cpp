/*
 * 프로그램명 : ImagingSW
 * 파일명 : main.cpp
 * 설명 : 촬영SW
 * 작성자 : 안다미로
 * 최종 수정일 : 2023.01.06
 */

#include "mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
