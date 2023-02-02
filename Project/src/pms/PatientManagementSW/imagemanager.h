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

public slots:
    void reloadImages(QString);
    void PSEDataSended(QString);

private:
    Ui::ImageManager *ui;
    QString pid;
};

#endif // IMAGEMANAGER_H
