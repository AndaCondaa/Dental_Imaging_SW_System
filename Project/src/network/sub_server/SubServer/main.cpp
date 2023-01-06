/*
 * 프로그램명 : SubServer
 * 파일명 : main.cpp
 * 설명 : 영상 검사실용 미니 서버 (촬영SW와 Modality의 연결)
 * 작성자 : 안다미로
 * 최종 수정일 : 2023.01.06
 */

#include "subserver.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    SubServer w;
    w.show();
    return a.exec();
}
