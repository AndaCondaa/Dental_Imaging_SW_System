#include "networkmanager.h"

#include <QBoxLayout>
#include <QDataStream>
#include <QTcpSocket>
#include <QFileDialog>
#include <QFile>
#include <QFileInfo>

NetworkManager::NetworkManager(QObject *parent)
    : QObject{parent}
{
    socket = new QTcpSocket(this);
    fd_flag = connectToMainHost("127.0.0.1"); // localhost
    connect(socket, SIGNAL(readyRead()), this, SLOT(receiveData()));

    if(!fd_flag)
        qDebug()<<("DataSocket connect fail\n");
    else{
        qDebug()<<("DataSocket connect\n");
        QString connectData = "CNT<CR>PMS<CR>";

        QByteArray sendTest = connectData.toStdString().c_str();
        socket->write(sendTest);
    }


    fileSocket = new QTcpSocket(this);
    fileSocket->connectToHost("127.0.0.1", 8001);
    //file_flag = connectToFileHost("127.0.0.1");

    connect(fileSocket, SIGNAL(readyRead()), this, SLOT(receiveFile()));


    if(fileSocket->waitForConnected())
        fileSocket->write("CNT<CR>PMS<CR>NULL");
    else
        qDebug()<<("FileServer connect failed\n");

    qDebug() << "%%%%%%%%%%%%%%%%%%%%%%%%";

}





bool NetworkManager::connectToMainHost(QString host)
{
    socket->connectToHost(host, 8000);
    return socket->waitForConnected();
}


//지워도 될 듯
bool NetworkManager::connectToFileHost(QString host)
{
    socket->connectToHost(host, 8001);
    return socket->waitForConnected();
}






void NetworkManager::receiveFile() //P00004 파일 2개짜리로 해보기
{

    fileSocket = dynamic_cast<QTcpSocket*>(sender());
    qDebug("%d", __LINE__);

    //    QDataStream in(&allFileSended, QIODevice::ReadOnly);
    //    in >> allFileSize >> allFile;
    //    QByteArray byte;
    //    byte = fileSocket->readAll();
    allFile.append(fileSocket->readAll());

    if(QString(allFile).right(5) == "<FIN>")
    {
        qDebug() << "allFile size: " <<allFile.size();
        qDebug() << "Finish Catched";
        makeFiles();
    }
}


