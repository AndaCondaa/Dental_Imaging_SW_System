#include "widget.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Widget w;
    w.setGeometry(100, 100, 300, 300);
    w.show();
    return a.exec();
}
