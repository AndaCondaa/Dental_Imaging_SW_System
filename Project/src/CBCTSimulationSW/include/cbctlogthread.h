#ifndef CBCTLOGTHREAD_H
#define CBCTLOGTHREAD_H

#include <QObject>

class QFile;
class CBCTLogThread : public QObject
{
    Q_OBJECT
public:
    explicit CBCTLogThread(QObject *parent = nullptr);

signals:

};

#endif // CBCTLOGTHREAD_H
