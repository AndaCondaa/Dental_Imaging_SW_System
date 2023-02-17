/*프로그램명 : ImageViewerSW
파일명 : imagescene.h
설명 : graphicsview, graphicsscene을 활용하여 원, 사각형, 텍스트, 임플란트 식립, 길이 측정,
각도 측정 등의 기능을 담고있는 클래스
작성자 : 이정연
최종 수정 날짜 : 2023.02.11*/

#ifndef IMAGESCENE_H
#define IMAGESCENE_H

#include <QGraphicsView>
#include <QGraphicsItem>
#include <QGraphicsSceneMouseEvent>
#include <QWidget>

class QPinchGesture;
class QGestureEvent;
class MovableItem;


class ImageScene : public QGraphicsScene
{
    Q_OBJECT
public:
    explicit ImageScene(QWidget *parent = 0);

    enum DrawType {
        Lines,      //0
        FreeHand,   //1
        Laser,      //2
        Cursor,     //3
        Ellipse,    //4
        Rectangle,  //5
        Text,       //6
        Delete,     //7
        Clear,      //8
        Length,     //9
        Angle,      //11
        Capture,    //12
        Implant     //14
    };

private:
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) Q_DECL_OVERRIDE;
    void mousePressEvent(QGraphicsSceneMouseEvent *event) Q_DECL_OVERRIDE;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) Q_DECL_OVERRIDE;
    void addEllipseItem(QPointF, QPointF);
    void addRectItem(QPointF, QPointF);
    void addTextItem(QPointF);
    int GetDistance(QPointF, QPointF);

    QList<QGraphicsPathItem*> m_pathList;
    QList<QGraphicsPathItem*> m_laserList;
    QList<QGraphicsEllipseItem*> m_ellipseList;
    QList<QGraphicsRectItem*> m_rectList;
    QList<QGraphicsSimpleTextItem*> m_textList;
    QList<QGraphicsItem*> m_angleList;
    QList<QGraphicsItem*> m_implantList;
    QList<QGraphicsItem*> m_implantImageList;
    QList<QGraphicsItem*> m_lengthList;
    QGraphicsSimpleTextItem *lengthItem;
    QGraphicsSimpleTextItem *angleItem;
    QGraphicsPathItem* m_pathItem;
    QGraphicsItem* m_currentItem;
    QPointF first;
    QPointF second;
    QPointF third;
    QPointF m_startPos;
    QString inputText;
    QString imageType;
    QString m_implantImage;
    DrawType m_drawType;
    QColor m_penColor;
    double xRate, yRate;
    bool m_isDrawable;
    int m_penThickness;
    int m_fontSize;
    int point;

private slots:
    void ReceiveLength(int, int, int, int, QString);
    void ReceiveImplantType(int);
    void ReceiveThickness(int);
    void ReceiveType(int);
    void ReceiveBrushColor(QColor);
    void ReceiveText(QString);

signals:
    void SendCapturePos(QPointF, QPointF);
};


#endif // IMAGESCENE_H
