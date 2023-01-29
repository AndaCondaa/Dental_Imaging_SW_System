#include "mainserver.h"
#include "ui_mainserver.h"
#include <QTcpServer>
#include <QTcpSocket>
#include <QSqlQuery>
#include <QSqlDatabase>
#include <QTableWidget>
#include <QSqlRecord>
#include <QTcpSocket>

//mvc패턴 적용시키기 위해 정보 변경&삭제시 뷰어와 이미징 모듈에 패킷 보내줄 것 ex. PMO<CR>P00001<CR>NULL

static inline qint32 ArrayToInt(QByteArray source);

MainServer::MainServer(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainServer)
{
    ui->setupUi(this);
    server = new QTcpServer(this);

    QTcpSocket* socket = (QTcpSocket*)(sender());
    connect(server, SIGNAL(newConnection()), this, SLOT(newConnection()));

    QString socket_data = QString("MainServer Listening: %1\n").arg(server->listen(QHostAddress::Any, 8000) ? "true" : "false");
    ui->textEdit->append(socket_data);



    fileServer = new QTcpServer(this);
    //    QTcpSocket* fileSocket = dynamic_cast<QTcpSocket*>(sender());
    //    QTcpSocket* pmsFileSocket = dynamic_cast<QTcpSocket*>(sender());
    //    QTcpSocket* imagingFileSocket = dynamic_cast<QTcpSocket*>(sender());
    //    QTcpSocket* viewerFileSocket = dynamic_cast<QTcpSocket*>(sender());
    QString fileSocket_data = QString("FileServer Listening: %1\n").arg(fileServer->listen(QHostAddress::Any, 8001) ? "true" : "false");
    connect(fileServer, SIGNAL(newConnection()), this, SLOT(newFileConnection()));
    ui->textEdit->append(fileSocket_data);


    this->loadData();
    

    //connect(this, SIGNAL(sendNewPID(QString)), SLOT(sendDataToClient(QString))); //구현 다 한 후에 헤더에 선언하기
    
}

MainServer::~MainServer()
{
    delete ui;
}

void MainServer::newConnection()
{
    QTcpSocket* socket = (QTcpSocket*)(sender());
    while (server->hasPendingConnections())
    {
        //QTcpSocket *socket = server->nextPendingConnection();
        socket = server->nextPendingConnection();
        connect(socket, SIGNAL(readyRead()), this, SLOT(receiveData()));
        connect(socket, SIGNAL(disconnected()), this, SLOT(disconnected()));
        QByteArray *buffer = new QByteArray();
        qint32 *s = new qint32(0);
        buffers.insert(socket, buffer);
        sizes.insert(socket, s);
    }
}

void MainServer::newFileConnection()
{
    qDebug("%d", __LINE__);
    QTcpSocket* fileSocket = fileServer->nextPendingConnection();           //receivedSocket에 fileServer에서 저장해두었던 다음 보류중인 연결을 연결해준다
    connect(fileSocket, SIGNAL(readyRead()), this, SLOT(receiveFile()));     //받은 소켓에서 정보를 읽어 serverform에서 파일 전송이 가능하도록 만듦
    qDebug("new file connection");
    qDebug() << fileSocket->readAll().toStdString().c_str();
    qDebug("%d", __LINE__);
    //    qDebug("can transfer files");

    //    QString event = saveData.split("<CR>")[0];
    //    QString id = saveData.split("<CR>")[1];
    //    QString data = saveData.split("<CR>")[2];

    //    qDebug() << "이벤트: " << event;

    //    if(event == "CNT"){

    //        /*어떤 모듈과 연관이 있는 소켓인지 알 수 있도록 map에 연결해 저장하는 부분*/
    //        if(id == "PMS")
    //        {
    //            pmsFileSocket = dynamic_cast<QTcpSocket*>(sender());
    //            fileSocketMap.insert(pmsFileSocket, "PMS");
    //            qDebug() << "pmsSocket ready";
    //        }
    //        else if(id == "IMG")
    //        {
    //            imagingSocket = dynamic_cast<QTcpSocket*>(sender());
    //            fileSocketMap.insert(imagingSocket, "IMG");
    //            qDebug() << "imagingSocket ready";
    //        }
    //        else if(id == "VEW")
    //        {
    //            viewerSocket = dynamic_cast<QTcpSocket*>(sender());
    //            fileSocketMap.insert(viewerSocket, "VEW");
    //            qDebug() << "viewerSocket ready";
    //        }
    //    }


    //    connect(pmsFileSocket, SIGNAL(readyRead()), this, SLOT(receiveFile()));
    //    connect(imagingSocket, SIGNAL(readyRead()), this, SLOT(receiveFile()));
    //    connect(viewerSocket, SIGNAL(readyRead()), this, SLOT(receiveFile()));
}

