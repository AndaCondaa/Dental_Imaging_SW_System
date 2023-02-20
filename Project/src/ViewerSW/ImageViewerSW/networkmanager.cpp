/*프로그램명 : ImageViewerSW
파일명 : networkmanager.cpp
설명 : 서버와 클라이언트(뷰어 SW)간 Event를 통해 데이터를 주고받고 하기 위한 클래스
작성자 : 이정연
최종 수정 날짜 : 2023.02.14*/


#include "networkmanager.h"
#include <QBoxLayout>
#include <QDataStream>
#include <QTcpSocket>
#include <QFileDialog>
#include <QFile>
#include <QFileInfo>
#include <QMessageBox>

NetworkManager::NetworkManager(QObject *parent)
    : QObject{parent}
{
    messagebox = new QMessageBox();
    messagebox->setStyleSheet("QMessageBox { "
                              "background-color: rgb(255, 255, 255);"
                              "color:#000000;"
                              "}"
                              "QPushButton { "
                              "background-color: rgb(250, 250, 250);"
                              "border-radius:2px;"
                              "font: bold;"
                              "color:rgb(0, 0, 0);"
                              "border-color: blue"
                              "outline: 0;"
                              "}");
}

// 접속 버튼 클릭 시 서버로 전송하기
void NetworkManager::receiveLoginToServer(QString IP, int port)
{    
    socket = new QTcpSocket(this);
    fileSocket = new QTcpSocket(this);

    socket->connectToHost(IP, 8000);
    fileSocket->connectToHost(IP, 8001);

    if (socket->waitForConnected(1000) && fileSocket->waitForConnected(1000)) {
        connect(socket, SIGNAL(readyRead()), this, SLOT(receiveData()));
        connect(socket, SIGNAL(disconnected()), this, SLOT(disconnect()));
        connect(fileSocket, SIGNAL(readyRead()), this, SLOT(receiveFile()));

        QString connectData = "SEN^CNT<CR>VEW<CR>NULL";
        QByteArray sendTest = connectData.toStdString().data();
        socket->write(sendTest);
        fileSocket->write(sendTest);
        emit serverConnection(1);
    }
    else {
        QMessageBox::critical(messagebox, "경고", "서버와 연결되지 않았습니다.");
        socket->deleteLater();
        fileSocket->deleteLater();
    }
}

// 서버와 연결이 끊어졌을 때 로그인창으로 변경 및 소켓 삭제
void NetworkManager::disconnect()
{
    QMessageBox::critical(messagebox, "경고", "서버와의 연결이 끊어졌습니다");

    // 프로그램 강제 종료 시 해당 환자의 이미지 파일 삭제
    QDir dir("./Image/");
    dir.removeRecursively();

    socket->deleteLater();
    fileSocket->deleteLater();

    emit serverConnection(0);
    emit disconnectedToServer(0);
}


