#ifndef MAINWINDOW_H
#define MAINWINDOW_H


#include <QMainWindow.h>
#include <QObject.h>

#include <vtkSmartPointer.h>
#include <vtkPolyData.h>
#include <vtkPolyDataMapper.h>
#include <vtkActor.h>
#include <vtkRenderer.h>
#include <vtkGenericOpenGLRenderWindow.h>
#include "cbctmodelcontroller.h"
#include "cbctrawimageviewer.h"
#include "cbctfiletransfer.h"
#include "cbctlogthread.h"
#include "controlpanel.h"
#include "protocol.h"
#include "packetdata.h"

class QMessageBox;
class CBCTModelController;
class CBCTRawImageViewer;
class CBCTFileTransfer;
class CBCTLogThread;
class ControlPanel;
class Protocol;
class PacketData;

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	MainWindow(QWidget* parent = nullptr);
	~MainWindow();

private slots:
	void on_CaptureReadyPushButton_clicked();
	void on_CaptureResetPushButton_clicked();
	void on_CaptureStartPushButton_clicked();
	void on_CaptureStopPushButton_clicked();


	void receive_Pano_Modality();
	void receive_Ceph_Modality();
	void fileLogSlot(QString mode, QString fileLog);
	void receive_Message_LogSlot(QString receiveMsg);
	void send_Message_LogSlot(QString msg);

	void emitResetSignal();
	void emitReadySignal();
	void emitStartSignal();
	void emitStopSignal();

private:
	void resizeEvent(QResizeEvent* event) override;

	// Button 초기 상태 지정 
	void initializeButton();

	// CBCTModel Controller 관련 connect 
	void connectCBCTModelCtr();

	// UI Button 관련 connect 
	void connectUIBtn();

	// CBCTRawImageViewer 관련 connect 
	void connectCBCTRawImageView();

	// CBCTFileTransfer 관련 connect
	void connectCBCTFileTrans();

	void connectLogMaster();

	CBCTFileTransfer* m_fileTransfer;
	CBCTLogThread* m_logThread;
	CBCTModelController* m_modelController;
	CBCTRawImageViewer* m_rawImageViewer;
	MainWindow* m_mainWindow;
	ControlPanel* m_controlPanel;
	QMessageBox* m_panoErrorMessage;
	QMessageBox* m_cephErrorMessage;

	Ui::MainWindow* ui;

private slots:
	void slot_panoImage(QImage* pImg);
	void slot_cephImage(QImage* cImg);

};
#endif // MAINWINDOW_H