void MainServer::receiveFile()
{
    //ex.CNT<CR>IMG<CR>NULL

    QTcpSocket *socket = dynamic_cast<QTcpSocket*>(sender());

    if (pmsFileSocket != socket && imagingFileSocket != socket && viewerFileSocket != socket) {
        QByteArray arr = socket->readAll();
        QString id = QString(arr).split("<CR>")[1];
        if (id == "IMG") {
            qDebug("%d: ididididididi", __LINE__);
            imagingFileSocket = socket;
        }
        else if (id == "PMS") {
            qDebug("%d: RECEIVED PMS SOCKET", __LINE__);
            qDebug() << "pmsFileSocket saveData: " << QString(arr);
            pmsFileSocket = socket;

        } else if(id == "VEW") {
            qDebug("%d: RECEIVED VIEWER SOCKET", __LINE__);
            qDebug() << "viewerFileSocket saveData: " << QString(arr);
            viewerFileSocket =socket;
        }
        return;
    }


    //    saveFileData = QString(receiveFileData);
    //    ui->textEdit->insertPlainText("FileData: " + saveFileData);
    //    ui->textEdit->insertPlainText("\n");

    // Beginning File Transfer
    if (byteReceived == 0) {                                    // First Time(Block) , var byteReceived is always zero
        checkFileName = fileName;                               // 다음 패킷부터 파일이름으로 구분하기 위해 첫 패킷에서 보낸 파일이름을 임시로 저장

        QDataStream in(imagingFileSocket);
        in.device()->seek(0);
        in >> totalSize >> byteReceived >> fileName;
        if(checkFileName == fileName) return;

        QFileInfo info(fileName);
        currentPID = info.fileName();

        QDir dir(QString("./Image/%1").arg(currentPID.first(6)));
        if (!dir.exists())
            dir.mkpath(".");

        //QFileInfo info(fileName);
        QString currentFileName = dir.path() + "/" +info.fileName();



        file = new QFile(currentFileName);
        file->open(QFile::WriteOnly);

    } else {
        if(checkFileName == fileName) return;
        inBlock = imagingFileSocket->readAll();

        byteReceived += inBlock.size();
        file->write(inBlock);
        file->flush();
    }

    if (byteReceived == totalSize) {        // file sending is done
        qDebug() << QString("%1 receive completed").arg(fileName);
        inBlock.clear();
        byteReceived = 0;
        totalSize = 0;
        file->close();
        delete file;
qDebug("%d", __LINE__);
        sendFile();
qDebug("%d", __LINE__);
    }



}

//void MainServer::receiveFile()
//{
//    QTcpSocket *socket = dynamic_cast<QTcpSocket*>(sender());

//        // Beginning File Transfer
//        if (byteReceived == 0) {        // First Time(Block) , var byteReceived is always zero
//            checkFileName = fileName;
//            QDataStream in(socket);
//            in.device()->seek(0);
//            in >> totalSize >> byteReceived >> fileName;
//            if(checkFileName == fileName) return;   //이전의 파일이름과 같으면 그냥 return 함

//            QDir dir(QString("image/%1/").arg(currentPID));
//            if (!dir.exists())
//                dir.mkpath(".");

//            QFileInfo info(fileName);
//            QString currentFileName = dir.path() + "/"+ info.fileName();
//            qDebug() << info.fileName();
//            qDebug() << currentFileName;
//            file = new QFile(currentFileName);
//            file->open(QFile::WriteOnly);
//        } else {
//            if(checkFileName == fileName) return;
//            inBlock = socket->readAll();

//            byteReceived += inBlock.size();
//            file->write(inBlock);
//            file->flush();
//        }

//        if (byteReceived == totalSize) {        // file sending is done
//            qDebug() << QString("%1 receive completed").arg(fileName);
//            inBlock.clear();
//            byteReceived = 0;
//            totalSize = 0;
//            file->close();
//            delete file;
//        }

//}

void MainServer::goOnSend(qint64 numBytes)
{
    qDebug() <<byteToWrite;
    qDebug("%d", __LINE__);
    /*파일의 전체 크기에서 numBytes씩만큼 나누어 전송*/
    byteToWrite -= numBytes; // 데이터 사이즈를 유지
qDebug("%d", __LINE__);
qDebug() << file->size();
    outBlock = file->read(qMin(byteToWrite, numBytes));
qDebug("%d", __LINE__);

    if(sendFileFlag==0)
        pmsFileSocket->write(outBlock);

    //정연이
    else if(sendFileFlag==1)
        viewerFileSocket->write(outBlock);
qDebug("%d", __LINE__);
    if (byteToWrite == 0) {                 // 전송이 완료되었을 때(이제 더이상 남은 파일 크기가 없을 때)
        qDebug("%d", __LINE__);
        qDebug("File sending completed!");
        file->flush();
    }
    qDebug("%d", __LINE__);
}

