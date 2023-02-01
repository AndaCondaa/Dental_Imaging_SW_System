#ifndef CBCTMODELCONTROLLER_H
#define CBCTMODELCONTROLLER_H

#include <QObject>


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class GeometryDataType {
public:
	enum Type {
		Lower = 0,
		Upper,
		Panorama,
		Cephalo,
	};

	static QString toString(int type) {
		switch (type) {
		case GeometryDataType::Lower:
			return "Lower";
		case GeometryDataType::Upper:
			return "Upper";
		case GeometryDataType::Panorama:
			return "Panorama";
		case GeometryDataType::Cephalo:
			return "Cephalo";
		}

		return "Error";
	}

	static int toEnum(const QString& type) {
		if (type == "Lower") {
			return GeometryDataType::Lower;
		}
		else if (type == "Upper") {
			return GeometryDataType::Upper;
		}
		else if (type == "Panorama") {
			return GeometryDataType::Panorama;
		}
		else if (type == "Cephalo") {
			return GeometryDataType::Cephalo;
		}
		return -1;
	}
};

class GeometryViewType {
public:
	enum Type {
		All = 0,
		Main,
		Sub,
	};

	static QString toString(int type) {
		switch (type) {
		case GeometryViewType::All:
			return "All";
		case GeometryViewType::Main:
			return "Main";
		case GeometryViewType::Sub:
			return "Sub";

		}

		return "Error";
	}

	static int toEnum(const QString& type) {
		if (type == "All") {
			return GeometryViewType::All;
		}
		else if (type == "Main") {
			return GeometryViewType::Main;
		}
		else if (type == "Sub") {
			return GeometryViewType::Sub;
		}
		return -1;
	}
};

class CBCTModelController : public QObject
{
	Q_OBJECT
public:
	CBCTModelController(Ui::MainWindow* parentUI = nullptr);
	~CBCTModelController();

private slots:
	// MainPush Btn : Pano Obj File이 Y Axis 기준으로 360 회전을 한다. 
	void on_MainPushButton_clicked();
	// SubPush Btn : Ceph Obj File이 우측 끝에서 좌측 끝으로 이동 후 원 상태로 복귀 한다. 
	void on_SubPushButton_clicked();
	// 상승 Btn : Lower Body Obj File 만 빼고 Y Axis 기준 +Up Max를 벗어나면 안된다. 
	void on_AscendingPushButton_pressed();
	// 하강 Btn : Lower Body Obj File 만 빼고 Y Axis 기준 -Lo Min을 벗어나면 안된다. 
	void on_DescendingPushButton_pressed();

	void on_CaptureResetPushButton_VTK_clicked();
	void on_CaptureReadyPushButton_VTK_clicked();

public:
	bool initialize();
	void on_Rotate_PanoObject(const int& val);
	void on_Translate_CephObject(const int& val);

private:
	class Internal;
	Internal* PData;
};

#endif // CBCTMODELCONTROLLER_H
