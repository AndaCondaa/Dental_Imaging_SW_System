#ifndef CBCTLOGTHREAD_H
#define CBCTLOGTHREAD_H

#include <QObject>
#include <QtWidgets>
#include <QThread>
#include "cbctmodelcontroller.h"
#include "cbctrawimageviewer.h"

class CBCTLogThread : public QThread
{
    Q_OBJECT
public:
    CBCTLogThread();
    void run() override;

signals:
    void pano_Capture_Thread_Signal();
//    void ceph_Capture_Thread_Signal();
protected:

private slots:
    void runMethod();
public slots:
 //   void pano_Capture_Thread_Slot();
 //   void pano_Capture_Start_Thread();

private:
    CBCTModelController *m_modelController;
    CBCTRawImageViewer *m_rawImageViewer;
};

\




#endif // CBCTLOGTHREAD_H
