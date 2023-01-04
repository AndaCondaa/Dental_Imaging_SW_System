#include "subserver.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    SubServer w;
    w.show();
    return a.exec();
}
