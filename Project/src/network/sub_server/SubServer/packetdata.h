#ifndef PACKETDATA_H
#define PACKETDATA_H

#include <QObject>

class PacketData : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QString event READ event WRITE setEvent NOTIFY eventChanged);
    Q_PROPERTY(int index READ index WRITE setIndex NOTIFY indexChanged);
    Q_PROPERTY(QString msg READ msg WRITE setMsg NOTIFY msgChanged);

public:
    explicit PacketData(QString = "", int = 0, QString = "", QObject* parent = nullptr);

    QString event();
    void setEvent(QString);

    int index();
    void setIndex(int = 0);

    QString msg();
    void setMsg(QString);

    QByteArray makeSendData();

private:
    QString m_event;
    int m_index;
    QString m_msg;

signals:
    void eventChanged();
    void indexChanged();
    void msgChanged();
};

#endif // PACKETDATA_H
