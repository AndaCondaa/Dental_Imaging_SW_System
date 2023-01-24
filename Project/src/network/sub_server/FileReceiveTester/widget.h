#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QTcpServer>
#include <QFile>

class Widget : public QWidget
{
    Q_OBJECT

public slots:
    void newFileClient();
    void receiveFile();

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

    QTcpServer *server;
    QMap<QTcpSocket*, int> fileSocketMap;

    QFile* file;                                // File Object for FileSending Protocol
    qint64 totalSize;                           // Total size of File that clients are sending
    qint64 byteReceived = 0;                    // size of File read currently
    QByteArray inBlock;                         // Units divided to transfer files
    QString fileName;                           // Receiving FileName
    QString checkFileName;                      // Previous File Name for checking new file
    qint64 loadSize;                // File Size
    qint64 byteToWrite;             // File Size per a block
    QByteArray outBlock;            // Block for sending
};
#endif // WIDGET_H
