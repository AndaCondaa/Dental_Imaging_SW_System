#ifndef PACKETDATA_H
#define PACKETDATA_H

#include <QObject>

class PacketData : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QString header READ header WRITE setHeader NOTIFY headerChanged);
    Q_PROPERTY(QString event READ event WRITE setEvent NOTIFY eventChanged);
    Q_PROPERTY(int type READ type WRITE setType NOTIFY typeChanged);
    Q_PROPERTY(QString msg READ msg WRITE setMsg NOTIFY msgChanged);

public:
    explicit PacketData(QString = "", QString = "", int = 0, QString = "", QObject* parent = nullptr);

    QString header();
    void setHeader(QString);

    QString event();
    void setEvent(QString);

    int type();
    void setType(int = 0);

    QString msg();
    void setMsg(QString);

    QByteArray makeSendData();

private:
    QString m_header;
    QString m_event;
    int m_type;
    QString m_msg;

signals:
    void headerChanged();
    void eventChanged();
    void typeChanged();
    void msgChanged();
};

#endif // PACKETDATA_H
