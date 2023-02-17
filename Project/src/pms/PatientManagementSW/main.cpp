/*
 * 프로그램명 : MainServer
 * 파일명 : main.cpp
 * 설명 : 환자관리 SW
 * 작성자 : 김유선
 * 최종 수정일 : 2023.02.16
 */


#include "mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;

    //QTest::qExec(&w);

    w.show();
    return a.exec();
}
