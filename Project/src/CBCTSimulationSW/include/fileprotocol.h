#ifndef FILEPROTOCOL_H
#define FILEPROTOCOL_H

#include <QObject>

class FileProtocol : public QObject
{
    Q_OBJECT
public:
    explicit FileProtocol(QObject *parent = nullptr);

signals:

};

#endif // FILEPROTOCOL_H
