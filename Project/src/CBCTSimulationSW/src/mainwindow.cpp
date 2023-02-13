#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QFile>
#include <QGraphicsPixmapItem>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QList>
#include <QTransform>
#include <qstring.h>
#include <qevent.h>
#include <QThread>
#include <QDebug>
#include <QDateTime>
#include <QtNetwork>

//#define USE_DISPLAY_GLOBALAXIS

MainWindow::MainWindow(QWidget* parent)
	: QMainWindow(parent), ui(new Ui::MainWindow)
{
	ui->setupUi(this);

	/* 프로그램 시작 시 초기화 및 준비 버튼만 활성화 */
	initializeButton();

	// Model Controller 생성.
	m_modelController = new CBCTModelController(ui);
	m_rawImageViewer = new CBCTRawImageViewer();
	m_fileTransfer = new CBCTFileTransfer(this);

	if (!m_modelController->initialize())
		qDebug() << "CBCTModelController initialize Fail ! ";

	/* 모델컨트롤러 시그널 & 슬롯 */
	connectCBCTModelCtr();

	/* 버튼 클릭시 기본 기능 동작*/
	connectUIBtn();

	/* rawImageViewer 로부터 메인 윈도우에 표시하기 위한 시그널 & 슬롯 */
	connectCBCTRawImageView();

	/* 촬영 SW에서 Signal 받았을 시 Emit Signal 제외한 기능 동작 */
	connectCBCTFileTrans();

	connectLogMaster();
}

MainWindow::~MainWindow()
{
	delete ui;
}

void MainWindow::resizeEvent(QResizeEvent* event)
{
	auto Size = event->size();
	QMainWindow::resizeEvent(event);
}

void MainWindow::initializeButton()
{
	// TODO : 상황에 따라 업데이트 필요한 Widget들이 있다. 
	// 
	// 초기화면에서 Reset, Ready만 활성화 
	// check box는 Pano만 체크 된 상태로 
	ui->PanoCheckBox->setChecked(true);

	ui->CaptureResetPushButton->setEnabled(true);
	ui->CaptureReadyPushButton->setEnabled(true);
	ui->CaptureStartPushButton->setEnabled(false);
	ui->CaptureStopPushButton->setEnabled(false);

	ui->AscendingPushButton->setAutoRepeat(true);
	ui->AscendingPushButton->setAutoRepeatDelay(100);
	ui->AscendingPushButton->setAutoRepeatInterval(100);

	ui->DescendingPushButton->setAutoRepeat(true);
	ui->DescendingPushButton->setAutoRepeatDelay(100);
	ui->DescendingPushButton->setAutoRepeatInterval(100);
}

void MainWindow::connectCBCTModelCtr()
{

	connect(ui->StopPushButton, SIGNAL(clicked()), m_modelController, SLOT(stop()));
	connect(ui->ResetPushButton, SIGNAL(clicked()), m_modelController, SLOT(on_PanoModel_Reset()));
	connect(ui->ResetPushButton, SIGNAL(clicked()), m_modelController, SLOT(on_CephModel_Reset()));
	// TODO : Patient Remove 동작에 따른 Signal 연결 (Widget 구성 후 ) 
	connect(ui->MainPushButton, SIGNAL(clicked()), m_modelController, SLOT(on_MainPushButton_clicked()));
	connect(ui->CaptureResetPushButton, SIGNAL(clicked()), m_modelController, SLOT(on_PanoModel_Reset()));

	connect(ui->SubPushButton, SIGNAL(clicked()), m_modelController, SLOT(on_SubPushButton_clicked()));
	connect(ui->CaptureResetPushButton, SIGNAL(clicked()), m_modelController, SLOT(on_CephModel_Reset()));

	connect(ui->CaptureReadyPushButton, &QPushButton::clicked, m_modelController, [&](bool state) {

		if (ui->CephCheckBox->isChecked())
		{
			m_modelController->on_XRayModule_Ready();
		}
		});

	connect(ui->AscendingPushButton, SIGNAL(clicked()), m_modelController, SLOT(on_AscendingPushButton_pressed()));
	connect(ui->DescendingPushButton, SIGNAL(clicked()), m_modelController, SLOT(on_DescendingPushButton_pressed()));


	connect(ui->InvitePatientPushButton, &QPushButton::clicked, this, [&](bool state) {

		if (ui->PanoCheckBox->isChecked())
		{
		
		QString filepath = QFileDialog::getOpenFileName(this, "patient", "C:\\", "Files(*.*)");

		if (!m_modelController->Load_PanoPatient(filepath))
			qDebug() << "Load 실패 했습니다.";
		}
		else if(ui->CephCheckBox->isChecked())
		{
		QString filepath = QFileDialog::getOpenFileName(this, "patient", "C:\\", "Files(*.*)");

		if (!m_modelController->Load_CephPatient(filepath))
			qDebug() << "Load 실패 했습니다.";
		}
		});

	connect(ui->LeavePatientPushButton, &QPushButton::clicked, this, [&](bool state) {
		m_modelController->Remove_PanoPatient();
		m_modelController->Remove_CephPatient();
		});
}

