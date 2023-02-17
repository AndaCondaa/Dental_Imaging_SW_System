/*
 * 프로그램명 : MainServer
 * 파일명 : mainserver.cpp
 * 설명 : PMS SW, 이미지 뷰어 SW, 촬영 SW와 연결되어 데이터를 주고받는 서버
 * 작성자 : 김유선
 * 최종 수정일 : 2023.02.16
 */


#include "mainserver.h"
#include "ui_mainserver.h"
#include <QTcpServer>
#include <QTcpSocket>
#include <QSqlQuery>
#include <QSqlDatabase>
#include <QTableWidget>
#include <QSqlRecord>
#include <QTcpSocket>
#include <QTextStream>

// mvc패턴 적용시키기 위해 정보 변경&삭제시 뷰어와 이미징 모듈에 패킷 보내줄 것 ex. ACK^PMO<CR>P00001<CR>NULL

MainServer::MainServer(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainServer)
{
    ui->setupUi(this);

    // 데이터 서버 생성: 클라이언트들로부터 받는 데이터 소켓들과 연결
    server = new QTcpServer(this);
    connect(server, SIGNAL(newConnection()), this, SLOT(newConnection()));
    QString socket_data = QString("MainServer Listening: %1\n").arg(server->listen(QHostAddress::Any, 8000) ? "true" : "false");
    ui->textEdit->append(socket_data);

    // 파일 서버 생성: 클라이언트들로부터 받는 파일 소켓들과 연결
    fileServer = new QTcpServer(this);
    QString fileSocket_data = QString("FileServer Listening: %1\n").arg(fileServer->listen(QHostAddress::Any, 8001) ? "true" : "false");
    connect(fileServer, SIGNAL(newConnection()), this, SLOT(newFileConnection()));
    ui->textEdit->append(fileSocket_data);

    // DB 로드
    this->loadData();

}

MainServer::~MainServer()
{
    delete ui;
}

// 새로운 데이터 소켓이 연결될 때
void MainServer::newConnection()
{
    QTcpSocket* socket= server->nextPendingConnection();

    connect(socket, SIGNAL(readyRead()), this, SLOT(receiveData()));
    connect(socket, SIGNAL(disconnected()), this, SLOT(socketDisconnected()));

     qDebug("new socket connection");
}

// 새로운 파일 소켓이 연결될 때
void MainServer::newFileConnection()
{
    QTcpSocket* fileSocket = fileServer->nextPendingConnection();           // receivedSocket에 fileServer에서 저장해두었던 다음 보류중인 연결을 연결해준다
    connect(fileSocket, SIGNAL(readyRead()), this, SLOT(receiveFile()));     // 받은 소켓에서 정보를 읽어 serverform에서 파일 전송이 가능하도록 만듦
    connect(fileSocket, SIGNAL(disconnected()), this, SLOT(fileSocketDisconnected()));

    qDebug("new file socket connection");
}


// 모듈과의 연결이 끊어질 때 호출되는 함수. 연결이 끊어지면 해당 파일소켓을 nullptr로 설정해준다
void MainServer::fileSocketDisconnected()
{

    QTcpSocket *socket = dynamic_cast<QTcpSocket*>(sender());
    if(socket == viewerFileSocket)
        viewerFileSocket=nullptr;
    else if(socket == pmsFileSocket)
        pmsFileSocket=nullptr;
    else if(socket == imagingFileSocket)
        imagingFileSocket=nullptr;

}


// 파일 소켓들과 관련있는 함수
void MainServer::receiveFile()
{
    // 이미지 파일명 예시: type_date.bmp

    // 파일 소켓이 처음 연결될 때
    // ex.CNT<CR>IMG<CR>NULL
    QTcpSocket *socket = dynamic_cast<QTcpSocket*>(sender());


    if (pmsFileSocket != socket && imagingFileSocket != socket && viewerFileSocket != socket) {
        QByteArray arr = socket->readAll();
        QString id = QString(arr).split("<CR>")[1];
        if (id == "IMG") {
            qDebug("%d: RECEIVED IMG SOCKET", __LINE__);
            imagingFileSocket = socket;
        }
        else if (id == "PMS") {
            qDebug("%d: RECEIVED PMS SOCKET", __LINE__);
            pmsFileSocket = socket;
        } else if(id == "VEW") {
            qDebug("%d: RECEIVED VIEWER SOCKET", __LINE__);
            viewerFileSocket =socket;
        }
        return;
    }

    if (byteReceived == 0) {                // 파일 받기 시작
        QDataStream in(imagingFileSocket);
        in.device()->seek(0);
        in >> totalSize >> byteReceived >> currentPID >> type;

        QDir dir(QString("./Image/%1").arg(currentPID));
        if (!dir.exists())
            dir.mkpath(".");

        fileName = type + "_" + QDate::currentDate().toString("yyyyMMdd") + ".jpg";
        currentFileName = dir.path() + "/" + fileName;
        file = new QFile(currentFileName);
        file->open(QFile::WriteOnly);

    } else {                                // 파일 받는 중
        inBlock = imagingFileSocket->readAll();

        byteReceived += inBlock.size();
        file->write(inBlock);
        file->flush();
    }

    if (byteReceived == totalSize) {        // 파일 receive 완료
        qDebug() << QString("%1 receive completed").arg(fileName);
        inBlock.clear();
        byteReceived = 0;
        totalSize = 0;
        file->close();
        delete file;


        // DB에 이미지 정보 저장
        QString newIID = makeImageNo();

        query3->prepare("INSERT INTO image (image_no, patient_no, type, image_date, image_path)"
                        "VALUES(:image_no, :patient_no, :type, :image_date, :image_path)");

        query3->bindValue(":image_no", newIID);
        query3->bindValue(":patient_no", currentPID);
        query3->bindValue(":type", type);
        query3->bindValue(":image_date", QDate::currentDate().toString("yyyyMMdd"));
        query3->bindValue(":image_path", currentFileName);
        query3->exec();

        qDebug()<<"새로운 이미지 정보 저장 완료";
        imageModel->select();
    }
}


