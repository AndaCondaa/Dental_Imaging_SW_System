#include <QSplitter>
#include <QListWidget>
#include <QDir>
#include <QFileInfo>

//#include "imageview.h"
#include "album.h"

#include <QGraphicsScene>


Album::Album(QWidget *parent) : QSplitter(parent)
{
    QPalette palette;
    palette.setColor(QPalette::Window, Qt::white);
    palette.setColor(QPalette::WindowText, Qt::blue);

    graphicsScene = new QGraphicsScene;

//    imageView = new ImageView(this);
//    imageView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
//    imageView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
//    imageView->setScene(graphicsScene);
//    imageView->grabGesture(Qt::PinchGesture);

    listWidget = new QListWidget(this);
    listWidget->setViewMode(QListWidget::IconMode);
    listWidget->setIconSize(QSize(120, 80));
    listWidget->setResizeMode(QListWidget::Adjust);
    listWidget->setFlow(QListWidget::TopToBottom);
    listWidget->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    connect(listWidget, SIGNAL(itemClicked(QListWidgetItem*)), SLOT(selectItem(QListWidgetItem*)));

    setOrientation(Qt::Horizontal);
//    addWidget(imageView);
    addWidget(listWidget);
    setStretchFactor(0, 3);

    QList<int> list;
    list << 520 << 100;
    setSizes(list);

    reloadImages();
}

void Album::reloadImages()
{
    QDir dir(".");
    QStringList filters;
    filters << "*.png" << "*.jpg" << "*.bmp";
    QFileInfoList fileInfoList = dir.entryInfoList(filters, QDir::Files | QDir::NoDotAndDotDot);
    graphicsScene->clear();

    listWidget->clear();
    for(int i = 0; i < fileInfoList.count(); i++) {
        QListWidgetItem* item = new QListWidgetItem(QIcon(fileInfoList.at(i).fileName()), NULL, listWidget); //, QListWidgetItem::UserType);
        item->setStatusTip(fileInfoList.at(i).fileName());
        listWidget->addItem(item);
    };
}

void Album::selectItem(QListWidgetItem* item)
{
//    imageView->resetTransform();
    graphicsScene->clear();
    graphicsScene->addPixmap(QPixmap(item->statusTip()).scaled(640, 480, Qt::KeepAspectRatio, Qt::SmoothTransformation));
}

void Album::setImage(QString path)
{
//    imageView->resetTransform();
    graphicsScene->clear();
    graphicsScene->addPixmap(QPixmap(path).scaled(640, 480, Qt::KeepAspectRatio, Qt::SmoothTransformation));

    QFileInfo fileInfo(path);
    for(int i = 0; i < listWidget->count(); i++) {
        if(listWidget->item(i)->statusTip() == fileInfo.fileName()) {
            listWidget->setCurrentRow(i);
            break;
        }
    }
}

QString Album::currentImage()
{
    return (listWidget->currentRow() >=0)?listWidget->currentItem()->statusTip():"";
}

void Album::reset()
{
//    imageView->resetTransform();
}
