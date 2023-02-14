#include "cbctlogthread.h"
#include <QDebug>
#include <QThread>

CBCTLogThread::CBCTLogThread()
{
 }


void CBCTLogThread::run(){

}

void CBCTLogThread::runMethod()
{
    m_modelController = new CBCTModelController;
    m_rawImageViewer = new CBCTRawImageViewer;

    QThread* modelThread = new QThread;
    QThread* viewThread = new QThread;

    m_modelController->moveToThread(modelThread);
    m_rawImageViewer->moveToThread(viewThread);

 //   connect(modelThread, &QThread::started, m_modelController, &CBCTModelController::on_MainPushButton_clicked);
 //   connect(viewThread, &QThread::started, m_rawImageViewer, &CBCTRawImageViewer::viewer_Output);

}