// 파일을 PMS나 뷰어로 전송하는 함수
void MainServer::sendFile()
{
    loadSize = 0;
    byteToWrite = 0;
    totalSize = 0;
    outBlock.clear();

    QDir dir(QString("./Image/%1").arg(currentPID));
    QStringList filters;
    filters << "*.png" << "*.jpg" << "*.bmp";
    QFileInfoList fileInfoList = dir.entryInfoList(filters, QDir::Files | QDir::NoDotAndDotDot);
    qDebug() << "폴더 안의 전체 파일 개수-fileInfoList.count(): " << fileInfoList.count();


    // 보내는 형식: 파일개수<CR>파일명<f>파일크기<n>파일명<f>파일크기<n>|파일정보1파일정보2...
    QByteArray allFile;
    allFile.append(QString::number(fileInfoList.count()).toStdString());
    allFile.append("<CR>");

    // 폴더 안에 아무것도 없으면 파일개수에 0만 적어서 보냄
    if(fileInfoList.count() == 0)
    {
        if(sendFileFlag == 0)
            pmsFileSocket->write(allFile);
        else if(sendFileFlag == 1)
            viewerFileSocket->write(allFile);
    }


    QByteArray tempArray;

    for(int i = 0; i < fileInfoList.count(); i++)
    {
        QString currentFileName = dir.path() + "/" + fileInfoList.at(i).fileName();
        qDebug() << currentFileName;

        file = new QFile(currentFileName);
        file->open(QFile::ReadOnly);

        // 파일명과 파일크기 전송
        QString fileString = fileInfoList.at(i).fileName() + "<f>"
                + QString::number(file->size()) + "<n>";

        allFile.append(fileString.toStdString());

        tempArray.append(file->readAll());
        qDebug() << "tempSize " << file->size();
    }
    allFile.append("|");
    allFile.append(tempArray);  // 파일정보 전송
    allFile.append("<FIN>");

    qDebug() << "allFile.size()" << allFile.size();


    if(sendFileFlag == 0)
        pmsFileSocket->write(allFile);
    else if(sendFileFlag == 1)
        viewerFileSocket->write(allFile);

}



// 모듈과의 연결이 끊어질 때 호출되는 함수. 연결이 끊어지면 해당 소켓을 nullptr로 설정해준다
void MainServer::socketDisconnected()
{

    QTcpSocket *socket = dynamic_cast<QTcpSocket*>(sender());
    if(socket == viewerSocket)
        viewerSocket=nullptr;
    else if(socket == pmsSocket)
        pmsSocket=nullptr;
    else if(socket == imagingSocket)
        imagingSocket=nullptr;

}

bool MainServer::writeData(QByteArray data)
{
    QTcpSocket* socket = (QTcpSocket*)(sender());
    if(socket->state() == QAbstractSocket::ConnectedState)
    {
        socket->write(data); // 데이터를 보내줌
        return socket->waitForBytesWritten();
    }
    else
    {
        return false;
    }
}

