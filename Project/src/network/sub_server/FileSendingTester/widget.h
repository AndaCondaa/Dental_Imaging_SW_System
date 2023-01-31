#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QFile>

#include "protocol.h"

class Widget : public QWidget
{
    Q_OBJECT

public slots:
    void buttonClicked();
    void sendFile();

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

private:
    Protocol *protocol;
    QTcpSocket *fileSocket;

    QByteArray totalData;

signals:
};
#endif // WIDGET_H
