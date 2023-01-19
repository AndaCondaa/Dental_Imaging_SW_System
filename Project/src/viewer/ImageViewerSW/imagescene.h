#ifndef IMAGESCENE_H
#define IMAGESCENE_H

#include <QGraphicsView>
#include <QGraphicsItem>
#include <QGraphicsSceneMouseEvent>
#include <QWidget>
#include <QBasicTimer>

class QPinchGesture;
class QGestureEvent;
class ImageAlbum;
class MovableItem;

enum DrawType {
    Lines,
    FreeHand,
    Triangle
};

class ImageScene : public QGraphicsScene
{
    Q_OBJECT
public:
    explicit ImageScene(QWidget *parent = 0);

public:
//    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) Q_DECL_OVERRIDE;
    void mousePressEvent(QGraphicsSceneMouseEvent *event) Q_DECL_OVERRIDE;
//    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) Q_DECL_OVERRIDE;
//    void wheelEvent(QGraphicsSceneWheelEvent *event) Q_DECL_OVERRIDE;
//    bool event(QEvent *event) Q_DECL_OVERRIDE;
//    QGraphicsScene* graphicsScene;

private:
//    void pinchTriggered(QPinchGesture *gesture);
//    bool gestureEvent(QGestureEvent *event);

//    bool _pan;
//    int _panStartX, _panStartY;
//    int _numScheduledScalings;
//    int _currentStepScaleFactor;
//    int _scaleFactor, _rotationAngle;
//    QPointF startPos;
//    QGraphicsItem * lastRect;
//    QColor m_penColor;
//    int m_penThickness;
//    int m_drawType;


//    QPointF A, B, C;
//    int count;
//    QGraphicsEllipseItem* first;
//    QGraphicsEllipseItem* second;
//    QGraphicsEllipseItem* third;
//    QPainterPath *path1;

//private slots:
//    void scalingTime(qreal);
//    void animFinished();

//    void ReceiveBrushColor(QColor);
//    void ReceiveThickness(int);
//    void ReceiveType(int);
};


#endif // IMAGESCENE_H