// 소켓에서 데이터를 받은 후 해당 이벤트에 대한 기능을 수행하는 부분
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
        // 어떤 이벤트인지 확인.
        QString event = saveData.split("<CR>")[0];

        // 클라이언트로부터 오는 헤더는 SEN
        QString header = event.split("^")[0];

        if(saveData.contains(header, Qt::CaseInsensitive) == true)
            event = event.split("^")[1];    // 헤더를 제외한 이벤트만 잘라냄
        else        // 클라이언트로부터 오는 헤더는 SEN으로 통일하였으므로, SEN이 포함되어있지 않으면 return 한다
            return;

        // id는 일부 예외사항을 제외하고는 ID가 저장되는 부분
        QString id = saveData.split("<CR>")[1];

        // data는 특정 이벤트에 대한 data가 저장되는 부분
        QString data = saveData.split("<CR>")[2];

        if(event == "CNT"){         // 모듈 연결 기능
            /*어떤 모듈과 연관이 있는 소켓인지 알 수 있도록 map에 연결해 저장하는 부분*/
            if(id == "PMS")         // 환자관리SW 연결
            {
                sk.insert(pmsSocket, "PMS");
                pmsSocket = socket;
                qDebug() << "pmsSocket ready";
                qDebug() << "pmsSocket save: " << saveData;

                // 연결이 되면 대기목록을 보냄
                sendWaitingList(pmsSocket);
            }
            else if(id == "IMG")    // 촬영SW 연결
            {
                sk.insert(imagingSocket, "IMG");
                imagingSocket = dynamic_cast<QTcpSocket*>(sender());
                qDebug() << "imagingSocket ready";

                // 연결이 되면 대기목록을 보냄
                sendWaitingList(imagingSocket);

                // 뷰어가 연결되어 있었던 상태라면, 뷰어는 촬영SW가 커넥트 될 때마다 촬영요청을 수행하기 위해 촬영SW가 연결되었다는 메세지를 받아야 함
                if(viewerSocket!=nullptr)
                {
                    QString tempData = saveData.split("^")[1];
                    QString sendData = "ACK^" + tempData;
                    viewerSocket->write(sendData.toStdString().c_str());
                }
            }
            else if(id == "VEW")    // 뷰어SW 연결
            {
                sk.insert(viewerSocket, "VEW");
                viewerSocket = dynamic_cast<QTcpSocket*>(sender());
                qDebug() << "viewerSocket ready";

                // 연결이 되면 대기목록을 보냄
                sendWaitingList(viewerSocket);
            }

        }
        else if(event == "VLG") // 뷰어의 의사 로그인 기능
        {

            qDebug()<<"Login Data: "<<id << ", " <<data;

            // DB 내의 의사 테이블 안에 전달받은 의사id를 검색해 몇 번째 줄에 있는지를 저장
            QModelIndexList indexes = dentistModel->match(dentistModel->index(0, 0), Qt::EditRole, id,
                                                          -1, Qt::MatchFlags(Qt::MatchCaseSensitive));
            QString name;
            foreach(auto ix, indexes) {
                /*의사 id를 통해 찾은 의사의 이름을 변수에 저장*/
                name = dentistModel->data(ix.siblingAtColumn(1)).toString();
            }


            QString sendData;

            if(name == data)        // DB에서 찾은 이름이 뷰어의 이름과 같으면 True 값을 전달해 로그인 성공을 알림
            {
                sendData = "ACK^VLG<CR>";
                sendData = sendData +id + "<CR>True";
            }
            else                    // DB에서 찾은 이름이 뷰어의 이름과 다르면 False 값을 전달해 로그인 실패를 알림
            {
                sendData = "ACK^VLG<CR>";
                sendData = sendData +id + "<CR>False";
            }

            qDebug() << "Send Login Data: " << sendData;
            viewerSocket->write(sendData.toStdString().c_str());

        }
        /*환자관리 SW 이벤트*/
        else if(event == "PER")      // 신규환자 등록 끝났을 때: PER(enroll)
        {
            // DB에 환자정보 추가
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
            patientModel->select();

            // 저장 잘 되었다고 pms에 알려주기
            QString tempData = saveData.split("^")[1];
            QString sendData = "ACK^" + tempData;
            pmsSocket->write(sendData.toStdString().c_str());

        }
        else if(event == "PID")     // 신규환자 PID 요청: PID
        {
            QString newPID = makeId();
            qDebug() << "newPID: " << newPID;
            QString sendData = "ACK^PID<CR>" + newPID + "<CR>";

            // 이거 고치기 socket->write(sendData.toStdString().c_str());
            pmsSocket->write(sendData.toStdString().c_str());


        }
        else if(event == "PDE")     // 환자 정보 삭제: PDE(delete)
        {
            // 이미지 폴더의 pid 폴더 삭제
            QDir dir(QString("./Image/%1").arg(id));
            dir.removeRecursively();

            // 환자 정보를 DB의 patient 테이블에서 삭제
            query->exec("delete from patient WHERE patient_no = '" + id + "'");
            patientModel->select();

            // image 테이블에서 지우려는 환자 이미지 정보도 함께 삭제
            query3->exec("select * from image WHERE patient_no = '" + id +"'");
            while(query3->next())
                query3->exec("delete from image WHERE patient_no = '" + id + "'");
            imageModel->select();

            // 잘 삭제되었다고 pms에 알려주기
            QString tempData = saveData.split("^")[1];
            QString sendData = "ACK^" + tempData;
            pmsSocket->write(sendData.toStdString().c_str());

        }
        else if(event == "PSE")     // 검색: PSE(search), id가 0이면 환자번호로 검색, 1이면 환자 이름으로 검색
        {
            QString sendData ="ACK^PSE<CR>";

            if(id == "0"){      // 환자번호로 검색했을 때
                sendData = sendData + data + "<CR>";

                currentPID = data;

                // DB의 patient 테이블에서 환자번호를 검색
                query->exec("select * from patient WHERE patient_no = '" + data + "'");
                QSqlRecord rec = query->record();
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


                // 환자에 해당하는 진료기록 데이터도 함께 보냄
                QString reportData ="<NEL>";
                query4->exec("select * from report WHERE patient_no = '"+data +"'");
                QSqlRecord reportRec =query4->record();
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
                        QString tmpData = query4->value(i).toString()+"|";
                        reportData +=tmpData;

                    }

                    // dentist number에 해당하는 의사 이름을 DB의 dentist 테이블에서 찾아 함께 보내줌
                    query2->exec("select * from dentist WHERE dentist_no = '"+ dentistID +"'");
                    while(query2->next())
                    {
                        dentistName = query2->value(1).toString();
                    }

                    query4->nextResult();
                    reportData += dentistName + "<NEL>";    //  환자 진료기록이 여러 개일 때는 <NEL>로 환자 기록을 구분해줌
                }

                sendData += reportData;

            }
            else if(id == "1"){     //  환자이름으로 검색했을 때
                QString pid;

                //  검색했는데 나온 값이 없으면 id값에 NULL을 입력해 전송
                query->exec("select * from patient WHERE patient_name = '" + data + "'");
                if(query->first()==false)
                    sendData+="NULL<CR>";

                //  검색 결과가 존재하면 동명이인이 몇 명인지 검색
                query->exec("select * from patient WHERE patient_name = '" + data + "'");
                int tempcount =0;
                while (query->next()){
                    tempcount ++;
                }

                //  2명 이상이면(동명이인이면) 동명이인 정보들을 한번에 묶어 전송함
                if(tempcount>1)
                {
                    query->exec("select * from patient WHERE patient_name = '" + data + "'");
                    QString tempSendData = "ACK^PSE<CR>" + QString::number(tempcount) + "<CR>";

                    //  이 세 명에 대한 정보들 쭉 모아서 보내줌 ex. PSE<CR>검색된 환자수<CR>PID|이름|전화번호(인덱스4번)<r>PID|이름|전화번호(인덱스4번)<r>.....
                    while (query->next()){//  검색된 환자 수만큼 검색
                        QString tempPID = query->value(0).toString();
                        QString tempPhoneNum = query->value(4).toString();

                        tempSendData += tempPID + "|" + data + "|" + tempPhoneNum + "<r>";
                    }

                    qDebug() << "tempSendData: " << tempSendData;
                    pmsSocket->write(tempSendData.toStdString().c_str());

                    return;
                }


                //  검색 결과가 한 명이면(동명이인이 아니었다면) 테이블에서 해당 환자의 이름에 해당하는 정보를 찾아 전송
                query->exec("select * from patient WHERE patient_name = '" + data + "'");
                QSqlRecord rec = query->record();
                while (query->next()){
                    for(int i = 0; i<rec.count() ; i++)
                    {
                        if(i == 0)  //  pid 앞에는 <CR>이라는 구분자를 붙여줌
                        {
                            qDebug() << "i: " << i << "data: " << query->value(i).toString();

                            QString data = query->value(i).toString();
                            pid = data; //  pid만 저장해줌

                            data += "<CR>"; //  구분자 붙임
                            sendData += data;
                            qDebug() << "sendData: " << sendData;

                        }
                        else        //  환자 정보 구분을 위해 |라는 구분자를 붙여줌
                        {
                            QString data = query->value(i).toString() + "|";
                            sendData += data;

                        }
                    }
                }


                qDebug() << "sendData: " << sendData;

                currentPID = pid;

                //  진료기록 검색
                QString reportData ="<NEL>";
                query4->exec("select * from report WHERE patient_no = '"+ pid +"'");
                QSqlRecord reportRec =query4->record();

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

                        QString tmpData = query4->value(i).toString()+"|";
                        reportData +=tmpData;
                        qDebug()<<"reportData : "<<reportData ;

                    }
                    //  의사 이름 검색
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

            //  pms로 환자 id와 관련된 이미지 파일 전송
            sendFileFlag = 0;
            sendFile();

            //  pms 소켓에 데이터 전송
            qDebug() << "PSE's sendData: " << sendData;
            pmsSocket->write(sendData.toStdString().c_str());


        }
        else if(event == "PMO")     //  환자 정보 수정: PMO(modify)
        {
            qDebug() << "PMO's saveData: " << saveData;

            QString name = data.split("|")[0];
            QString sex = data.split("|")[1];
            QString birthdate = data.split("|")[2];
            QString tel = data.split("|")[3];
            QString address = data.split("|")[4];
            QString memo = data.split("|")[5];


            //  DB에서 환자 정보 수정
            query->exec("update patient set patient_name = '" + name + "', patient_sex = '" + sex +
                        "', patient_birthdate = '" + birthdate + "', patient_tel = '" + tel +
                        "', patient_address = '" + address + "', patient_memo = '" + memo +
                        "'where patient_no = '" + id + "'");

            patientModel->select();

            //  수정 잘 되었다고 pms에 알려주기
            QString tempData = saveData.split("^")[1];
            QString sendData = "ACK^" + tempData;
            pmsSocket->write(sendData.toStdString().c_str());
        }
        else if(event == "AWL")     //  대기 환자 추가: AWL(Add to Wait list)
        {
            QString sendWaitData = "ACK^" + event + "<CR>" + id + "<CR>" + data.split("|")[0];

            //  뷰어가 켜져있지 않을 때
            if(viewerSocket == nullptr)
            {
                QString sendData = "ERR^AWL<CR>" + id + "<CR>" + data.split("|")[0];
                pmsSocket->write(sendData.toStdString().c_str());

                //  에러 메세지를 보냈으므로 더 이상의 작업을 하지 않음
                return;
            }

            //  viewer 소켓에 대기환자가 추가되었음을 알리는 정보를 보내줌
            viewerSocket->write(sendWaitData.toStdString().c_str());
            //  pms소켓에 뷰어에 잘 추가되었다고 메세지를 보내줌
            pmsSocket->write(sendWaitData.toStdString().c_str());

            //  진료대기 환자 추가 시 대기리스트 파일에 저장
            QFile waitingList("waitingList.txt");
            if (!waitingList.open(QIODevice::WriteOnly | QIODevice::Append))
                return;
            QTextStream out(&waitingList);
            out << id << "," <<"WT" << "\n";
            waitingList.close( );
        }

        /*촬영 SW 이벤트*/
        else if(event == "IPR")     //  환자 준비: IPR(patient ready) [받는 정보: 이벤트, ID / 보낼 정보: 이벤트, ID, 이름, 생년월일, 성별]
        {
            QString sendData ="ACK^IPR<CR>";
            sendData = sendData + id + "<CR>";

            query->exec("select * from patient where patient_no = '" + id + "'");

            // 촬영SW로부터 받은 id값에 해당하는 환자정보를 보내줌
            while (query->next()){

                for(int i = 1; i<4 ; i++)
                {
                    QString data = query->value(i).toString() + "|";
                    sendData += data;
                    qDebug() << "sendData: " << sendData;
                }
            }

            // 촬영SW 소켓에 정보를 적어 전송
            socket->write(sendData.toStdString().c_str());

        }
        // 파일소켓으로는 이미지 파일을 send&receive하고, ISV는 파일이 서버로 보내졌다는 사실만을 알려주는 이벤트임
        else if(event == "ISV")     // 저장 및 전송: ISV(save) [받을 정보, 보낼 정보 동일: 이미지 No, 환자 ID, 이름, 촬영 타입] - imaging module에서 클릭될 시에 다른 모듈에서는 진료대기로 바뀜
        {
            QString tempData = saveData.split("^")[1];
            QString sendData = "ACK^" + tempData;

            // 촬영SW쪽에 메인서버에 잘 저장되었다고 ACK 보내기
            imagingSocket->write(sendData.toStdString().c_str());

            // 촬영중인 상태인 환자가 촬영완료 되었을 때 대기목록리스트 파일에서 해당 환자의 상태를 대기중으로 변경
            QFile oldList("waitingList.txt");
            oldList.open(QIODevice::Text | QIODevice::ReadOnly);
            QString dataText = oldList.readAll();

            QString tempStatus = data;
            if( tempStatus == "CEPH")
                tempStatus = "CE";
            else if(tempStatus=="PANO")
                tempStatus = "PA";
            else if(tempStatus=="BOTH")
                tempStatus = "BO";
            QString changeNeededText = id + "," + tempStatus;
            QString changeText = id + "," + "WT";

            QRegularExpression re(changeNeededText);
            QString replacementText(changeText);
            dataText.replace(re, replacementText);
            QFile newList("waitingList.txt");
            if(newList.open(QFile::WriteOnly | QFile::Truncate)) {
                QTextStream out(&newList);
                out << dataText;
            }
            newList.close();

            if(pmsSocket == nullptr || viewerSocket == nullptr)     // pms나 뷰어가 켜져있지 않을 때
            {
                // 서버가 죽지 않도록 예외처리
                return;
            }
            else                                                    // pms나 뷰어가 켜져있을 때
            {
                // 촬영이 끝났음을 pms와 뷰어에 알려주기
                pmsSocket->write(sendData.toStdString().c_str());
                viewerSocket->write(sendData.toStdString().c_str());
            }
        }
        else if(event == "MWL")     // 진료중이었던 환자 상태를 진료대기로 변경
        {
            QString tempData = saveData.split("^")[1];
            QString sendData = "ACK^" + tempData;

            // 진료중에서 진료대기로 환자상태를 변경
            QFile oldList("waitingList.txt");
            oldList.open(QIODevice::Text | QIODevice::ReadOnly);
            QString dataText = oldList.readAll();

            // 대기환자 리스트 파일의 환자 상태를 진료중에서 진료대기로 변경
            QString changeNeededText = id + "," + "TM";
            QString changeText = id + "," + "WT";
            QRegularExpression re(changeNeededText);
            QString replacementText(changeText);

            dataText.replace(re, replacementText);
            QFile newList("waitingList.txt");
            if(newList.open(QFile::WriteOnly | QFile::Truncate)) {
                QTextStream out(&newList);
                out << dataText;
            }
            newList.close();

            if(pmsSocket == nullptr)                                // pms연결되어있지 않으면 return
                return;         // 서버 죽지 않도록 예외처리
            else
                pmsSocket->write(sendData.toStdString().c_str());   // pms연결되어있으면 메세지 보내줌
        }


        /*영상 뷰어 SW 이벤트*/
        else if(event == "VNT")     // 처방전 작성: VNT (write note)
            // 받을 정보: SEN^VNT<CR>PID<CR>환자이름|의사번호|의사이름|진료날짜|진료내용(처방내용)
            // 보낼 정보: ACK^VNT<CR>PID<CR>진료차트 번호(이거는 내가 계산)|환자이름|의사번호|의사이름|진료날짜|진료내용(처방내용)
        {
            qDebug()<< saveData;

            query4->prepare("INSERT INTO report (report_no, patient_no, dentist_no, report_date, report_note)"
                            "VALUES(:report_no, :patient_no, :dentist_no, :report_date, :report_note)");

            query4->bindValue(":report_no", makeReportNo());    // 오류있는듯 P00001 대신 R00001대야함
            query4->bindValue(":patient_no", id);
            query4->bindValue(":dentist_no", data.split("|")[1]);
            query4->bindValue(":report_date", data.split("|")[3]);
            query4->bindValue(":report_note", data.split("|")[4]);
            query4->exec();
            qDebug()<<"새로운 진료기록 정보 저장 완료";
            reportModel->select();

            /*DB에 잘 저장되었다고 뷰어로 ACK보내주는 부분*/
            // save데이터에서 이벤트 부분부터 잘라내서 저장
            QString tempData = saveData.toStdString().c_str();
            tempData = tempData.split("^")[1];
            QString sendData = "ACK^" + tempData;
            viewerSocket->write(sendData.toStdString().c_str());

            // pms가 켜져있으면
            if(pmsSocket !=nullptr)
            {
                pmsSocket->write(sendData.toStdString().c_str());   // 새로운 진료기록 전송
            }
            else    // 꺼져있으면 서버 죽지 않도록 만듦
                return;
        }
        else if(event == "VTS")     // 진료 시작: VTS(treatment start)
            // [받을 정보: 이벤트, pid / 보낼 정보: 이벤트, pid, 이름, 성별, 생년월일, 메모]
        {
            QString sendData ="ACK^VTS<CR>";
            sendData = sendData + id + "<CR>";

            currentPID = id;

            // 뷰어에서 진료시작 버튼을 눌렀을 때 서버에서 받는 pid를 검색해 필요 정보들을 뷰어로 재전송해줌
            query->exec("select * from patient where patient_no = '" + id + "'");
            while (query->next()){

                for(int i = 1; i<4 ; i++)
                {
                    QString data = query->value(i).toString() + "|";
                    sendData += data;
                }
                sendData += query->value(6).toString();
            }

            qDebug() << "sendData: " << sendData;

            viewerSocket->write(sendData.toStdString().c_str());

            // 뷰어쪽으로 진료시작한 환자에 해당하는 이미지 파일을 보내줌
            sendFileFlag = 1;
            sendFile();

            // 진료가 시작되었을 때, 대기환자 리스트 파일에서 환자의 상태를 진료대기 상태에서 진료중으로 변경
            QFile oldList("waitingList.txt");
            oldList.open(QIODevice::Text | QIODevice::ReadOnly);
            QString dataText = oldList.readAll();

            // 환자 이름은 NULL인 상태로 오므로 DB에서 검색해 변경해 줄 것
            query->exec("select * from patient WHERE patient_no = '" + id + "'");

            QString tempName;
            while (query->next()){
                tempName = query->value(1).toString();
            }

            // 파일에서 진료시작한 환자에 해당하는 정보를 찾아 진료대기 상태에서 진료중으로 변경해줌
            QString changeNeededText = id + "," + "WT";
            QString changeText = id + "," + "TM";
            QRegularExpression re(changeNeededText);
            QString replacementText(changeText);

            dataText.replace(re, replacementText);
            qDebug() <<"dataText: "<<dataText;
            QFile newList("waitingList.txt");
            if(newList.open(QFile::WriteOnly | QFile::Truncate)) {
                QTextStream out(&newList);
                out << dataText;
            }
            newList.close();

            if(pmsSocket == nullptr)        // pms가 켜져있지 않을 때
            {
                // 서버가 죽지 않도록 예외처리
                return;
            }
            else                            // pms가 켜져있을 때는 바로 대기 리스트에서 진료중으로 환자 상태를 변경하게끔 하기 위해 정보를 전송
                pmsSocket->write(sendData.toStdString().c_str());


        }
        else if(event == "VTF")     // 진료 완료: VTF(treatment finish) [받을 정보: 이벤트, pid / 보낼 정보: 이벤트, pid]
        {
            // 진료중 상태에서 진료가 완료되었을 때 환자상태를 수납대기로 변경
            QFile oldList("waitingList.txt");
            oldList.open(QIODevice::Text | QIODevice::ReadOnly);
            QString dataText = oldList.readAll();

            // 대기환자 목록 파일에서 받은 id를 토대로 환자를 찾아 수납대기 상태로 변경해줌
            QString changeNeededText = id + "," + "TM";
            QString changeText = id + "," + "WP";
            QRegularExpression re(changeNeededText);
            QString replacementText(changeText);
            dataText.replace(re, replacementText);
            QFile newList("waitingList.txt");
            if(newList.open(QFile::WriteOnly | QFile::Truncate)) {
                QTextStream out(&newList);
                out << dataText;
            }
            newList.close();

            // 진료완료 정보 저장 잘 되었다고 뷰어에 알려주기
            QString tempData = saveData.split("^")[1];
            QString sendData = "ACK^" + tempData;
            viewerSocket->write(sendData.toStdString().c_str());


            // pms가 켜져있지 않을 때
            if(pmsSocket == nullptr)
            {
                // 서버가 죽지 않도록 예외처리
                return;
            }
            else
            {
                sendData += "|";  // pms의 statusRequestSended함수에서 name에 해당하는 부분을 |로 나누어주기 때문에 필요한 부분
                pmsSocket->write(saveData.toStdString().c_str()); // 뷰어쪽에서 받은 정보 그대로 환자관리SW에 전송=>환자관리에서는 event가 VTS일 시에 환자 진료 상태 진료중으로 변경해주면 될 듯
            }

        }

        /*촬영 요청 이벤트(환자SW(진료대기)/뷰어SW(진료중)->촬영SW)*/
        else if(event == "SRQ")     // 촬영 의뢰: SRQ(shoot request)
        {
            QString tempData = saveData.split("^")[1];
            QString sendData = "ACK^" + tempData;

            // 촬영SW가 켜져있지 않을 때
            if(imagingSocket == nullptr)
            {
                // 들어왔던 소켓에 바로 ERR을 보내줌
                QString sendData = "ERR^SRQ<CR>" + id + "<CR>" + data;
                socket->write(sendData.toStdString().c_str());

                // 서버가 죽지 않도록 예외처리
                return;
            }
            else
                imagingSocket->write(sendData.toStdString().c_str());


            // 진료대기 환자 촬영 요청 시 대기리스트 파일에서 대기중인 환자를 촬영중으로 변경
            QFile oldList("waitingList.txt");
            oldList.open(QIODevice::Text | QIODevice::ReadOnly);
            QString dataText = oldList.readAll();
            qDebug() <<"dataText: "<<dataText;

            QString shootType = data.split("|")[1];
            if(shootType=="PANO")
                shootType ="PA";
            else if(shootType=="CEPH")
                shootType ="CE";
            else if(shootType=="BOTH")
                shootType ="BO";

            if(pmsSocket == socket)             // pms소켓에서 촬영의뢰 보냈을 때는 현재 상태 WT
            {
            QString changeNeededText = id + "," + "WT";
            QString changeText = id + "," + shootType;


            QRegularExpression re(changeNeededText);
            QString replacementText(changeText);

            dataText.replace(re, replacementText);
            qDebug() <<"dataText: "<<dataText;
            QFile newList("waitingList.txt");
            if(newList.open(QFile::WriteOnly | QFile::Truncate)) {
                QTextStream out(&newList);
                out << dataText;
            }
            newList.close();

            }
            else if(viewerSocket == socket)     // 뷰어에서 촬영의뢰 보냈을 때는 현재 상태 WT

            {
                QString changeNeededText = id + "," + "TM";
                QString changeText = id + "," + shootType;


                QRegularExpression re(changeNeededText);
                QString replacementText(changeText);

                dataText.replace(re, replacementText);
                qDebug() <<"dataText: "<<dataText;
                QFile newList("waitingList.txt");
                if(newList.open(QFile::WriteOnly | QFile::Truncate)) {
                    QTextStream out(&newList);
                    out << dataText;
                }
                newList.close();
            }


            // pms가 켜져있지 않을 때
            if(pmsSocket == nullptr)
            {
                // 서버가 죽지 않도록 예외처리
                return;
            }
            else
                // 촬영요청이 pms에서 오든 viewer에서 오든 둘 다 촬영중으로 바뀌었다는 신호를 받아야 하기 때문에 SRQ이벤트를 서버쪽에서 다시 보내주도록 하였음
                pmsSocket->write(sendData.toStdString().c_str());

            // 뷰어가 켜져있지 않을 때
            if(pmsSocket == nullptr)
            {
                // 서버가 죽지 않도록 예외처리
                return;
            }
            else
                viewerSocket->write(sendData.toStdString().c_str());

        }
        else if(event == "WPY")     // WPY: Wait Payment
        {
            // pms에서 수납처리를 누를 시에 오는 이벤트. waitingList 파일에서 해당 환자 정보를 삭제해 줌
            QFile oldList("waitingList.txt");
            oldList.open(QIODevice::Text | QIODevice::ReadOnly);
            QString dataText = oldList.readAll();
            qDebug() <<"dataText: "<<dataText;
            QString changeNeededText = id + "," + "WP\n";
            QString changeText = "";

            QRegularExpression re(changeNeededText);
            QString replacementText(changeText);

            dataText.replace(re, replacementText);
            qDebug() <<"dataText: "<<dataText;
            QFile newList("waitingList.txt");
            if(newList.open(QFile::WriteOnly | QFile::Truncate)) {
                QTextStream out(&newList);
                out << dataText;
            }
            newList.close();


            // 잘 수납처리 되었고 대기리스트에서 해당 정보가 잘 삭제되었음을 pms에 알려주기
            QString tempData = saveData.split("^")[1];
            QString sendData = "ACK^" + tempData;
            pmsSocket->write(sendData.toStdString().c_str());
        }
    }
    // 받은 내용에 <CR> 들어가 있지 않으면 return
    else
        return;
}



