#include "mainserver.h"
#include "ui_mainserver.h"
#include <QTcpServer>
#include <QTcpSocket>
#include <QSqlQuery>
#include <QSqlDatabase>
#include <QTableWidget>

static inline qint32 ArrayToInt(QByteArray source);

MainServer::MainServer(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainServer)
{
    ui->setupUi(this);
    server = new QTcpServer(this);
    connect(server, SIGNAL(newConnection()), this, SLOT(newConnection()));

    QString socket_data = QString("Listening: %1\n").arg(server->listen(QHostAddress::Any, 8001) ? "true" : "false");
    ui->textEdit->append(socket_data);

}

MainServer::~MainServer()
{
    delete ui;
}

void MainServer::newConnection()
{
    while (server->hasPendingConnections())
    {
        QTcpSocket *socket = server->nextPendingConnection();
        connect(socket, SIGNAL(readyRead()), this, SLOT(receiveData()));
        connect(socket, SIGNAL(disconnected()), this, SLOT(disconnected()));
        QByteArray *buffer = new QByteArray();
        qint32 *s = new qint32(0);
        buffers.insert(socket, buffer);
        sizes.insert(socket, s);
    }
}

void MainServer::disconnected()
{
    QTcpSocket *socket = static_cast<QTcpSocket*>(sender());
    QByteArray *buffer = buffers.value(socket);
    qint32 *s = sizes.value(socket);
    socket->deleteLater();
    delete buffer;
    delete s;
}

void MainServer::receiveData()
{

    QTcpSocket *socket = static_cast<QTcpSocket*>(sender());
    QByteArray *buffer = buffers.value(socket);
        qint32 *s = sizes.value(socket);
        qint32 size = *s;
        while (socket->bytesAvailable() > 0)
        {
            buffer->append(socket->readAll());
            while ((size == 0 && buffer->size() >= 4) || (size > 0 && buffer->size() >= size)) //While can process data, process it
            {
                if (size == 0 && buffer->size() >= 4) //if size of data has received completely, then store it on our global variable
                {
                    size = ArrayToInt(buffer->mid(0, 4));
                    *s = size;
                    buffer->remove(0, 4);
                }

                if (size > 0 && buffer->size() >= size) // If data has received completely, then emit our SIGNAL with the data
                {
                    ui->textEdit->insertPlainText(QString(buffer->data()));
                    QByteArray data = buffer->mid(0, size);
                    buffer->remove(0, size);
                    size = 0;
                    *s = size;
                    emit dataReceived(data);
                }
            }
        }
}

qint32 ArrayToInt(QByteArray source)
{
    qint32 temp;
    QDataStream data(&source, QIODevice::ReadWrite);
    data >> temp;
    return temp;
}

//QString MainServer::makeId()
//{

//}

void MainServer::loadData()
{
    /*DB를 추가하고 DB의 이름을 설정*/
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE", "Patient");
    db.setDatabaseName("patient.db");

    QString name;
    QString sex;
    /*DB를 오픈해 새로운 테이블을 만듦*/
    if (db.open( )) {
        query= new QSqlQuery(db);
        query->exec("CREATE TABLE IF NOT EXISTS patient(patient_no VARCHAR(10) Primary Key,"
                    "patient_name VARCHAR(10) NOT NULL, patient_sex VARCHAR(5) NOT NULL, patient_birthdate DATE NOT NULL,"
                    "patient_tel VARCHAR(15) NOT NULL, patient_address VARCHAR(60) NOT NULL, patient_memo VARCHAR(100));");

        query->exec("CREATE TABLE IF NOT EXISTS dentist(dentist_no VARCHAR(10) Primary Key,"
                    "dentist_name VARCHAR(10) NOT NULL, dentist_sex VARCHAR(5) NOT NULL), dentist_tel VARCHAR(15) NOT NULL);");

        query->exec("CREATE TABLE IF NOT EXISTS image(image_no VARCHAR(10) Primary Key, patient_no VARCHAR(10) NOT NULL,"
                    "dentist_no VARCHAR(10) NOT NULL, modality VARCHAR(10) NOT NULL, bodypart_examined VARCHAR(30) NOT NULL,"
                    "image_date DATE NOT NULL, image_path varchar(300) NOT NULL);");

        query->exec("CREATE TABLE IF NOT EXISTS report(report_no VARCHAR(10) Primary Key, patient_no VARCHAR(10) NOT NULL,"
                    "dentist_no VARCHAR(10) NOT NULL, report_date DATE NOT NULL, report_note VARCHAR(500) NOT NULL);");

        query->exec("CREATE TABLE IF NOT EXISTS image_relation(report_no VARCHAR(10) Primary Key, image_no VARCHAR(10) Primary Key);");

        query->exec("INSERT INTO patient(patient_name, patient_sex) VALUES('" + name + "', 'woman')");
        query->exec("SELECT * FROM patient where patient_sex = '" + sex + "'");
    }






}
