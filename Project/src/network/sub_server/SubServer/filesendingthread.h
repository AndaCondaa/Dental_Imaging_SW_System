#ifndef FILESENDINGTHREAD_H
#define FILESENDINGTHREAD_H

#include <QThread>
#include <QTcpSocket>
#include <QFile>
#include <QFileDialog>

class FileSendingThread : public QThread
{
    Q_OBJECT
public:
    explicit FileSendingThread(QTcpSocket *socket, QObject *parent = nullptr);

private slots:
    void run();
    void sendFile(qint64);

private:
    QTcpSocket *m_socket;

signals:
    void sendingBlock(QByteArray);
};

#endif // FILESENDINGTHREAD_H