void MainServer::loadData()
{
    /*DB를 추가하고 DB의 이름을 설정*/
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE", "databaseConnection");
    db.setDatabaseName("database.db");

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
        // query->exec("INSERT INTO patient VALUES ('P00001', '김유선', '여성', '2023-02-06', '010-1111-1111', 'ㅈㅅ', 'ㅁㅁ')");
        // query->exec("INSERT INTO patient VALUES ('P00002', '김도예', '여성', '2023-02-06','010-2222-2222', 'ㅈㅅ', 'ㅁㅁ')");
        // query->exec("INSERT INTO patient VALUES ('P00003', '한성은', '여성', '2023-02-06','010-3333-3333', 'ㅈㅅ', 'ㅁㅁ')");
        patientModel->select();


        query2 = new QSqlQuery(db);
        query2->exec("CREATE TABLE IF NOT EXISTS dentist(dentist_no VARCHAR(10) Primary Key,"
                     "dentist_name VARCHAR(10) NOT NULL, dentist_sex VARCHAR(5) NOT NULL, dentist_tel VARCHAR(15) NOT NULL);");

        dentistModel = new QSqlTableModel(this, db);
        dentistModel->setTable("dentist");
        dentistModel->select();
        dentistModel->setHeaderData(0, Qt::Horizontal, tr("No"));
        dentistModel->setHeaderData(1, Qt::Horizontal, tr("Name"));
        dentistModel->setHeaderData(2, Qt::Horizontal, tr("Sex"));
        dentistModel->setHeaderData(3, Qt::Horizontal, tr("Telephone Number"));
        ui->dentistTableView->setModel(dentistModel);
        // 의사 정보는 수정삭제 불가능하게 만들어놨음. 고정된 정보
        query2->exec("INSERT INTO dentist VALUES ('D00001', '이정연', '여성', '010-1234-5678')");
        query2->exec("INSERT INTO dentist VALUES ('D00002', '안다미로', '남성', '010-8765-4321')");
        query2->exec("INSERT INTO dentist VALUES ('D00003', '박병규', '남성', '010-3456-7890')");
        query2->exec("INSERT INTO dentist VALUES ('1', '1', '남성', '010-3456-7890')");
        dentistModel->select();


        query3= new QSqlQuery(db);
        query3->exec("CREATE TABLE IF NOT EXISTS image(image_no VARCHAR(10) Primary Key, patient_no VARCHAR(10) NOT NULL,"
                     "type VARCHAR(10) NOT NULL, image_date VARCHAR(15) NOT NULL, image_path varchar(300) NOT NULL);");
        imageModel = new QSqlTableModel(this, db);
        imageModel->setTable("image");

        imageModel->setHeaderData(0, Qt::Horizontal, tr("Image No"));
        imageModel->setHeaderData(1, Qt::Horizontal, tr("Patient No"));
        imageModel->setHeaderData(2, Qt::Horizontal, tr("Type"));
        imageModel->setHeaderData(3, Qt::Horizontal, tr("Image Date"));
        imageModel->setHeaderData(4, Qt::Horizontal, tr("Image Path"));
        ui->imageTableView->setModel(imageModel);
        query3->exec("INSERT INTO image VALUES ('I00001', 'P00002', 'CEPH', '20230205', './Image/P00002/20230205_CEPH.bmp')");
        query3->exec("INSERT INTO image VALUES ('I00002', 'P00002', 'PANO', '20230205', './Image/P00002/20230205_PANO.bmp')");
        query3->exec("INSERT INTO image VALUES ('I00003', 'P00001', 'PANO', '20230205', './Image/P00002/20230205_PANO.bmp')");
        imageModel->select();

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
    }
}


