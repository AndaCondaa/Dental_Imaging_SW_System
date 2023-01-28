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
#include "fileprotocol.h"
#include "packetdata.h"

class QMessageBox;
class CBCTModelController;
class CBCTRawImageViewer;
class CBCTFileTransfer;
class CBCTLogThread;
class ControlPanel;
class Protocol;
class FileProtocol;
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

private:
	void resizeEvent(QResizeEvent* event) override;

	CBCTFileTransfer* m_fileTransfer;
	CBCTLogThread* m_logThread;
	CBCTModelController* m_modelController;
	CBCTRawImageViewer* m_rawImageViewer;
    ControlPanel* m_controlPanel;
	QMessageBox* m_panoErrorMessage;
	QMessageBox* m_cephErrorMessage;

	Ui::MainWindow* ui;
	
private slots:
	void slot_panoImage(QImage* pImg);
	void slot_cephImage(QImage* cImg);

	void on_openGLWidget_All_resized();

signals:
	void READYSignal(ControlType);
	void RESETSignal(ControlType);
	void STARTSignal(ControlType);
	void STOPSignal(ControlType);

};
#endif // MAINWINDOW_H