bool NetworkManager::writeData(QByteArray data)
{
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

void NetworkManager::receiveFile()
{
    fileSocket = dynamic_cast<QTcpSocket*>(sender());
    allFile.append(fileSocket->readAll());

    if(QString(allFile).right(5) == "<FIN>")
    {
        qDebug() << "Finish Catched";
        makeFiles();
    }
}

void NetworkManager::makeFiles()
{
    QByteArray fileInfoArray = allFile.split('|')[0];
    QString totalFileInfo = fileInfoArray.toStdString().c_str();
    int fileCount = totalFileInfo.split("<CR>")[0].toInt();
    QString fileInfo = totalFileInfo.split("<CR>")[1];

    QMap<int, int> fileMap; // 인덱스, 파일크기
    QMap<int, QString> fileNameMap; // 인덱스, 파일이름

    for(int i = 0; i < fileCount; i++)
    {
        QString partInfo = fileInfo.split("<n>")[i];
        QString partFileName = partInfo.split("<f>")[0];
        qDebug() << "partFileName" << partFileName;

        int partFileSize = partInfo.split("<f>")[1].toInt();
        qDebug() << "partFileSize" << partFileSize;

        fileMap.insert(i, partFileSize);
        fileNameMap.insert(i, partFileName);
        allFileSize += partFileSize;
    }

        QDir dir("Image");
        if (!dir.exists())
            dir.mkpath(".");

        int firstSize, secondSize;
        for(int i = 0; i < fileCount; i++)
        {
            fileName = dir.path() + "/" + fileNameMap.value(i);
            QFile file;
            file.setFileName(fileName);
            file.open(QIODevice::WriteOnly);

            QByteArray writeArray;
            if(i==0){
                writeArray.append(allFile.sliced(fileInfoArray.size()+1, fileMap.value(0)));
                firstSize = fileInfoArray.size()+1;
                secondSize = fileMap.value(0);
            }
            else{
                writeArray.append(allFile.sliced(firstSize+secondSize, fileMap.value(i)));
                firstSize = firstSize+secondSize;
                secondSize = fileMap.value(i);
            }

            file.write(writeArray);
        }
        allFile.clear();

        // 환자의 이미지 파일이 모두 수신되면 true 값 반환
        emit sendAllImageFile(1);

        // 진료 시작 버튼 클릭 시 해당 환자의 이미지 업로드
        emit sendImageFile();
}


// 서버로 보내줄 데이터
void NetworkManager::newDataSended(QString newData)
{    
    // MainServer의 textEdit에 띄울 정보
    socket->write(newData.toStdString().c_str());
}

// 서버에서 받아올 데이터
void NetworkManager::receiveData()
{
    socket = static_cast<QTcpSocket*>(sender());
    QByteArray array = socket->readAll();
    saveData = QString(array);
    QString signal = saveData.split("^")[0];
    saveData = saveData.split("^")[1];

    if(saveData.contains("<CR", Qt::CaseInsensitive) == true)
    {
        // 어떤 이벤트인지에 따라 불러올 함수 써주기(각각 이벤트에 대한 함수 만들고 if-else문 타도록 만들자)
        QString event = saveData.split("<CR>")[0];
        QString id = saveData.split("<CR>")[1];
        QString data = saveData.split("<CR>")[2];

        // AWL : PMS에서 대기 환자 추가 시 뷰어의 대기 리스트에도 추가
        if(event == "AWL" && signal == "ACK")
        {
            emit sendWaitingList(id, data);
        }

        // VTS(진료 시작) : 대기 리스트에서 선택된 환자 정보 추가
        else if(event == "VTS" && signal == "ACK")
        {
            emit sendSelectPatient(id, data);
        }

        // 촬영SW에서 전달받아 뷰어와 PMS에서 촬영을 시작하는 환자의 진행 상황 추가
        else if(event == "SRQ" && signal == "ACK")
        {
            emit sendPMSCameraPatient(id, data);
        }

        // 촬영 SW가 꺼져있을 경우 Data로 Null을 전송
        else if(event == "SRQ" && signal == "ERR")
        {
            emit sendPMSCameraPatient(id, "Null");
        }

        // VLG(로그인) : 뷰어에서 로그인 시 해당 의사 정보로 로그인 여부 확인
        else if(event == "VLG" && signal == "ACK")
        {
            emit sendLogInCheck(data);
        }

        // 촬영SW에서 환자의 촬영 완료를 알리고 해당 환자 정보 전송
        else if(event == "ISV" && signal == "ACK")
        {
            emit sendPhotoEnd(id);
        }

        // 뷰어 프로그램 시작 시 PMS의 대기리스트 정보를 바로 불러와 띄워주기 위한 데이터
        else if(event == "WTR" && signal == "ACK")
        {
            emit sendWaitTreatment(id.toInt(), data);
        }

        // 처방전 작성 완료 버튼 클릭 시 DB에 저장 여부를 시그널 전송
        else if(event == "VNT")
        {
            emit sendPrescriptionCheck(signal);
        }

        // 진료 종료 버튼 클릭 시 서버에 환자 관리 클래스를 비워주기 위해 시그널 전송
        else if(event == "VTF" && signal == "ACK")
        {
            emit sendPatientTreatmentEnd();
        }

        // 촬영 SW가 연결되면 메시지 박스를 통해 알려줌
        else if(event == "CNT" && signal == "ACK" && id == "IMG")
        {
            QMessageBox::information(messagebox, "확인", "촬영 SW와 연결이 되었습니다.");
        }
    }
}