void MainWindow::connectUIBtn()
{
	connect(ui->CaptureResetPushButton, SIGNAL(clicked()), this, SLOT(on_CaptureResetPushButton_clicked()));
	connect(ui->CaptureReadyPushButton, SIGNAL(clicked()), this, SLOT(on_CaptureReadyPushButton_clicked()));
	connect(ui->CaptureStartPushButton, SIGNAL(clicked()), this, SLOT(on_CaptureStartPushButton_clicked()));
	connect(ui->CaptureStopPushButton, SIGNAL(clicked()), this, SLOT(on_CaptureStopPushButton_clicked()));


	// ui check box Update
	connect(ui->PanoCheckBox, &QCheckBox::clicked, this, [&](bool state) {
		if (state == true)
		{
			if (ui->CephCheckBox->isChecked())
			{
				ui->CephCheckBox->setCheckState(Qt::Unchecked);
			}
		}
		});

	connect(ui->CephCheckBox, &QCheckBox::clicked, this, [&](bool state) {
		if (state == true)
		{
			if (ui->PanoCheckBox->isChecked())
			{
				ui->PanoCheckBox->setCheckState(Qt::Unchecked);
			}
		}
		});
}

void MainWindow::connectCBCTRawImageView()
{
	connect(m_rawImageViewer, SIGNAL(signals_panoImage(QImage*)), this, SLOT(slot_panoImage(QImage*)));
	connect(m_rawImageViewer, SIGNAL(signals_cephImage(QImage*)), this, SLOT(slot_cephImage(QImage*)));
}

void MainWindow::connectCBCTFileTrans()
{
	/* 촬영 SW 에서 시그널 받았을 때 기본 기능 동작 */
	connect(m_fileTransfer, SIGNAL(receiveResetSignal(QString)), this, SLOT(on_CaptureResetPushButton_clicked()));
	connect(m_fileTransfer, SIGNAL(receiveReadySignal(QString)), this, SLOT(on_CaptureReadyPushButton_clicked()));
	connect(m_fileTransfer, SIGNAL(receiveStartSignal(QString)), this, SLOT(on_CaptureStartPushButton_clicked()));
	connect(m_fileTransfer, SIGNAL(receiveStopSignal(QString)), this, SLOT(on_CaptureStopPushButton_clicked()));

	/* 촬영 SW에서 Signal 받았을 시 Modality CheckBox 기능 동작 */
	connect(m_fileTransfer, SIGNAL(receivePanoSignal(QString)), this, SLOT(receive_Pano_Modality()));
	connect(m_fileTransfer, SIGNAL(receiveCephSignal(QString)), this, SLOT(receive_Ceph_Modality()));


	/* 장비에서 클릭 시 시그널 전송 */
	connect(ui->CaptureResetPushButton, SIGNAL(clicked()), this, SLOT(emitResetSignal()));
	connect(ui->CaptureReadyPushButton, SIGNAL(clicked()), this, SLOT(emitReadySignal()));
    connect(ui->CaptureStartPushButton, SIGNAL(clicked()), this, SLOT(emitStartSignal()));
    connect(ui->CaptureStopPushButton, SIGNAL(clicked()), this, SLOT(emitStopSignal()));


}

void MainWindow::connectLogMaster()
{
	/* 촬영 SW로 Signal 보낼 때 로그 출력 */
	connect(m_fileTransfer, SIGNAL(sending_Control_Signal(QString)), this, SLOT(send_Message_LogSlot(QString)));

	/* 파일 전송 시 로그 출력 */
	connect(m_fileTransfer, SIGNAL(fileLogSignal(QString, QString)), this, SLOT(fileLogSlot(QString, QString)));

	/* 촬영 SW에서 Signal 받았을 시 로그 출력 */
	connect(m_fileTransfer, SIGNAL(receiveResetSignal(QString)), this, SLOT(receive_Message_LogSlot(QString)));
	connect(m_fileTransfer, SIGNAL(receiveReadySignal(QString)), this, SLOT(receive_Message_LogSlot(QString)));
	connect(m_fileTransfer, SIGNAL(receiveStartSignal(QString)), this, SLOT(receive_Message_LogSlot(QString)));
	connect(m_fileTransfer, SIGNAL(receiveStopSignal(QString)), this, SLOT(receive_Message_LogSlot(QString)));
	connect(m_fileTransfer, SIGNAL(receivePanoSignal(QString)), this, SLOT(receive_Message_LogSlot(QString)));
	connect(m_fileTransfer, SIGNAL(receiveCephSignal(QString)), this, SLOT(receive_Message_LogSlot(QString)));

}

