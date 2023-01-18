#ifndef ALBUM_H
#define ALBUM_H

#include <QSplitter>

class ImageView;
class QGraphicsScene;
class QListWidget;
class QListWidgetItem;

class Album : public QSplitter
{
    Q_OBJECT
public:
    explicit Album(QWidget *parent = 0);
    QString currentImage();

private:
    ImageView* imageView;
    QGraphicsScene* graphicsScene;
    QListWidget* listWidget;

public slots:
    void reset();
    void reloadImages();
    void selectItem(QListWidgetItem*);
    void setImage(QString);
};

#endif // ALBUM_H

