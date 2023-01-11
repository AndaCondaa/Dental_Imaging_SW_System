#ifndef PACKETDATA_H
#define PACKETDATA_H

#include <QObject>

class PacketData : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QString event READ event WRITE setEvent NOTIFY eventChanged);
    Q_PROPERTY(int pid READ pid WRITE setPid NOTIFY pidChanged);
    Q_PROPERTY(QString msg READ msg WRITE setMsg NOTIFY msgChanged);

public:
    explicit PacketData(QString = "", int = 0, QString = "", QObject* parent = nullptr);

    QString event();
    void setEvent(QString);

    int pid();
    void setPid(int = 0);

    QString msg();
    void setMsg(QString);

    QByteArray makeSendData();

private:
    QString m_event;
    int m_pid;
    QString m_msg;

signals:
    void eventChanged();
    void pidChanged();
    void msgChanged();
};

#endif // PACKETDATA_H