QString MainServer::makeId( )
{
    int id;

    qDebug()<< "rowCount: " << patientModel->rowCount();

    if(patientModel->rowCount() == 0) {
        id = 1;
        return "P" + QString::number(id).rightJustified(5,'0');
    } else {
        int tempPid = patientModel->itemData(patientModel->index(patientModel->rowCount() - 1,0)).value(0).toString().right(5).toInt()+1; // 마지막 row의 pid+1 값을 리턴
        return "P" + QString::number(tempPid).rightJustified(5,'0');
    }
}

QString MainServer::makeReportNo()
{
    int id;

    qDebug()<< "reportModel rowCount: " << reportModel->rowCount();

    if(reportModel->rowCount() == 0) {
        id = 1;
        return "R" + QString::number(id).rightJustified(5,'0');
    } else {
        int tempReportNo= reportModel->itemData(reportModel->index(reportModel->rowCount() - 1,0)).value(0).toString().right(5).toInt()+1; // 마지막 row의 pid+1 값을 리턴
        return "R" + QString::number(tempReportNo).rightJustified(5,'0');
    }
}


QString MainServer::makeImageNo()
{
    int id;

    qDebug()<< "imageModel rowCount: " << imageModel->rowCount();

    if(imageModel->rowCount() == 0) {
        id = 1;
        return "I" + QString::number(id).rightJustified(5,'0');
    } else {
        int tempImageNo= imageModel->itemData(imageModel->index(imageModel->rowCount() - 1,0)).value(0).toString().right(5).toInt()+1; // 마지막 row의 pid+1 값을 리턴
        return "I" + QString::number(tempImageNo).rightJustified(5,'0');
    }
}

