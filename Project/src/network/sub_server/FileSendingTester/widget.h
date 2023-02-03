#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QFile>

#include "protocol.h"

class Widget : public QWidget
{
    Q_OBJECT

public slots:
    void cephButtonClicked();
    void panoButtonClicked();
    void buttonClicked();
    void sendFile();

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

private:
    Protocol *protocol;
    QTcpSocket *fileSocket;

    QString currentType;

    QFile file;
    QString fileName;

    int countMax = 0;
    int frameSize = 0;

signals:
};
#endif // WIDGET_H
