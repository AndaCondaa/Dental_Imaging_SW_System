#ifndef IMAGEVIEW_H
#define IMAGEVIEW_H

#include <QGraphicsView>

class QPinchGesture;
class QGestureEvent;

class ImageView : public QGraphicsView
{
    Q_OBJECT
public:
    explicit ImageView(QWidget *parent = 0);
    QGraphicsScene* graphicsScene;
    QGraphicsView* imageView;

protected:
    void mouseMoveEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
    void mousePressEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
    void mouseReleaseEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
    void wheelEvent(QWheelEvent *event) Q_DECL_OVERRIDE;
    bool event(QEvent *event) Q_DECL_OVERRIDE;

private:
    void pinchTriggered(QPinchGesture *gesture);
    bool gestureEvent(QGestureEvent *event);

    bool _pan;
    int _panStartX, _panStartY;
    int _numScheduledScalings;
    int _currentStepScaleFactor;
    int _scaleFactor, _rotationAngle;

private slots:
    void scalingTime(qreal);
    void animFinished();
};

#endif // IMAGEVIEW_H
