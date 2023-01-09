#ifndef IMAGEMANAGER_H
#define IMAGEMANAGER_H

#include <QWidget>

namespace Ui {
class ImageManager;
}

class ImageManager : public QWidget
{
    Q_OBJECT

public:
    explicit ImageManager(QWidget *parent = nullptr);
    ~ImageManager();

private:
    Ui::ImageManager *ui;
};

#endif // IMAGEMANAGER_H
