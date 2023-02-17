/*
 * 프로그램명 : ImagingSW
 * 파일명 : packetdata.h
 * 설명 : 서브서버용 패킷 데이터 클래스
 * 작성자 : 안다미로
 * 최종 수정일 : 2023.02.12
 */

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

    // Getter 함수
    QString header();
    QString event();
    int type();
    QString msg();

    // Setter 함수
    void setHeader(QString);
    void setEvent(QString);
    void setType(int = 0);
    void setMsg(QString);

    QByteArray makeSendData();      // 전송패킷용 데이터 구성

private:
    QString m_header;
    QString m_event;
    int m_type;
    QString m_msg;

signals:
    // 변수 변경 노티시그널
    void headerChanged();
    void eventChanged();
    void typeChanged();
    void msgChanged();
};

#endif // PACKETDATA_H