void NetworkManager::makeFiles() //P00004 파일 2개짜리로 해보기
{
    QByteArray fileInfoArray = allFile.split('|')[0];
    qDebug() << "fileInfoArray.size: " << fileInfoArray.size(); //ex.53

    QString totalFileInfo = fileInfoArray.toStdString().c_str();
    int fileCount = totalFileInfo.split("<CR>")[0].toInt();
    QString fileInfo = totalFileInfo.split("<CR>")[1];


    QMap<int, int> fileMap; //인덱스, 파일크기
    QMap<int, QString> fileNameMap; //인덱스, 파일이름

    for(int i=0; i<fileCount; i++)
    {
        QString partInfo = fileInfo.split("<n>")[i];

        QString partFileName = partInfo.split("<f>")[0];
        qDebug() << "partFileName"<<partFileName;

        int partFileSize = partInfo.split("<f>")[1].toInt();
        qDebug() << "partFileSize"<<partFileSize;



        fileMap.insert(i, partFileSize);
        fileNameMap.insert(i, partFileName);
        allFileSize += partFileSize;
        qDebug()<< "************" <<allFileSize;
    }

    qDebug() <<fileInfoArray.size() +  fileMap.value(0) + fileMap.value(1) +6; //6은 |과 <FIN> 의 크기
    qDebug()<< allFile.sliced(fileInfoArray.size()+1, fileMap.value(0)).size();
    qDebug()<< allFile.sliced(fileMap.value(0)+1, fileMap.value(1)).size();
               //+ allFile.sliced(fileInfoArray.size()+fileMap.value(0), fileInfoArray.size()+fileMap.value(0) + fileMap.value(1)-1).size();

    //qDebug() << allFile.sliced(fileInfoArray.size()+1, fileInfoArray.size()+fileMap.value(0)-1);
    //qDebug() << allFile.sliced(fileInfoArray.size()+fileMap.value(0), fileInfoArray.size()+fileMap.value(0) + fileMap.value(1)-1);




        //QDir dir(QString("Image/%1").arg(currentPID));
        QDir dir("Image");
        if (!dir.exists())
            dir.mkpath(".");

        for(int i=0; i<fileCount; i++)
        {
            fileName = dir.path() + "/" + fileNameMap.value(i);
            QFile file;
            file.setFileName(fileName);
            file.open(QIODevice::WriteOnly);

            QByteArray writeArray;
            if(i==0)
                writeArray.append(allFile.sliced(fileInfoArray.size()+1, fileMap.value(0)));
            else
                writeArray.append(allFile.sliced(fileInfoArray.size()+1+fileMap.value(0), fileMap.value(i)));

            //qDebug()<<allFile.sliced(fileMap.value(i-1)+1, fileMap.value(i));

            file.write(writeArray);
        }

allFile.clear();

emit PSEDataInNET(id);
}






        /*이미지 파일을 string형태로 변환해버리면 나중에 다시 변환해줘야하는 번거로움이 있음.
      일단은 bytearray형태로 저장해야함*/
        //    allFile.append(byte);
        //    //QByteArray fileInfoArray = allFile.split('|')[0];
        //    qDebug() << "fileInfoArray.size: " << fileInfoArray.size(); //ex.53

        //    QString totalFileInfo = fileInfoArray.toStdString().c_str();
        //    int fileCount = totalFileInfo.split("<CR>")[0].toInt();
        //    QString fileInfo = totalFileInfo.split("<CR>")[1];

        //    for(int i=0; i<fileCount; i++)
        //    {
        //        QString partInfo = fileInfo.split("<n>")[i];

        //        QString partFileName = partInfo.split("<f>")[0];
        //        qDebug() << "partFileName"<<partFileName;

        //        int partFileSize = partInfo.split("<f>")[1].toInt();
        //        qDebug() << "partFileSize"<<partFileSize;

        //        fileNameMap.insert(partFileName, partFileSize);
        //        allFileSize += partFileSize;
        //        qDebug()<< "************" <<allFileSize;

        //    }


        //qDebug()<< "@@@@@"<<fileInfoArray;
        //    //qDebug()<< fileSocket->readAll();
        //    qDebug() << allFile.size();

        //    if(allFile.size()==allFileSize)
        //    {
        //        makeFiles();
        //    }










        //void NetworkManager::receiveFile() //P00004 파일 2개짜리로 해보기
        //{

        //    fileSocket = dynamic_cast<QTcpSocket*>(sender());
        //    qDebug("%d", __LINE__);


        //    /*이미지 파일을 string형태로 변환해버리면 나중에 다시 변환해줘야하는 번거로움이 있음.
        //      일단은 bytearray형태로 저장해야함*/
        //    allFile.append(fileSocket->readAll());
        //    QByteArray fileInfoArray = allFile.split('|')[0];
        //    qDebug() << "fileInfoArray.size: " << fileInfoArray.size(); //ex.53

        //    QString totalFileInfo = fileInfoArray.toStdString().c_str();
        //    int fileCount = totalFileInfo.split("<CR>")[0].toInt();
        //    QString fileInfo = totalFileInfo.split("<CR>")[1];

        //    for(int i=0; i<fileCount; i++)
        //    {
        //        QString partInfo = fileInfo.split("<n>")[i];

        //        QString partFileName = partInfo.split("<f>")[0];
        //        qDebug() << "partFileName"<<partFileName;

        //        int partFileSize = partInfo.split("<f>")[1].toInt();
        //        qDebug() << "partFileSize"<<partFileSize;

        //        fileNameMap.insert(partFileName, partFileSize);
        //        allFileSize += partFileSize;
        //        qDebug()<< "************" <<allFileSize;

        //    }


        //qDebug()<< "@@@@@"<<fileInfoArray;
        //    //qDebug()<< fileSocket->readAll();
        //    qDebug() << allFile.size();

        //    if(allFile.size()==allFileSize)
        //    {
        //        makeFiles();
        //    }

        //}

        //void NetworkManager::makeFiles()
        //{


        //    QByteArray fileInfoArray = allFile.split('|')[0];
        //    qDebug() << "fileInfoArray" << fileInfoArray;

        //    QByteArray fileArray = allFile.split('|')[1];
        //    qDebug()<<"sdssds" <<fileArray;


        //    //QString fileInfo = fileSocket->readAll().toStdString().c_str();
        //    //qDebug() << "fileInfo" << fileInfo;


        //    //qDebug("%d", __LINE__);
        //    qDebug("%d", allFile.size());


        //    QString fileName;

        //    QDir dir(QString("Image/%1").arg(currentPID));
        //    if (!dir.exists())
        //        dir.mkpath(".");

        //    fileName = dir.path() + "/" + QString("1.png");
        //    QFile file;
        //    file.setFileName(fileName);
        //    file.open(QIODevice::WriteOnly);

        //    file.write(allFile);
        //    qDebug("saved");

        //    file.close();

        //    qDebug("file saved");
        //}

















        //void NetworkManager::receiveFile() //P00004 파일 2개짜리로 해보기
        //{
        //    fileSocket = dynamic_cast<QTcpSocket*>(sender());
        //    qDebug("%d", __LINE__);

        //    allFile.append(fileSocket->readAll());
        //    qDebug()<< fileSocket->readAll();
        //    //qDebug("%d", __LINE__);
        //    //qDebug("%d", allFile.size());


        //    QString fileName;

        //    QDir dir(QString("Image/%1").arg(currentPID));
        //    if (!dir.exists())
        //        dir.mkpath(".");

        //    fileName = dir.path() + "/" + QString("1.png");
        //    QFile file;
        //    file.setFileName(fileName);
        //    file.open(QIODevice::WriteOnly);

        //    file.write(allFile);
        //    qDebug("saved");

        //    file.close();

        //    qDebug("file saved");
        //}







        //void NetworkManager::receiveFile()
        //{
        //    QTcpSocket *socket = dynamic_cast<QTcpSocket*>(sender());

        //    if (fileSocket != socket) {
        //        QByteArray arr = socket->readAll();
        //        QString id = QString(arr).split("<CR>")[1];
        //        if (id == "PMS") {  //근데 여기서는 굳이 소켓을 멤버변수로 설정하지는 않아도 될 것 같음. 소켓이 하나밖에 없어서..
        //            fileSocket = socket;
        //        }
        //        return;
        //    }

        //    qDebug() << "@@@@@@@@@@@@@@";

        //    if (byteReceived == 0) {                                    // First Time(Block) , var byteReceived is always zero
        //        //checkFileName = fileName;                               // 다음 패킷부터 파일이름으로 구분하기 위해 첫 패킷에서 보낸 파일이름을 임시로 저장

        //        //QDataStream in(fileSocket);
        //        //in.device()->seek(0);
        //        //in >> totalSize >> byteReceived >> fileName;
        //        //if(checkFileName == fileName) return;

        //        QFileInfo info(fileName);
        //        currentPID = info.fileName();

        //        QDir dir(QString("./Image/%1").arg(currentPID.first(6)));   //ex.P00001
        //        if (!dir.exists())
        //            dir.mkpath(".");

        //        QString currentFileName = dir.path() + "/" +info.fileName();

        //qDebug() << "$$$$$$$$$$$$";

        //        file = new QFile(currentFileName);
        //        file->open(QFile::WriteOnly);
        //    } else {
        //        //if(checkFileName == fileName) return;
        //        inBlock = fileSocket->readAll();

        //        byteReceived += inBlock.size();
        //qDebug() << "inBlock: " << inBlock;

        //        file->write(inBlock);
        //        file->flush();
        //    }

        //    if (byteReceived == totalSize) {        // file sending is done
        //        qDebug() << QString("%1 receive completed").arg(fileName);
        //        inBlock.clear();
        //        byteReceived = 0;
        //        totalSize = 0;
        //        file->close();
        //        delete file;

        //        emit PSEDataInNET(currentPID.first(6));
        //    }
        //}

        //QByteArray IntToArray(qint32 source)
        //{
        //    QByteArray temp;
        //    QDataStream data(&temp, QIODevice::ReadWrite);
        //    data << source;
        //    return temp;
        //}


        bool NetworkManager::writeData(QByteArray data)
        {
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

        //서버로 보내줄 데이터
        void NetworkManager::newDataSended(QString newData)
        {

            if(fd_flag)
            {
                QString sendData = newData; //MainServer의 textEdit에 띄울 정보
                send_flag = writeData(sendData.toStdString().c_str()); //writeData의 첫 번째 인자는 char *data와 같은 형식임
                if(!send_flag)
                    qDebug() << "Socket send fail\n";

            }

        }

        //서버에서 받아올 데이터
        void NetworkManager::receiveData()
        {
            qDebug("%d", __LINE__);
            socket = static_cast<QTcpSocket*>(sender());
            //buffer = buffers.value(socket);
            qDebug("%d", __LINE__);
            //buffer->append(socket->readAll());
            QByteArray array = socket->readAll();
            qDebug("%d", __LINE__);
            //    saveData = QString(buffer->data());
            saveData = QString(array);
            qDebug("%d", __LINE__);

            if(saveData.contains("<CR", Qt::CaseInsensitive) == true)
            {
                //어떤 이벤트인지에 따라 불러올 함수 써주기(각각 이벤트에 대한 함수 만들고 if-else문 타도록 만들자)
                QString event = saveData.split("<CR>")[0];
                id = saveData.split("<CR>")[1];
                QString data = saveData.split("<CR>")[2];
                qDebug("%d", __LINE__);
                qDebug() << "event: " << event;

                if(event == "PID")
                {
                    sendedPID = id;
                    qDebug() << "sendedPID: " << id;
                    emit sendNewPID(sendedPID); //enrollment 클래스로 emit
                }
                else if(event == "PSE")
                {
                    qDebug() << "PSE data: " << data;
                    emit sendSearchResult(id, data);    //patientInfoManager 클래스와 medicalRecordManager 클래스 두 곳으로 모두 보내줘야 함
                }
                else if(event == "SRQ")
                {
                    qDebug()<<"SRQ event Received: " << saveData;
                    emit sendSRQRequest(saveData);
                }
                else if(event == "VTS")
                {
                    qDebug()<<"VTS event Received: " << saveData;
                    emit sendVTSRequest(saveData);
                }
                else if(event == "ISV")
                {
                    qDebug()<<"ISV event Received: " << saveData;
                    emit sendISVevent(saveData);
                }
                else if(event == "VTF")
                {
                    qDebug()<<"VTF event Received: " << saveData;
                    emit sendVTFevent(saveData);
                }
                else if(event == "VNT")
                {
                    qDebug()<<"VNT event Received: " << saveData;
                    emit sendVNTevent(saveData);
                }


                //    buffer->clear(); //버퍼 비워주기
            }
        }

