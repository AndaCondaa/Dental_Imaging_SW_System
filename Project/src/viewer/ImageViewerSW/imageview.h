#ifndef IMAGEVIEW_H
#define IMAGEVIEW_H

#include <QGraphicsView>
#include <QGraphicsItem>
#include <QGraphicsSceneMouseEvent>
#include <QWidget>
#include <QBasicTimer>

class QPinchGesture;
class QGestureEvent;
class ImageAlbum;
class MovableItem;
class ImageScene;


class ImageView : public QGraphicsView
{
    Q_OBJECT
public:
    explicit ImageView(QWidget *parent = 0);

public:
    void mouseMoveEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
    void mousePressEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
    void mouseReleaseEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
    void wheelEvent(QWheelEvent *event) Q_DECL_OVERRIDE;
    bool event(QEvent *event) Q_DECL_OVERRIDE;
    ImageScene* graphicsScene;

private:
    void pinchTriggered(QPinchGesture *gesture);
    bool gestureEvent(QGestureEvent *event);

    bool _pan;
    int _panStartX, _panStartY;
    int _numScheduledScalings;
    int _currentStepScaleFactor;
    int _scaleFactor, _rotationAngle;
    QPointF startPos;
    QGraphicsItem * lastRect;
    QColor m_penColor;
    int m_penThickness;
    int m_drawType;


    QPointF A, B, C;
    int count;
    QGraphicsEllipseItem* first;
    QGraphicsEllipseItem* second;
    QGraphicsEllipseItem* third;
    QPainterPath *path1;




private slots:
    void scalingTime(qreal);
    void animFinished();

    void ReceiveBrushColor(QColor);
    void ReceiveThickness(int);
    void ReceiveType(int);
};

#endif // IMAGEVIEW_H
