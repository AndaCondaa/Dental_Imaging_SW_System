#ifndef PACKETDATA_H
#define PACKETDATA_H

#include <QObject>

class PacketData : public QObject
{
    Q_OBJECT

    Q_PROPERTY(int id READ id WRITE setId NOTIFY idChanged);
    Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged);

public:
    explicit PacketData(int, QString, QObject* parent = nullptr);

    int id();
    void setId(int = 0);

    QString name();
    void setName(QString);

    QDataStream& data();
    void setData(QDataStream);

private:
    int m_id;
    QString m_name;

signals:
    void idChanged();
    void nameChanged();
};

#endif // PACKETDATA_H
