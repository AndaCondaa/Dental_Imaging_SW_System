#include <QMouseEvent>
#include <QScrollBar>
#include <QWheelEvent>
#include <QTimeLine>
#include <QPinchGesture>
#include <QGraphicsItem>
#include "imageview.h"
#include "imagescene.h"


ImageView::ImageView(QWidget *parent)
    : QGraphicsView(parent)
{
    graphicsScene = new ImageScene(this);

    _pan = false;
    _currentStepScaleFactor = 1;

    //펜의 색상, 두께의 초깃값 설정
    m_penColor = Qt::black;
    m_penThickness = 10;

    setScene(graphicsScene);
    resetTransform();
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
}

void ImageView::ReceiveType(int type)
{
    m_drawType = type;
    qDebug() << "ImageView : " << m_drawType;
}

void ImageView::ReceiveBrushColor(QColor paintColor)
{
    m_penColor = paintColor;
    qDebug() << "ImageView : " << m_penColor;
}

void ImageView::ReceiveThickness(int Thickness)
{
    m_penThickness = Thickness;
    qDebug() << "ImageView : " << m_penThickness;
}

void ImageView::mousePressEvent(QMouseEvent *event)
{
    qDebug("mousePressEvent");
//    A = mapToScene(event->pos());

    switch (m_drawType) {
    case DrawType::Lines:
        //일반 선 그리기
        if (event->button() == Qt::LeftButton) {
            startPos = mapToScene(event->pos());
            setCursor(Qt::ClosedHandCursor);
            _pan = true;
            event->accept();
            return;
        }
        break;

    case DrawType::FreeHand:
        //닫힌 선 그리기
        if (event->button() == Qt::LeftButton) {
                A = mapToScene(event->pos());
            startPos = mapToScene(event->pos());
            setCursor(Qt::ClosedHandCursor);
            _pan = true;
            event->accept();
            return;
        }
        break;

    case DrawType::Triangle:

        qDebug() << "첫번째 점";
        switch (count) {
        case 0:
            A = mapToScene(event->pos());
            first = graphicsScene->addEllipse(A.x()-5, A.y()-5, 10, 10,
                                              QPen(Qt::NoPen),
                                              QBrush(m_penColor));
            break;

        case 1: {
            B = mapToScene(event->pos());
            second = graphicsScene->addEllipse(B.x()-5, B.y()-5, 10, 10,
                                               QPen(Qt::NoPen),
                                               QBrush(m_penColor));

            /* Draw AB line */
            QPainterPath path(A);
            path.quadTo(A, B);
            graphicsScene->addPath(path, QColorConstants::Svg::red);
        }
            break;
        case 2: {
            C = mapToScene(event->pos());
            third = graphicsScene->addEllipse(C.x()-5, C.y()-5, 10, 10,
                                              QPen(Qt::NoPen),
                                              QBrush(m_penColor));
            /* Draw BC line */
            QPainterPath path(B);
            path.quadTo(B, C);
            graphicsScene->addPath(path,  QColorConstants::Svg::red);

            /* Draw ABBC cubic Bezier curve */
            QPainterPath path2(C);
            path2.quadTo(C, A);

            graphicsScene->addPath(path2,  QColorConstants::Svg::red);
        }
            break;
        default:
            break;
        }
        if (count >= 2) {
            count = 0;
        } else {
            count++;
        }
        break;

    default:
        break;
    }







//    if (event->button() == Qt::LeftButton) {
//        startPos = mapToScene(event->pos());
//        //        graphicsScene->addRect(startPos.x(), startPos.y(),
//        //                               50, 50,
//        //                               QPen(Qt::NoPen),
//        //                               QBrush(m_penColor));
//        setCursor(Qt::ClosedHandCursor);
//        _pan = true;
//        event->accept();
//        return;
//    }

    event->ignore();
}

