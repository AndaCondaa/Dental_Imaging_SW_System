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

class QMessageBox;
class CBCTFileTransfer;
class CBCTLogThread;
class CBCTModelController;
class CBCTRawImageViewer;

typedef enum {
	RESET,
	READY,
	START,
	STOP
} ControlType;

typedef enum {
	Lower,
	Upper,
	Pano,
	Ceph,
} DataType;

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
	void on_MainPushButton_clicked();

	void on_SubPushButton_clicked();

	void on_CaptureStartPushButton_clicked();
	void on_CaptureStopPushButton_clicked();

	void on_READYPushButton_clicked();
	void on_RESETPushButton_clicked();
	void on_STARTPushButton_clicked();
	void on_STOPPushButton_clicked();
	//    void on_AscendingPushButton_pressed();

	//    void on_AscendingPushButton_released();

	//    void on_DescendingPushButton_pressed();

	//    void on_DescendingPushButton_released();

private:
	// OBJ 파일을 읽어 vtkPolydata로 저장한다. 
	// 
	// >> Path : Objfile Path. 
    // >> Mtls : Mtl 파일 Path.
    void resizeEvent(QResizeEvent* event) override;

	void Load_OBJFile(QStringList paths, QStringList mtls,std::vector<vtkSmartPointer<vtkPolyData>>& objs);

	vtkSmartPointer<vtkPolyData> Load_OBJFile(DataType type);

	void Create_Mapper(const std::vector<vtkSmartPointer<vtkPolyData>>& objs, std::vector<vtkSmartPointer<vtkMapper>>& mapper);
	void Create_Actor(const std::vector<vtkSmartPointer<vtkMapper>>& mappers, const QString& Color, std::vector<vtkSmartPointer<vtkActor>>& actors);
	vtkSmartPointer<vtkGenericOpenGLRenderWindow> Create_Render(const std::vector<vtkSmartPointer<vtkActor>>& Actors);

	
	CBCTFileTransfer* m_fileTransfer;
	CBCTLogThread* m_logThread;
	CBCTModelController* m_modelController;
	CBCTRawImageViewer* m_rawImageViewer;

	QMessageBox* m_panoErrorMessage;
	QMessageBox* m_cephErrorMessage;

	Ui::MainWindow* ui;

private slots:
	void slot_panoImage(QImage* pImg);
	void slot_cephImage(QImage* cImg);

signals:
	void READYSignal(ControlType);
	void RESETSignal(ControlType);
	void STARTSignal(ControlType);
	void STOPSignal(ControlType);

	// 멤버 변수
private:
	std::vector<vtkSmartPointer<vtkPolyData>> m_vecdata;
	std::vector<vtkSmartPointer<vtkMapper>> m_mapper_All;
	std::vector<vtkSmartPointer<vtkMapper>> m_mapper_Main;
	std::vector<vtkSmartPointer<vtkMapper>> m_mapper_Sub;

	std::vector<vtkSmartPointer<vtkActor>> m_actor_All;
	std::vector<vtkSmartPointer<vtkActor>> m_actor_Main;
	std::vector<vtkSmartPointer<vtkActor>> m_actor_Sub;
	std::vector< vtkSmartPointer<vtkGenericOpenGLRenderWindow>> m_renderwindow;
};
#endif // MAINWINDOW_H