void MainServer::sendFile()
{
    qDebug() << saveFileData;
    //QString event = saveFileData.split("<CR>")[0];
    //QString id = saveFileData.split("<CR>")[1];


    if(sendFileFlag==0)
        connect(pmsFileSocket, SIGNAL(bytesWritten(qint64)), SLOT(goOnSend(qint64)));  //데이터를 보낼 준비가되면 다른 데이터를 보내고, 데이터를 다 보냈을 때는 데이터 전송을 끝냄
//정연
    else if(sendFileFlag==1)
        connect(viewerFileSocket, SIGNAL(bytesWritten(qint64)), SLOT(goOnSend(qint64, viewerFileSocket)));
qDebug("%d", __LINE__);
    loadSize = 0;
    byteToWrite = 0;
    totalSize = 0;
    outBlock.clear();
qDebug("%d", __LINE__);
    //QString filename = id; //여기까지함


qDebug() << currentPID;
    if(currentPID.length()) {

        file = new QFile(QString("./Image/%1/%2").arg(currentPID.first(6)).arg(currentPID));
        file->open(QFile::ReadOnly);

        qDebug() << QString("file %1 is opened").arg(currentPID);

        qDebug() <<"@@@@@@@@file->size(): " << file->size();

        byteToWrite = totalSize = file->size(); // Data remained yet
        loadSize = 1024; // Size of data per a block

        QDataStream out(&outBlock, QIODevice::WriteOnly);
        out << qint64(0) << qint64(0) << currentPID;

        totalSize += outBlock.size();
        byteToWrite += outBlock.size();

        out.device()->seek(0);
        out << totalSize << qint64(outBlock.size());

        if(sendFileFlag==0)
            pmsFileSocket->write(outBlock); // Send the read file to the socket    //서버로 보내줌
//정연
        else if(sendFileFlag==1)
            viewerFileSocket->write(outBlock);


        //        if(id == "PMS")
//        {
//            fileSocketMap.key("PMS")->write(outBlock);
//        }
//        if(id == "VEW")
//        {
//            fileSocketMap.key("VEW")->write(outBlock); // Send the read file to the socket
//        }

    }
    qDebug() << QString("Sending file %1").arg(currentPID);
    qDebug("%d", __LINE__);

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

bool MainServer::writeData(QByteArray data)
{
    QTcpSocket* socket = (QTcpSocket*)(sender());
    if(socket->state() == QAbstractSocket::ConnectedState)
    {
        //socket->write(IntToArray(data.size()));
        socket->write(data); // 데이터를 보내줌
        return socket->waitForBytesWritten();
    }
    else
    {
        return false;
    }
}

void MainServer::sendDataToClient(QString newData)
{
    
    
    QString sendData = newData;
    send_flag = writeData(sendData.toStdString().c_str()); //writeData의 첫 번째 인자는 char *data와 같은 형식임
    if(!send_flag)
        qDebug() << "Socket send fail\n";
    
    
    
    //    if(fd_flag)
    //    {
    //        QString sendData = newData;
    //        send_flag = writeData(sendData.toStdString().c_str()); //writeData의 첫 번째 인자는 char *data와 같은 형식임
    //        if(!send_flag)
    //            qDebug() << "Socket send fail\n";
    
    //    }
    
    
}

void MainServer::receiveData()
{
    QTcpSocket* socket = dynamic_cast<QTcpSocket*>(sender());


    
    QByteArray receiveData = socket->readAll();
    
    saveData = QString(receiveData);
    ui->textEdit->insertPlainText(saveData);
    ui->textEdit->insertPlainText("\n");
    
    qDebug() << "savedata: " << saveData;
    
    if(saveData.contains("<CR>", Qt::CaseInsensitive) == true)
    {
        
        //어떤 이벤트인지에 따라 불러올 함수 써주기(각각 이벤트에 대한 함수 만들고 if-else문 타도록 만들자)
        QString event = saveData.split("<CR>")[0];
        QString id = saveData.split("<CR>")[1];
        QString data = saveData.split("<CR>")[2];
        
        qDebug() << "이벤트: " << event;

        if(event == "CNT"){

            /*어떤 모듈과 연관이 있는 소켓인지 알 수 있도록 map에 연결해 저장하는 부분*/
            if(id == "PMS")
            {

                sk.insert(pmsSocket, "PMS");
                pmsSocket = socket;
                qDebug() << "pmsSocket ready";
                qDebug() << "pmsSocket save: " << saveData;

            }
            else if(id == "IMG")
            {
                sk.insert(imagingSocket, "IMG");
                imagingSocket = dynamic_cast<QTcpSocket*>(sender());
                qDebug() << "imagingSocket ready";
            }
            else if(id == "VEW")
            {
                sk.insert(viewerSocket, "VEW");
                viewerSocket = dynamic_cast<QTcpSocket*>(sender());
                qDebug() << "viewerSocket ready";
            }
        }
        else if(event == "VLG")
        {
            QString sendData = "VLG<CR>";

            qDebug()<<"Login Data: "<<id << ", " <<data;
            QModelIndexList indexes = dentistModel->match(dentistModel->index(0, 0), Qt::EditRole, id,
                                                          -1, Qt::MatchFlags(Qt::MatchCaseSensitive));
            qDebug() << indexes;
            QString name;
            foreach(auto ix, indexes) {
                /*검색창에서 입력한 값으로 찾은 데이터를 통해 고객 데이터들을 반환*/
                name = dentistModel->data(ix.siblingAtColumn(1)).toString();
            }
            qDebug() << name;
            if(name == data)
            {
                sendData = sendData +id + "<CR>True";

            }
            else
            {
                sendData = sendData +id + "<CR>False";
            }

            qDebug() << "로그인 sendData: " << sendData;
            viewerSocket->write(sendData.toStdString().c_str());


            //qDebug() << query->exec("select * from (select * from dentist where dentist_no = '" + id + "') where dentist_name = '" + data + "'");
            //qDebug() << dentistModel->rowCount();
        }
        /*환자관리 SW 이벤트*/
        else if(event == "PER")      //신규환자 등록 끝났을 때: PER(enroll)
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
            
            qDebug()<<"새로운 환자 정보 저장 완료";
            updateRecentData();
            //this->loadData();
            
        }
        else if(event == "PID")     //신규환자 PID 요청: PID
        {
            QString newPID = makeId();
            qDebug() << "newPID: " << newPID;
            QString sendData = "PID<CR>" + newPID + "<CR>";
            
            //이거 고치기 socket->write(sendData.toStdString().c_str());
            pmsSocket->write(sendData.toStdString().c_str());
            //sendDataToClient(sendData);
            
            //emit sendNewPID(newPID);
        }
        else if(event == "PDE")     //환자 정보 삭제: PDE(delete)
        {
            query->exec("delete from patient WHERE patient_no = '" + id + "'");
            patientModel->select();
        }
        else if(event == "PSE")     //검색: PSE(search)         //DB에 없는 환자 검색했을 때 죽는 거 예외처리 해야 함
        {
            qDebug() << "savedata: " << saveData;

            qDebug() << data;
            QString reportData ="<NEL>";
            query4->exec("select * from report WHERE patient_no = '"+data +"'");
            QSqlRecord reportRec =query4->record();
            qDebug()<<"Number of columns: "<<reportRec.count();
            qDebug() << "report value: " << query4->value(3);


            while(query4->next())
            {
                for(int i=0;i<reportRec.count();i++)
                {
                    //qDebug()<<"report i: "<<i <<"report data: "<<query4->value(i).toString();//output all names
                    QString tmpData = query4->value(i).toString()+"|";
                    reportData +=tmpData;
                    qDebug()<<"reportData : "<<reportData ;

                }
                query4->nextResult();
                reportData += "<NEL>";
            }



            QString sendData ="PSE<CR>";

            if(id == "0"){      //환자번호로 검색했을 때
                sendData = sendData + data + "<CR>";
                
                //시도중
                currentPID = data+".png";

                query->exec("select * from patient WHERE patient_no = '" + data + "'");
                QSqlRecord rec = query->record();
                qDebug() << "Number of columns: " << rec.count();
                
                while (query->next())
                {
                    for(int i=1; i<rec.count() ; i++)
                    {
                        qDebug() << "i: " << i << "data: " << query->value(i).toString(); // output all names
                        QString data = query->value(i).toString() + "|";
                        sendData += data;
                        qDebug() << "sendData: " << sendData;
                    }
                }


                QString reportData ="<NEL>";
                query4->exec("select * from report WHERE patient_no = '"+data +"'");
                QSqlRecord reportRec =query4->record();
                qDebug()<<"Number of columns: "<<reportRec.count();
                qDebug() << "report value: " << query4->value(3);

                QString dentistID, dentistName;
                while(query4->next())
                {
                    for(int i=0;i<reportRec.count();i++)
                    {
                        if(i==2)
                        {
                            dentistID = query4->value(i).toString();
                            qDebug()<<"doctorID: " << dentistID;
                        }
                        //qDebug()<<"report i: "<<i <<"report data: "<<query4->value(i).toString();//output all names
                        QString tmpData = query4->value(i).toString()+"|";
                        reportData +=tmpData;
                        qDebug()<<"reportData : "<<reportData ;

                    }

                    query2->exec("select * from dentist WHERE dentist_no = '"+ dentistID +"'");
                    while(query2->next())
                    {
                        dentistName = query2->value(1).toString();
                        qDebug() << "Dentist Name: " <<dentistName;
                    }


                    query4->nextResult();
                    reportData += dentistName + "<NEL>";
                }

                sendData += reportData;

            }
            else if(id == "1"){     //환자이름으로 검색했을 때
                QString pid;

                query->exec("select * from patient WHERE patient_name = '" + data + "'");
                QSqlRecord rec = query->record();
                qDebug() << "Number of columns: " << rec.count();
                

                while (query->next()){
                    for(int i = 0; i<rec.count() ; i++)
                    {
                        if(i == 0)
                        {
                            qDebug() << "i: " << i << "data: " << query->value(i).toString();
                            QString data = query->value(i).toString();
                            pid = data; //pid만 저장해줌

                            data += "<CR>"; //구분자 붙임
                            sendData += data;
                            qDebug() << "sendData: " << sendData;


                        }
                        else
                        {
                            qDebug() << "i: " << i << "data: " << query->value(i).toString();
                            QString data = query->value(i).toString() + "|";
                            sendData += data;
                            qDebug() << "sendData: " << sendData;
                        }
                    }
                }
                
                qDebug()<<"pid: "<<pid;
                currentPID = pid+".png";

                QString reportData ="<NEL>";
                query4->exec("select * from report WHERE patient_no = '"+ pid +"'");
                QSqlRecord reportRec =query4->record();
                qDebug()<<"Number of columns: "<<reportRec.count();
                qDebug() << "report value: " << query4->value(3);


                QString dentistID, dentistName;
                while(query4->next())
                {

                    for(int i=0;i<reportRec.count();i++)
                    {
                        if(i==2)
                        {
                            dentistID = query4->value(i).toString();
                            qDebug()<<"doctorID: " << dentistID;
                        }

                        //qDebug()<<"report i: "<<i <<"report data: "<<query4->value(i).toString();//output all names
                        QString tmpData = query4->value(i).toString()+"|";
                        reportData +=tmpData;
                        qDebug()<<"reportData : "<<reportData ;

                    }

                    query2->exec("select * from dentist WHERE dentist_no = '"+ dentistID +"'");
                    while(query2->next())
                    {
                        dentistName = query2->value(1).toString();
                        qDebug() << "Dentist Name: " <<dentistName;
                    }

                    query4->nextResult();
                    reportData += dentistName + "<NEL>";
                }
                sendData += reportData;

            }
            
            // 이거 고치기 socket->write(sendData.toStdString().c_str());
            //            qDebug() << "PSE's Info Data: " << sendData;
            //            qDebug() << "PSE's Report List: " << reportData;

            qDebug() << "PSE's sendData: " << sendData;
            pmsSocket->write(sendData.toStdString().c_str());


            sendFileFlag = 0;
            sendFile();

            //this->loadData();
        }
        else if(event == "PMO")     //환자 정보 수정: PMO(modify)
        {
            qDebug() << "PMO's saveData: " << saveData;

            QString name = data.split("|")[0];
            QString sex = data.split("|")[1];
            QString birthdate = data.split("|")[2];
            QString tel = data.split("|")[3];
            QString address = data.split("|")[4];
            QString memo = data.split("|")[5];


            query->exec("update patient set patient_name = '" + name + "', patient_sex = '" + sex +
                        "', patient_birthdate = '" + birthdate + "', patient_tel = '" + tel +
                        "', patient_address = '" + address + "', patient_memo = '" + memo +
                        "'where patient_no = '" + id + "'");

            patientModel->select();

        }
        else if(event == "PFN")     //수납 처리: PFN(finish)           @@@@@@@@@뷰어와 확인필요@@@@@@@@
        {
            qDebug() << "PFN's saveData: " << saveData;
            pmsSocket->write(saveData.toStdString().c_str());
        }
        else if(event == "AWL")     //대기 환자 추가: AWL(Add to Wait list)   //환자정보에서는 내부적으로 대기목록에 추가됨. 뷰어로만 보내주면 될 듯(pid랑 이름만).
            //=> 환자SW에서 대기 버튼 눌렀다는 정보를 서버에서 받고 해당 환자에 대한 여러가지 정보들을 촬영/뷰어SW로 보냄
            //위에 있는 기능은 진료시작/촬영시작에 해당하는 기능인 걸로 결정했음. 여기서는 pid랑 이름만 주기로. 딱 대기목록에 추가될 정보만 보내자
        {
            qDebug()<< "받은 진료대기환자 정보: " << saveData;
            qDebug() << "뷰어쪽으로 보내줄 진료대기환자 정보(촬영SW에는 안 보내줌): " + event + "<CR>" + id + "<CR>" + data.split("|")[0];
            //socket->write(saveData.toStdString().c_str());  //이 정보는 촬영SW와 뷰어SW쪽으로 보내져야 함.(지금 써져있는 socket은 임시..
            QString sendWaitData = event + "<CR>" + id + "<CR>" + data.split("|")[0];
            qDebug("%d", __LINE__);
            //이거 고치기 socket->write(sendWaitData.toStdString().c_str());


            //**********여기는 정연이 뷰어SW가 켜져있을 때 다시 주석 풀기************
            //qDebug() << "정연이 소켓 있는지 확인: " << viewerSocket->isValid();
            viewerSocket->write(sendWaitData.toStdString().c_str());


        }

        /*촬영 SW 이벤트*/
        else if(event == "IPR")     //환자 준비: IPR(patient ready) [받는 정보: 이벤트, ID / 보낼 정보: 이벤트, ID, 이름, 생년월일, 성별]
        {
            qDebug("%d", __LINE__);
            QString sendData ="IPR<CR>";
            sendData = sendData + id + "<CR>";
            qDebug("%d", __LINE__);

            query->exec("select * from patient where patient_no = '" + id + "'");
            qDebug("%d", __LINE__);
            QSqlRecord rec = query->record();
            qDebug("%d", __LINE__);
            qDebug() << "Number of columns: " << rec.count();

            //와일문보기
            while (query->next()){
                qDebug("%d", __LINE__);
                for(int i = 1; i<4 ; i++)
                {
                    qDebug("%d", __LINE__);
                    //                    if(i == 0)
                    //                    {
                    //                        qDebug("%d", __LINE__);
                    //                        qDebug() << "i: " << i << "data: " << query->value(i).toString();
                    //                        QString data = query->value(i).toString() + "<CR>";
                    //                        sendData += data;
                    //                        qDebug() << "sendData: " << sendData;
                    //                        qDebug("%d", __LINE__);
                    //                    }
                    //                    else
                    //                    {
                    qDebug("%d", __LINE__);
                    qDebug() << "i: " << i << "data: " << query->value(i).toString();
                    QString data = query->value(i).toString() + "|";
                    sendData += data;
                    qDebug() << "sendData: " << sendData;
                    qDebug("%d", __LINE__);
                    //                    }
                }


                //                qDebug() << query->value(0).toString(); // output all names
                //                QString data = query->value(0).toString() + "<CR>";
                //                sendData += data;
                //                qDebug() << "sendData: " << sendData;
            }
            //qDebug() << sendData << "sfdffsdsf";




            //@@@@@@@@이부분 미로오빠꺼 열리면 주석풀기@@@@@@@@@
            imagingSocket->write(sendData.toStdString().c_str());
            //QString sendReadyData = event + "<CR>" + id + "<CR>" + name + birthdate + sex ;


        }
        //파일소켓으로는 자동으로 이미지가 전송되고 받아지고 할 거고 ISV는 파일이 서버로 보내졌다는 사실만을 알려주는 이벤트임
        else if(event == "ISV")     //저장 및 전송: ISV(save) [받을 정보, 보낼 정보 동일: 이미지 No, 환자 ID, 이름, 촬영 타입] - imaging module에서 클릭될 시에 다른 모듈에서는 진료대기로 바뀜
        {
            qDebug() << "ISV's saveData: " << saveData;
            pmsSocket->write(saveData.toStdString().c_str());
            viewerSocket->write(saveData.toStdString().c_str());

        }


        /*영상 뷰어 SW 이벤트*/
        else if(event == "VNT")     //처방전 작성: VNT (write note)
            //받을 정보: VNT<CR>PID<CR>환자이름|의사번호|의사이름|진료날짜|진료내용(처방내용)
            //보낼 정보: VNT<CR>PID<CR>진료차트 번호(이거는 내가 계산)|환자이름|의사번호|의사이름|진료날짜|진료내용(처방내용)
        {
            qDebug()<< saveData;
            //QString rpReportNo = makeReportNo();
            //QString rpPID = id;
            //QString rpPName = data.split("|")[0];
            //QString rpDID = data.split("|")[1];
            //QString rpDName = data.split("|")[2];
            //QString rpDate = data.split("|")[3];
            //QString rpNote = data.split("|")[4];

            query4->prepare("INSERT INTO report (report_no, patient_no, dentist_no, report_date, report_note)"
                            "VALUES(:report_no, :patient_no, :dentist_no, :report_date, :report_note)");

            query4->bindValue(":report_no", makeReportNo());    //오류있는듯 P00001 대신 R00001대야함
            query4->bindValue(":patient_no", data.split("|")[0]);
            query4->bindValue(":dentist_no", data.split("|")[2]);
            query4->bindValue(":report_date", data.split("|")[3]);
            query4->bindValue(":report_note", data.split("|")[4]);
            query4->exec();

            qDebug()<<"새로운 진료기록 정보 저장 완료";

            reportModel->select();
            pmsSocket->write(saveData.toStdString().c_str());



        }
        else if(event == "VTS")     //진료 시작: VTS(treatment start)
            //[받을 정보: 이벤트, pid / 보낼 정보: 이벤트, pid, 이름, 성별, 생년월일, 메모]
        {

            QString sendData ="VTS<CR>";
            sendData = sendData + id + "<CR>";
            qDebug("%d", __LINE__);

            query->exec("select * from patient where patient_no = '" + id + "'");
            qDebug("%d", __LINE__);
            QSqlRecord rec = query->record();
            qDebug("%d", __LINE__);
            qDebug() << "Number of columns: " << rec.count();

            //와일문보기
            while (query->next()){
                qDebug("%d", __LINE__);
                for(int i = 1; i<4 ; i++)
                {
                    qDebug() << "i: " << i << "data: " << query->value(i).toString();
                    QString data = query->value(i).toString() + "|";
                    sendData += data;
                    qDebug() << "sendData: " << sendData;
                    qDebug("%d", __LINE__);
                    //
                }
                sendData += query->value(6).toString();

                //                qDebug() << query->value(0).toString(); // output all names
                //                QString data = query->value(0).toString() + "<CR>";
                //                sendData += data;
                //                qDebug() << "sendData: " << sendData;
            }
            //qDebug() << sendData << "sfdffsdsf";
            qDebug() << "sendData: " << sendData;
            viewerSocket->write(sendData.toStdString().c_str());
            sendFileFlag = 1;
            sendFile();

            pmsSocket->write(sendData.toStdString().c_str());

        }
        else if(event == "VTF")     //진료 완료: VTF(treatment finish) [받을 정보: 이벤트, pid / 보낼 정보: 이벤트, pid]
        {
            saveData = saveData + "|";  //pms의 statusRequestSended함수에서 name에 해당하는 부분을 |로 나누어주기 때문에 필요한 부분
            pmsSocket->write(saveData.toStdString().c_str()); //뷰어쪽에서 받은 정보 그대로 환자관리SW에 전송=>환자관리에서는 event가 VTS일 시에 환자 진료 상태 진료중으로 변경해주면 될 듯
        }

        /*촬영 요청 이벤트(환자SW/뷰어SW->촬영SW)*/
        else if(event == "SRQ")     //촬영 의뢰: SRQ(shoot request)
        {
            qDebug() << "saveData: " << saveData;

            //미로오빠 소켓 주석
            imagingSocket->write(saveData.toStdString().c_str());

            //정연이 소켓 주석/촬영요청이 pms에서 오든 viewer에서 오든 둘 다 촬영중으로 바뀌었다는 신호를 받아야 하기 때문에 SRQ이벤트를 서버쪽에서 다시 보내주도록 하였음
            pmsSocket->write(saveData.toStdString().c_str());
            viewerSocket->write(saveData.toStdString().c_str());
        }

        //buffer->clear(); //버퍼 비워주기
    }
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
        
        query2 = new QSqlQuery(db);
        query2->exec("CREATE TABLE IF NOT EXISTS dentist(dentist_no VARCHAR(10) Primary Key,"
                     "dentist_name VARCHAR(10) NOT NULL, dentist_sex VARCHAR(5) NOT NULL, dentist_tel VARCHAR(15) NOT NULL);");
        
        //        query2->exec("CREATE TABLE IF NOT EXISTS dentist(dentist_no VARCHAR(10) Primary Key,"
        //                    "dentist_name VARCHAR(10) NOT NULL, dentist_sex VARCHAR(5) NOT NULL), dentist_tel VARCHAR(15) NOT NULL);");
        dentistModel = new QSqlTableModel(this, db);
        dentistModel->setTable("dentist");
        dentistModel->select();
        dentistModel->setHeaderData(0, Qt::Horizontal, tr("No"));
        dentistModel->setHeaderData(1, Qt::Horizontal, tr("Name"));
        dentistModel->setHeaderData(2, Qt::Horizontal, tr("Sex"));
        dentistModel->setHeaderData(3, Qt::Horizontal, tr("Telephone Number"));
        ui->dentistTableView->setModel(dentistModel);
        //의사 정보는 수정삭제 불가능하게 만들어놨음. 고정된 정보
        //query2->exec("INSERT INTO dentist VALUES ('D00001', '이정연', '여성', '010-1234-5678')");
        query2->exec("INSERT INTO dentist VALUES ('D00002', '안다미로', '남성', '010-8765-4321')");
        query2->exec("INSERT INTO dentist VALUES ('D00003', '박병규', '남성', '010-3456-7890')");

        
        
        query3= new QSqlQuery(db);
        query3->exec("CREATE TABLE IF NOT EXISTS image(image_no VARCHAR(10) Primary Key, patient_no VARCHAR(10) NOT NULL,"
                     "dentist_no VARCHAR(10) NOT NULL, modality VARCHAR(10) NOT NULL, bodypart_examined VARCHAR(30) NOT NULL,"
                     "image_date VARCHAR(15) NOT NULL, image_path varchar(300) NOT NULL);");
        imageModel = new QSqlTableModel(this, db);
        imageModel->setTable("image");
        imageModel->select();
        imageModel->setHeaderData(0, Qt::Horizontal, tr("Image No"));
        imageModel->setHeaderData(1, Qt::Horizontal, tr("Patient No"));
        imageModel->setHeaderData(2, Qt::Horizontal, tr("Dentist No"));
        imageModel->setHeaderData(3, Qt::Horizontal, tr("Modality"));
        imageModel->setHeaderData(4, Qt::Horizontal, tr("Body Part"));
        imageModel->setHeaderData(5, Qt::Horizontal, tr("Image Date"));
        imageModel->setHeaderData(6, Qt::Horizontal, tr("Image Path"));
        ui->imageTableView->setModel(imageModel);
        
        query4= new QSqlQuery(db);
        query4->exec("CREATE TABLE IF NOT EXISTS report(report_no VARCHAR(10) Primary Key, patient_no VARCHAR(10) NOT NULL,"
                     "dentist_no VARCHAR(10) NOT NULL, report_date VARCHAR(15) NOT NULL, report_note VARCHAR(500) NOT NULL);");
        reportModel = new QSqlTableModel(this, db);
        reportModel->setTable("report");
        reportModel->select();
        reportModel->setHeaderData(0, Qt::Horizontal, tr("Report No"));
        reportModel->setHeaderData(1, Qt::Horizontal, tr("Patient No"));
        reportModel->setHeaderData(2, Qt::Horizontal, tr("Dentist No"));
        reportModel->setHeaderData(3, Qt::Horizontal, tr("Report Date"));
        reportModel->setHeaderData(4, Qt::Horizontal, tr("Report Note"));
        ui->reportTableView->setModel(reportModel);

        /*임시로 데이터 넣어둔 것. 나중에 지워도 무관*/
        query4->exec("INSERT INTO report VALUES ('R00001', 'P00001', 'D00002', '2023-01-19', '19일 처방전')");
        query4->exec("INSERT INTO report VALUES ('R00002', 'P00001', 'D00002', '2023-01-20', '20일 처방전')");
        query4->exec("INSERT INTO report VALUES ('R00003', 'P00002', 'D00003', '2023-01-21', '21일 처방전')");
        reportModel->select();



        
        query5= new QSqlQuery(db);
        query5->exec("CREATE TABLE IF NOT EXISTS image_relation(report_no VARCHAR(10), image_no VARCHAR(10), CONSTRAINT relation Primary Key(report_no, image_no));");
        imageRelationModel = new QSqlTableModel(this, db);
        imageRelationModel->setTable("image_relation");
        imageRelationModel->select();
        imageRelationModel->setHeaderData(0, Qt::Horizontal, tr("Report No"));
        imageRelationModel->setHeaderData(1, Qt::Horizontal, tr("Image No"));
        ui->imageRelationTableView->setModel(imageRelationModel);
        
        
        
        
        
        
        
        
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
    
    qDebug()<< "rowCount: " << patientModel->rowCount();
    
    if(patientModel->rowCount() == 0) {
        id = 1;
        qDebug()<< "it will return rowCount: 1";
        return "P" + QString::number(id).rightJustified(5,'0');
    } else {
        //auto id = patientModel->data(patientModel->index(patientModel->rowCount()-1, 0)).toInt();
        //qDebug() << "now" << patientModel->data(patientModel->index(patientModel->rowCount()-1, 0)).toInt();
        //id = patientModel->rowCount();
        //qDebug() << "last: " << query->
        //id++;
        
        int tempPid = patientModel->itemData(patientModel->index(patientModel->rowCount() - 1,0)).value(0).toString().right(5).toInt()+1; //마지막 row의 pid+1 값을 리턴
        qDebug() << "id is not 1, id: " << tempPid;
        qDebug()<< "it will return rowCount: " << "P" + QString::number(tempPid).rightJustified(5,'0');
        
        //        qDebug() << "1: " << patientModel->itemData(patientModel->index(patientModel->rowCount() - 1,0));
        //        qDebug() << "2: " << patientModel->itemData(patientModel->index(patientModel->rowCount() - 1,0)).value(0);
        //        qDebug() << "3: " << patientModel->itemData(patientModel->index(patientModel->rowCount() - 1,0)).value(0).toString(); //"P00001"
        //        qDebug() << "4: " <<  patientModel->itemData(patientModel->index(patientModel->rowCount() - 1,0)).value(0).toString().right(5).toInt();
        
        //return "P" + QString::number(id).rightJustified(5,'0');
        return "P" + QString::number(tempPid).rightJustified(5,'0');
    }
}

QString MainServer::makeReportNo()
{
    int id;

    qDebug()<< "reportModel rowCount: " << reportModel->rowCount();

    if(reportModel->rowCount() == 0) {
        id = 1;
        qDebug()<< "it will return report rowCount: 1";
        return "R" + QString::number(id).rightJustified(5,'0');
    } else {
        int tempReportNo= reportModel->itemData(reportModel->index(reportModel->rowCount() - 1,0)).value(0).toString().right(5).toInt()+1; //마지막 row의 pid+1 값을 리턴
        qDebug() << "report no is not 1, no: " << tempReportNo;
        qDebug()<< "it will return report rowCount: " << "R" + QString::number(tempReportNo).rightJustified(5,'0');

        return "R" + QString::number(tempReportNo).rightJustified(5,'0');
    }
}

//그냥 함수 지우고 저 select문만 다시 써주면 더 나을 듯. 나중에 기능구현 다 하구 지울 것!
void MainServer::updateRecentData()
{
    patientModel->select();
}