void MainWindow::receive_Message_LogSlot(QString receiveMsg)
{
	ui->MessageLogTableWidget->insertRow(ui->MessageLogTableWidget->rowCount());
	ui->MessageLogTableWidget->setItem(ui->MessageLogTableWidget->rowCount() - 1, 0, new QTableWidgetItem(receiveMsg));
	ui->MessageLogTableWidget->setItem(ui->MessageLogTableWidget->rowCount() - 1, 1, new QTableWidgetItem(QDateTime::currentDateTime().toString()));

}

void MainWindow::send_Message_LogSlot(QString msg)
{
	ui->MessageLogTableWidget->insertRow(ui->MessageLogTableWidget->rowCount());
	ui->MessageLogTableWidget->setItem(ui->MessageLogTableWidget->rowCount() - 1, 0, new QTableWidgetItem(msg));
	ui->MessageLogTableWidget->setItem(ui->MessageLogTableWidget->rowCount() - 1, 1, new QTableWidgetItem(QDateTime::currentDateTime().toString()));

}

void MainWindow::fileLogSlot(QString mode, QString fileLog)
{
	ui->FileLogTableWidget->insertRow(ui->FileLogTableWidget->rowCount());
	ui->FileLogTableWidget->setItem(ui->FileLogTableWidget->rowCount() - 1, 0, new QTableWidgetItem(mode));
	ui->FileLogTableWidget->setItem(ui->FileLogTableWidget->rowCount() - 1, 1, new QTableWidgetItem(fileLog));
	ui->FileLogTableWidget->setItem(ui->FileLogTableWidget->rowCount() - 1, 2, new QTableWidgetItem(QDateTime::currentDateTime().toString()));
}

void MainWindow::receive_Pano_Modality()
{
	ui->PanoCheckBox->setCheckState(Qt::Unchecked);
	ui->CephCheckBox->setCheckState(Qt::Unchecked);
	ui->PanoCheckBox->setChecked(true);
}

void MainWindow::receive_Ceph_Modality()
{
	ui->PanoCheckBox->setCheckState(Qt::Unchecked);
	ui->CephCheckBox->setCheckState(Qt::Unchecked);
	ui->CephCheckBox->setChecked(true);
}

void MainWindow::emitResetSignal()
{
	if (ui->PanoCheckBox->isChecked())
	{
		m_fileTransfer->sendingControl("SEN", "CTL", 0, "PANO");
	}
	if (ui->CephCheckBox->isChecked())
	{
		m_fileTransfer->sendingControl("SEN", "CTL", 0, "CEPH");
	}

}
void MainWindow::emitReadySignal()
{

	if (ui->PanoCheckBox->isChecked())
	{
		m_fileTransfer->sendingControl("SEN", "CTL", 1, "PANO");

	}
	if (ui->CephCheckBox->isChecked())
	{
		m_fileTransfer->sendingControl("SEN", "CTL", 1, "CEPH");
	}


}

void MainWindow::emitStartSignal()
{
	//   m_mainWindow->on_CaptureStartPushButton_clicked();
	if (ui->PanoCheckBox->isChecked())

		if (ui->PanoCheckBox->isChecked())
		{
			m_fileTransfer->sendingControl("SEN", "CTL", 2, "PANO");
		}
		else if (ui->CephCheckBox->isChecked())
			{
				m_fileTransfer->sendingControl("SEN", "CTL", 2, "CEPH");
			}
}
	//    emit sendStartSignal(ControlType::START);

void MainWindow::emitStopSignal()
{
	
	if (ui->PanoCheckBox->isChecked())
	{
		m_fileTransfer->sendingControl("SEN", "CTL", 3, "PANO");
	}
	else if (ui->CephCheckBox->isChecked())
	{
		m_fileTransfer->sendingControl("SEN", "CTL", 3, "CEPH");
	}
	//    emit STOPSignal(ControlType::STOP);
}

