#ifndef IMAGINGMANAGER_H
#define IMAGINGMANAGER_H

#include <QWidget>

namespace Ui {
class ImagingManager;
}

class ImagingManager : public QWidget
{
    Q_OBJECT

public:
    explicit ImagingManager(QWidget *parent = nullptr);
    ~ImagingManager();

private:
    Ui::ImagingManager *ui;
};

#endif // IMAGINGMANAGER_H