void ImageView::mouseReleaseEvent(QMouseEvent *event)
{
    qDebug("mouseReleaseEvent");

    switch (m_drawType) {
    case DrawType::Lines:
        //일반 선 그리기
        if(_pan){
            setCursor(Qt::ArrowCursor);
            event->accept();
        }
        break;

    case DrawType::FreeHand:
        //닫힌 선 그리기
        if(_pan){
            setCursor(Qt::ArrowCursor);
            C = mapToScene(event->pos());
            QPainterPath path2(C);
            path2.quadTo(C, A);
            graphicsScene->addPath(path2,  QPen(m_penColor, m_penThickness,
                                                Qt::SolidLine, Qt::RoundCap));
            event->accept();
        }
        break;

    case DrawType::Triangle:
        break;

    default:
        break;
    }




    _pan = false;
    event->ignore();
}

void ImageView::mouseMoveEvent(QMouseEvent *event)
{
    qDebug("mouseMoveEvent");

    this->setDragMode(QGraphicsView::NoDrag);
    QPointF newPos = mapToScene(event->pos());

    switch (m_drawType) {
    case DrawType::Lines:
        //일반 선 그리기
        if(_pan){
            graphicsScene->addLine(startPos.x(), startPos.y(),
                                   newPos.x(), newPos.y(),
                                   QPen(m_penColor, m_penThickness, Qt::SolidLine, Qt::RoundCap));
            event->accept();
        }
        break;

    case DrawType::FreeHand:
        //닫힌 선 그리기
        if(_pan){
            graphicsScene->addLine(startPos.x(), startPos.y(),
                                   newPos.x(), newPos.y(),
                                   QPen(m_penColor, m_penThickness, Qt::SolidLine, Qt::RoundCap));
            event->accept();
        }
        break;

    case DrawType::Triangle:
        break;


    default:
        break;
    }

//    if(_pan){
////        graphicsScene->addLine(startPos.x(), startPos.y(),
////                               newPos.x(), newPos.y(),
////                               QPen(m_penColor, m_penThickness, Qt::SolidLine, Qt::RoundCap));
////        first->setPos(mapToScene(event->pos().x(), event->pos().y()));
//        event->accept();
//    }

    event->ignore();
    startPos = newPos;
}

void ImageView::wheelEvent(QWheelEvent *event)
{
    qDebug("wheelEvent");
    int numDegrees = event->angleDelta().y() / 8;
    int numSteps = numDegrees / 15; // see QWheelEvent documentation
    _numScheduledScalings += numSteps;
    if (_numScheduledScalings * numSteps < 0) // if user moved the wheel in another direction, we reset previously scheduled scalings
        _numScheduledScalings = numSteps;

    QTimeLine *anim = new QTimeLine(350, this);
    anim->setUpdateInterval(20);

    connect(anim, SIGNAL (valueChanged(qreal)), SLOT (scalingTime(qreal)));
    connect(anim, SIGNAL (finished()), SLOT (animFinished()));
    anim->start();
}

void ImageView::scalingTime(qreal x)
{
    Q_UNUSED(x);
    qreal factor = 1.0 + qreal(_numScheduledScalings) / 300.0;
    scale(factor, factor);
}

void ImageView::animFinished()
{
    (_numScheduledScalings > 0)?_numScheduledScalings--:_numScheduledScalings++;
    //delete sender();     //    sender()->~QObject();
}

bool ImageView::event(QEvent *event)
{
    if (event->type() == QEvent::Gesture)
        return gestureEvent(static_cast<QGestureEvent*>(event));
    return QGraphicsView::event(event);
}

bool ImageView::gestureEvent(QGestureEvent *event)
{
    qDebug("gestureEvent");
    if (QGesture *pinch = event->gesture(Qt::PinchGesture))
        pinchTriggered(static_cast<QPinchGesture *>(pinch));
    return true;
}

void ImageView::pinchTriggered(QPinchGesture *gesture)
{
    qDebug("pinchTriggered");
    QPinchGesture::ChangeFlags changeFlags = gesture->changeFlags();
    if (changeFlags & QPinchGesture::RotationAngleChanged) {
        qreal rotationDelta = gesture->rotationAngle() - gesture->lastRotationAngle();
        _rotationAngle += rotationDelta;
    }
    if (changeFlags & QPinchGesture::ScaleFactorChanged) {
        _currentStepScaleFactor = gesture->totalScaleFactor();
    }
    if (gesture->state() == Qt::GestureFinished) {
        _scaleFactor *= _currentStepScaleFactor;
        _currentStepScaleFactor = 1;
    }
    update();
}
