#include "filesendingthread.h"

FileSendingThread::FileSendingThread(QTcpSocket *socket, QObject *parent)
    : QThread{parent}, m_socket(socket)
{

}


void FileSendingThread::run()
{

}

void FileSendingThread::sendFile(qint64 byte)
{

}



