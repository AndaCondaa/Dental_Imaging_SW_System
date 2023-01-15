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

    this->loadData();

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

    socket = static_cast<QTcpSocket*>(sender());
    buffer = buffers.value(socket);
    //qint32 *s = sizes.value(socket);
    //qint32 size = *s;

    buffer->append(socket->readAll());
    saveData = QString(buffer->data());
    ui->textEdit->insertPlainText(saveData);
    ui->textEdit->insertPlainText("\n");




    //어떤 이벤트인지에 따라 불러올 함수 써주기(각각 이벤트에 대한 함수 만들고 if-else문 타도록 만들자)
    QString event = saveData.split("<CR>")[0];
    QString id = saveData.split("<CR>")[1];
    QString data = saveData.split("<CR>")[2];

    qDebug() << "이벤트: " << event;

    if(event == "PER")
    {
        query->prepare("INSERT INTO patient (patient_no, patient_name, patient_sex, patient_birthdate,"
                       "patient_tel, patient_address, patient_memo)"
                       "VALUES(:patient_no, :patient_name, :patient_sex, :patient_birthdate,"
                       ":patient_tel, :patient_address, :patient_memo)");


        query->bindValue(":patient_no", id);
        query->bindValue(":patient_name", data.split("|")[0]);
        query->bindValue(":patient_sex", data.split("|")[1]);
        query->bindValue(":patient_birthdate", data.split("|")[2]);
        query->bindValue(":patient_tel", data.split("|")[3]);
        query->bindValue(":patient_address", data.split("|")[4]);
        query->bindValue(":patient_memo", data.split("|")[5]);
        query->exec();
        this->loadData();

    }
    else if(event == "PSE")
    {
        if(id == "0"){  //환자번호로 검색했을 때
            query->exec("SELECT * FROM patient WHERE patient_no = '" + data + "'");
        }
        else if(id == "1"){
            query->exec("SELECT * FROM patient WHERE patient_name = '" + data + "'");
        }
        this->loadData();
    }






    buffer->clear(); //버퍼 비워주기
}

qint32 ArrayToInt(QByteArray source)
{
    qint32 temp;
    QDataStream data(&source, QIODevice::ReadWrite);
    data >> temp;
    return temp;
}


void MainServer::loadData()
{
    /*DB를 추가하고 DB의 이름을 설정*/
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE", "databaseConnection");
    db.setDatabaseName("database.db");

    //QString patient_no, patient_name, patient_sex, patient_birthdate, patient_tel, patient_address, patient_memo;

    /*DB를 오픈해 새로운 테이블을 만듦*/
    if (db.open( )) {
        query= new QSqlQuery(db);
        query->exec("CREATE TABLE IF NOT EXISTS patient(patient_no VARCHAR(10) Primary Key,"
                    "patient_name VARCHAR(10) NOT NULL, patient_sex VARCHAR(5) NOT NULL, patient_birthdate VARCHAR(15) NOT NULL,"
                    "patient_tel VARCHAR(15) NOT NULL, patient_address VARCHAR(60) NOT NULL, patient_memo VARCHAR(100));");

        patientModel = new QSqlTableModel(this, db);
        patientModel->setTable("patient");
        patientModel->select();
        patientModel->setHeaderData(0, Qt::Horizontal, tr("No"));
        patientModel->setHeaderData(1, Qt::Horizontal, tr("Name"));
        patientModel->setHeaderData(2, Qt::Horizontal, tr("Sex"));
        patientModel->setHeaderData(3, Qt::Horizontal, tr("Birthdate"));
        patientModel->setHeaderData(4, Qt::Horizontal, tr("Telephone Number"));
        patientModel->setHeaderData(5, Qt::Horizontal, tr("Address"));
        patientModel->setHeaderData(6, Qt::Horizontal, tr("Memo"));

        ui->patientTableView->setModel(patientModel);


        //        query->exec("CREATE TABLE IF NOT EXISTS dentist(dentist_no VARCHAR(10) Primary Key,"
        //                    "dentist_name VARCHAR(10) NOT NULL, dentist_sex VARCHAR(5) NOT NULL), dentist_tel VARCHAR(15) NOT NULL);");

        //        query->exec("CREATE TABLE IF NOT EXISTS image(image_no VARCHAR(10) Primary Key, patient_no VARCHAR(10) NOT NULL,"
        //                    "dentist_no VARCHAR(10) NOT NULL, modality VARCHAR(10) NOT NULL, bodypart_examined VARCHAR(30) NOT NULL,"
        //                    "image_date VARCHAR(15) NOT NULL, image_path varchar(300) NOT NULL);");

        //        query->exec("CREATE TABLE IF NOT EXISTS report(report_no VARCHAR(10) Primary Key, patient_no VARCHAR(10) NOT NULL,"
        //                    "dentist_no VARCHAR(10) NOT NULL, report_date VARCHAR(15) NOT NULL, report_note VARCHAR(500) NOT NULL);");

        //        query->exec("CREATE TABLE IF NOT EXISTS image_relation(report_no VARCHAR(10) Primary Key, image_no VARCHAR(10) Primary Key);");









        //            view->setModel(model);
        //            model->query(Qs("select * from table"));
        //            model->select();

        //        query->exec(QString("INSERT INTO patient VALUES (%1, '%2', '%3', '%4', '%5', '%6', '%7')").arg(patient_no)
        //                    .arg(patient_name).arg(patient_sex).arg(patient_birthdate).arg(patient_tel).arg(patient_address)
        //                    .arg(patient_memo));

        //query->exec("SELECT * FROM patient where patient_sex = '" + sex + "'");
    }

}


QString MainServer::makeId( )
{
    int id;

    if(patientModel->rowCount() == 0) {
        id = 1;
        return "P" + QString::number(id).rightJustified(5,'0');
    } else {
        auto id = patientModel->data(patientModel->index(patientModel->rowCount()-1, 0)).toInt();
        id++;
        return "P" + QString::number(id).rightJustified(5,'0');
    }
}