void MainWindow::on_CaptureResetPushButton_clicked()
{
	/* reset 버튼 클릭시 start, stop 버튼 비활성화 */
	ui->CaptureResetPushButton->setEnabled(true);
	ui->CaptureReadyPushButton->setEnabled(true);
	ui->CaptureStartPushButton->setEnabled(false);
	ui->CaptureStopPushButton->setEnabled(false);

	if (ui->PanoCheckBox->isChecked())
	{
		m_rawImageViewer->resetPanoTimer();
	}
	else if (ui->CephCheckBox->isChecked())
	{
		m_rawImageViewer->resetCephTimer();
	}
	ui->PanoProgressBar->reset();
	ui->CephProgressBar->reset();
	ui->PanoLabel->clear();
	ui->CephLabel->clear();
}

void MainWindow::on_CaptureReadyPushButton_clicked()
{
	/* ready 버튼 클릭시 pressed 상태로 고정 후 start 버튼 활성화 */
	ui->CaptureReadyPushButton->setEnabled(false);
	ui->CaptureStartPushButton->setEnabled(true);


	if (ui->PanoCheckBox->isChecked())
	{
		m_rawImageViewer->readyPanoTimer();
	}
	else if (ui->CephCheckBox->isChecked())
	{
		m_rawImageViewer->readyCephTimer();
	}

}

void MainWindow::on_CaptureStartPushButton_clicked()
{
	/* start 버튼 클릭시 Stop 버튼만 활성화 */
	ui->CaptureResetPushButton->setEnabled(false);
	ui->CaptureStartPushButton->setEnabled(false);
	ui->CaptureStopPushButton->setEnabled(true);

	if (ui->PanoCheckBox->isChecked())
	{
		if (ui->CephCheckBox->isChecked())
		{
			m_panoErrorMessage = new QMessageBox;
		m_panoErrorMessage:ERROR_LOG_POLICY_CONFLICT;
		}
		else
		{
			qDebug() << __FUNCTION__;
			m_rawImageViewer->startPanoTimer();
		}
	}

	if (ui->CephCheckBox->isChecked())
	{
		if (ui->PanoCheckBox->isChecked())
		{
			m_cephErrorMessage = new QMessageBox;
		m_cephErrorMessage:ERROR_LOG_POLICY_CONFLICT;

		}
		else
		{
			qDebug() << __FUNCTION__;
			m_rawImageViewer->startCephTimer();
		}
	}
}

void MainWindow::on_CaptureStopPushButton_clicked()
{
	/* stop 버튼 클릭시 reset 버튼만 활성화 */
	ui->CaptureResetPushButton->setEnabled(true);
	ui->CaptureReadyPushButton->setEnabled(false);
	ui->CaptureStartPushButton->setEnabled(false);
	ui->CaptureStopPushButton->setEnabled(false);

	ui->PanoCheckBox->isCheckable();
	ui->CephCheckBox->isCheckable();

	m_modelController->stop();

	m_rawImageViewer->stopPanoTimer();
	m_rawImageViewer->stopCephTimer();
}

void MainWindow::slot_panoImage(QImage* pImg)
{
	qDebug() << __FUNCTION__;
	QImage pano_Image(*pImg);
	QPixmap panoPix;
	panoPix = QPixmap::fromImage(pano_Image, Qt::AutoColor);

	/* 파노라마 이미지가 90도 회전되어 있으므로, 출력시 원상복구한다 */
	QTransform panoTransform;
	panoTransform.rotate(90);

	ui->PanoLabel->setPixmap(panoPix.transformed(panoTransform));

	/* 파노라마 Raw Image 전송상태를 표시해주는 ProgressBar */
	int panoValue = ui->PanoProgressBar->value();
	panoValue++;
	ui->PanoProgressBar->setValue(panoValue);
	ui->PanoProgressBar->update();
	m_fileTransfer->sendPanoFile(panoValue);
	// Progress 값을 통해 Pano Module 회전 
	m_modelController->on_Rotate_PanoObject(panoValue);
}

void MainWindow::slot_cephImage(QImage* cImg)
{
	qDebug() << __FUNCTION__;
	QImage ceph_Image(*cImg);
	QPixmap cephPix;
	cephPix = QPixmap::fromImage(ceph_Image, Qt::AutoColor);
	ui->CephLabel->setPixmap(cephPix);

	/* 세팔로 Raw Image 전송상태를 표시해주는 ProgressBar */
	int cephValue = ui->CephProgressBar->value();
	cephValue++;
	ui->CephProgressBar->setValue(cephValue);
	m_fileTransfer->sendCephFile(cephValue);
	// Progress 값을 통해 Ceph Module 이동
	m_modelController->on_Translate_CephObject(cephValue);
}
