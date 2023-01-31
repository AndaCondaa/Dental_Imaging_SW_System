#ifndef IMAGESCENE_H
#define IMAGESCENE_H

#include <QGraphicsView>
#include <QGraphicsItem>
#include <QGraphicsSceneMouseEvent>
#include <QWidget>
#include <QBasicTimer>

class QPinchGesture;
class QGestureEvent;
class MovableItem;


class ImageScene : public QGraphicsScene
{
    Q_OBJECT
public:
    explicit ImageScene(QWidget *parent = 0);

public:
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) Q_DECL_OVERRIDE;
    void mousePressEvent(QGraphicsSceneMouseEvent *event) Q_DECL_OVERRIDE;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) Q_DECL_OVERRIDE;
//    void wheelEvent(QGraphicsSceneWheelEvent *event) Q_DECL_OVERRIDE;
//    bool event(QEvent *event) Q_DECL_OVERRIDE;

    enum DrawType {
        Lines,
        FreeHand,
        Laser,
        Cursor,
        Ellipse,
        Rectangle,
        Text,
        Delete,
        Clear,
        Ceph,
        Pano,
        Angle,
        Copy,
        Paste
    };

private:
//    void pinchTriggered(QPinchGesture *gesture);
//    bool gestureEvent(QGestureEvent *event);
     bool m_isDrawable;
     DrawType m_drawType;
     QColor m_penColor;
     int m_penThickness;
     QList<QGraphicsPathItem*> m_pathList;
     QList<QGraphicsPathItem*> m_laserList;
     QList<QGraphicsEllipseItem*> m_ellipseList;
     QList<QGraphicsRectItem*> m_rectList;
     QList<QGraphicsSimpleTextItem*> m_textList;


     QGraphicsPathItem* m_pathItem;
     QList<QGraphicsItem*> m_itemList;
     QGraphicsItem* m_currentItem;
     QPointF first;
     QPointF second;
     QPointF third;
     QPointF m_startPos;
     QPointF m_endPos;
     int tmp;
     int point;
     QString inputText;

     void addEllipseItem(QPointF, QPointF);
     void addRectItem(QPointF, QPointF);
     void addTextItem(QPointF);

     double xRate, yRate;
     QTimer* timer;


private slots:
    void ReceiveBrushColor(QColor);
    void ReceiveThickness(int);
    void ReceiveType(int);
    void ReceiveText(QString);
    void ReceiveLength(int, int, int, int);

signals:
    void SendMeasurement(QString, double);
};


#endif // IMAGESCENE_H