void MainServer::sendWaitingList(QTcpSocket* specSocket)
{

    // 대기목록 파일의 정보를 모듈로 보내주는 부분
    QFile oldList("waitingList.txt");

    qDebug() << oldList.exists();
    // waitingList.txt 파일이 존재하지 않는다면 대기리스트가 없는 것이므로 return 해준다
    if(oldList.exists() == false)
        return;


    oldList.open(QIODevice::Text | QIODevice::ReadOnly);
    QString dataText = oldList.readAll();
    qDebug() << "dataText"<<dataText;


    int waitLineCount = dataText.count(QLatin1Char('\n'));

    QString sendData = "ACK^WTR<CR>";


    // PMS에는 모든 정보를 보내줌
    if(specSocket == pmsSocket)
    {
        sendData += QString::number(waitLineCount) + "<CR>";


        for(int i=0; i<waitLineCount; i++)
        {
            QString tempLine = dataText.split("\n")[i];
            qDebug() << "tempLine" <<tempLine;


            // 여기서 일단 pid를 이용해 이름 찾고 두번째 값인 현재상태도 붙여줌
            QString tempPID = tempLine.split(",")[0];

            // 환자 이름은 없는 상태이므로 DB에서 검색해 같이 보내줄 것
            query->exec("select * from patient WHERE patient_no = '" + tempPID + "'");
            QString tempName;
            while (query->next()){
                tempName = query->value(1).toString();
            }

            // 세 번째 값인 상태도 구해줌
            QString tempStatus = tempLine.split(",")[1];

            sendData += tempPID + "|" + tempName + "|" + tempStatus;

            // 마지막 정보가 아니면 tempLine 뒤에 <r>을 붙여줌
            if(i != waitLineCount-1)
                sendData+="<r>";

        }
    }


    // 뷰어SW는 수납대기를 제외한 정보만 보내줌
    else if(specSocket==viewerSocket)
    {

        QString tempData;
        int tempCount = 0;

        for(int i=0; i<waitLineCount; i++)
        {
            QString tempLine = dataText.split("\n")[i];
            qDebug() << "tempLine" <<tempLine;

            QString tempPID = tempLine.split(",")[0];

            // 환자 이름은 없는 상태이므로 DB에서 검색해 같이 보내줄 것
            query->exec("select * from patient WHERE patient_no = '" + tempPID + "'");
            QString tempName;
            while (query->next()){
                tempName = query->value(1).toString();
            }

            QString tempStatus = tempLine.split(",")[1];

            if(tempStatus != "WP")
            {
                tempCount++;
                tempData += tempPID + "|" + tempName + "|" + tempStatus;

                if(i != waitLineCount-1)
                {
                    tempData += "<r>";
                }
            }
        }
        sendData += QString::number(tempCount) + "<CR>" + tempData;

        qDebug() << "VIEWER SendData" << sendData;

    }


    // 촬영SW는 촬영중인 정보만 보내줌
    else if(specSocket==imagingSocket)
    {

        QString tempData;
        int tempCount = 0;

        for(int i=0; i<waitLineCount; i++)
        {
            QString tempLine = dataText.split("\n")[i];
            QString tempPID = tempLine.split(",")[0];

            // 환자 이름은 받지 않은 상태이므로 DB에서 검색해 같이 보내줄 것
            query->exec("select * from patient WHERE patient_no = '" + tempPID + "'");
            QString tempName;
            while (query->next()){
                tempName = query->value(1).toString();
            }

            QString tempStatus = tempLine.split(",")[1];

            if(tempStatus == "PA" || tempStatus == "CE" || tempStatus == "BO")
            {
                tempCount++;
                tempData += tempPID + "|" + tempName + "|" + tempStatus;

                if(i != waitLineCount-1)
                {
                    tempData += "<r>";
                }
            }
        }
        sendData += QString::number(tempCount) + "<CR>" + tempData;
        qDebug() << "IMG SendData" << sendData;
    }
    specSocket->write(sendData.toStdString().c_str());
}

// quit버튼을 누르면 정상종료. 대기리스트 파일이 삭제됨
void MainServer::on_quitPushButton_clicked()
{
    QFile::remove("./waitingList.txt");
    this->close();
}
