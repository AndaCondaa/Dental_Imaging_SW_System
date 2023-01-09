#ifndef EDITIMAGE_H
#define EDITIMAGE_H

#include <QWidget>
#include <QGraphicsView>

class ImageView;

namespace Ui {
class EditImage;
}

class EditImage : public QWidget
{
    Q_OBJECT

public:
    explicit EditImage(QWidget *parent = nullptr);
    ~EditImage();

private:
    Ui::EditImage *ui;



};

#endif // EDITIMAGE_H
