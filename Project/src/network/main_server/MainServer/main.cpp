/*
 * 프로그램명 : MainServer
 * 파일명 : main.cpp
 * 설명 : 메인서버
 * 작성자 : 김유선
 * 최종 수정일 : 2023.02.16
 */


#include "mainserver.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainServer w;

    //QTest::qExec(&w);

    w.show();
    return a.exec();
}
