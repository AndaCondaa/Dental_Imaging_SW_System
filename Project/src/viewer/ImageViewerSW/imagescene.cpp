#include "imagescene.h"
#include <QMouseEvent>
#include <QScrollBar>
#include <QWheelEvent>
#include <QTimeLine>
#include <QPinchGesture>
#include <QGraphicsItem>

#define ItemWidth   5

ImageScene::ImageScene(QWidget *parent)
    : QGraphicsScene(parent)
{
    QPainterPath path;
    m_pathItem = addPath(path);

    //펜의 색상, 두께의 초깃값 설정
    m_penColor = Qt::black;
    m_penThickness = 10;

    //그리기 타입 초깃값 설정(일반 선 그리기)
    m_drawType = Lines;
    m_currentItem = nullptr;
    point = 0;
    update();

}

void ImageScene::ReceiveType(int type)
{
    //각도를 위한 삼각형 데이터는 다른 버튼 클릭 시 바로 삭제
    if(!m_itemList.empty()){
        foreach(auto item, m_itemList)
            removeItem(item);
        update();
    }
    m_itemList.clear();


    if(type == 0){
        m_drawType = Lines;
    }

    else if(type == 1){
        m_drawType = FreeHand;
    }

    else if(type == 2){
        m_drawType = Laser;
        tmp = 0;
    }

    else if(type == 3){
        m_drawType = Cursor;
    }

    else if(type == 4){
        m_drawType = Ellipse;
    }

    else if(type == 5){
        m_drawType = Rectangle;
    }

    else if(type == 6){
        m_drawType = Text;
    }

    //Delete : 선택된 아이템 삭제
    else if(type == 7){
        m_drawType = Delete;
        foreach(auto item, selectedItems())
            removeItem(item);
        update();
    }

    //Clear : 원본을 눌렀을 때 저장되어있던 원, 사각형, 텍스트 데이터 삭제
    else if(type == 8){
        m_ellipseList.clear();
        m_rectList.clear();
        m_textList.clear();
    }

    //Ceph : 길이 측정
    else if(type == 9){
        m_drawType = Ceph;
    }

    //Pano : 길이 측정
    else if(type == 10){
        m_drawType = Pano;
    }

    else if(type == 11){
        m_drawType = Angle;
    }

    qDebug() << "ImageView : " << type;

    //원본을 누르지 않았으면 다시 이동이 불가하도록 하고 선택만 가능하도록 설정
    if(!m_ellipseList.empty()){
        foreach(auto item, m_ellipseList) {
            item->setFlag(QGraphicsItem::ItemIsMovable, false);
            item->setFlag(QGraphicsItem::ItemIsSelectable, true);
        }
    }

    //원본을 누르지 않았으면 다시 이동이 불가하도록 하고 선택만 가능하도록 설정
    if(!m_rectList.empty()){
        foreach(auto item, m_rectList) {
            item->setFlag(QGraphicsItem::ItemIsMovable, false);
            item->setFlag(QGraphicsItem::ItemIsSelectable, true);
        }
    }

    //원본을 누르지 않았으면 다시 이동이 불가하도록 하고 선택만 가능하도록 설정
    if(!m_textList.empty()){
        foreach(auto item, m_textList) {
            item->setFlag(QGraphicsItem::ItemIsMovable, false);
            item->setFlag(QGraphicsItem::ItemIsSelectable, true);
        }
    }
}

void ImageScene::ReceiveText(QString Text)
{
    inputText = Text;
}

void ImageScene::ReceiveBrushColor(QColor paintColor)
{
    m_penColor = paintColor;
}

void ImageScene::ReceiveThickness(int Thickness)
{
    m_penThickness = Thickness;
}

void ImageScene::ReceiveLength(int origWidth, int origHeight, int sceneWidth, int sceneHeight)
{
    xRate =  (double)origWidth / sceneWidth;
    yRate =  (double)origHeight / sceneHeight;
}

void ImageScene::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    qDebug("mousePressEvent");
    //    if(items(event->scenePos()).isEmpty()) {
    if(m_drawType == Lines) {
        m_isDrawable = true;

        QPainterPath path;
        QGraphicsPathItem* item = addPath(path, QPen(m_penColor, m_penThickness,
                                                     Qt::SolidLine, Qt::RoundCap));
        path = item->path();
        path.moveTo(event->scenePos());
        item->setPath(path);
        m_pathList.append(item);
    }

    else if(m_drawType == Laser){

    }

    else if(m_drawType == FreeHand){
        m_isDrawable = true;
        m_startPos = event->scenePos();

        QPainterPath path;
        QGraphicsPathItem* item = addPath(path, QPen(m_penColor, m_penThickness,
                                                     Qt::SolidLine, Qt::RoundCap));
        path = item->path();
        path.moveTo(event->scenePos());
        item->setPath(path);
        m_pathList.append(item);
    }

    else if(m_drawType == Ellipse){
        m_startPos = event->scenePos();
    }

    else if(m_drawType == Rectangle){
        m_startPos = event->scenePos();
    }

    else if(m_drawType == Text) {
        m_startPos = event->scenePos();
    }

    else if(m_drawType == Ceph) {
        m_startPos = event->scenePos();
    }

    else if(m_drawType == Pano) {
        m_startPos = event->scenePos();
    }

    else if(m_drawType == Angle){
        switch (point) {
        case 0: {

            if(!m_itemList.empty()){
                foreach(auto item, m_itemList)
                    removeItem(item);
                update();
            }
            m_itemList.clear();

            first = event->scenePos();
            QGraphicsEllipseItem* item1 = new QGraphicsEllipseItem();
            item1->setBrush(QBrush(QColor(m_penColor), Qt::SolidPattern));
            item1->setRect(first.x()-ItemWidth/2, first.y()-ItemWidth/2, ItemWidth, ItemWidth);
            item1->setTransformOriginPoint(ItemWidth/2, ItemWidth/2);
            addItem(item1);
            m_itemList.append(item1);
            break;
        }

        case 1: {
            second = event->scenePos();
            QGraphicsEllipseItem* item1 = new QGraphicsEllipseItem();
            item1->setBrush(QBrush(QColor(m_penColor), Qt::SolidPattern));
            item1->setRect(second.x()-ItemWidth/2, second.y()-ItemWidth/2, ItemWidth, ItemWidth);
            item1->setTransformOriginPoint(ItemWidth/2, ItemWidth/2);
            addItem(item1);
            m_itemList.append(item1);

            QPainterPath path(first);
            path.quadTo(first, second);
            QGraphicsPathItem* line = addPath(path, QPen(Qt::yellow, 2,
                                                         Qt::SolidLine, Qt::RoundCap));
            m_itemList.append(line);
            break;
        }

        case 2: {
            third = event->scenePos();
            QGraphicsEllipseItem* item1 = new QGraphicsEllipseItem();
            item1->setBrush(QBrush(QColor(m_penColor), Qt::SolidPattern));
            item1->setRect(third.x()-ItemWidth/2, third.y()-ItemWidth/2, ItemWidth, ItemWidth);
            item1->setTransformOriginPoint(ItemWidth/2, ItemWidth/2);

            addItem(item1);
            m_itemList.append(item1);

            QPainterPath path(second);
            path.quadTo(second, third);
            QGraphicsPathItem* line = addPath(path, QPen(Qt::yellow, 2,
                                                         Qt::SolidLine, Qt::RoundCap));
            m_itemList.append(line);
            break;
        }
        }

        point++;
        if(point >= 3)
        {
            double A = sqrt(pow(first.x() - third.x(), 2) + pow(first.y() - third.y(), 2));
            double B = sqrt(pow(first.x() - second.x(), 2) + pow(first.y() - second.y(), 2));
            double C = sqrt(pow(second.x() - third.x(), 2) + pow(second.y() - third.y(), 2));
            double Degree = acos((pow(B, 2) + pow(C, 2) - pow(A, 2)) / (2 * B * C));
            Degree = Degree * (180 / M_PI);
            emit SendMeasurement("Angle", Degree);
            point = 0;
        }
    }

    QGraphicsScene::mousePressEvent(event);
}

void ImageScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    qDebug("mouseReleaseEvent");
    Q_UNUSED(event);

    if(m_currentItem != nullptr)
        delete m_currentItem;

    switch (m_drawType) {

    case Cursor:
        break;

    case Rectangle:
        addRectItem(m_startPos, event->scenePos());
        break;

    case Ellipse:
        addEllipseItem(m_startPos, event->scenePos());
        break;

    case Text:
        addTextItem(m_startPos);
        break;

    case Lines: {
        if(m_isDrawable) {
            QGraphicsPathItem* item = m_pathList.last();
            if(item) {
                QPainterPath path = item->path();
                path.lineTo(event->scenePos());
                item->setPath(path);
            }
            m_isDrawable = false;
        }
        break;
    }

    case Laser:
        break;

    case FreeHand: {
        if(m_isDrawable) {
            QGraphicsPathItem* item = m_pathList.last();
            if(item) {
                QPainterPath path = item->path();
                path.lineTo(event->scenePos());
                item->setPath(path);
            }
            m_endPos = event->scenePos();
            QPainterPath path2(m_endPos);
            path2.quadTo(m_endPos, m_startPos);
            addPath(path2,  QPen(m_penColor, m_penThickness,
                                 Qt::SolidLine, Qt::RoundCap));
            m_isDrawable = false;
        }
    }
        break;

    default:
        break;
    }

    m_currentItem = nullptr;
    QGraphicsScene::mouseReleaseEvent(event);
}

void ImageScene::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    qDebug("mouseMoveEvent");

    switch (m_drawType) {
    case Lines: {
        if(m_isDrawable) {
            QGraphicsPathItem* item = m_pathList.last();
            if(item) {
                QPainterPath path = item->path();
                path.lineTo(event->scenePos());
                item->setPath(path);
            }
        }
    }
        break;

    case Laser:
        break;


    case FreeHand: {
        if(m_isDrawable) {
            QGraphicsPathItem* item = m_pathList.last();
            if(item) {
                QPainterPath path = item->path();
                path.lineTo(event->scenePos());
                item->setPath(path);
            }
        }
    }
        break;

    case Rectangle: {
        if(m_currentItem != nullptr)
            delete m_currentItem;
        QRectF rect(m_startPos, event->scenePos());
        QGraphicsRectItem *item = new QGraphicsRectItem(rect);
        item->setPen(QPen(QColor(Qt::yellow), 2));
        addItem(item);
        m_currentItem = item;
    }
        break;

    case Ellipse: {
        if(m_currentItem != nullptr)
            delete m_currentItem;
        QRectF rect(m_startPos, event->scenePos());
        QGraphicsRectItem *item = new QGraphicsRectItem(rect);
        item->setPen(QPen(QColor(Qt::yellow), 2));
        addItem(item);
        m_currentItem = item;
    }
        break;

    case Text: {
            if(m_currentItem != nullptr)
                delete m_currentItem;
            QRectF rect(m_startPos, event->scenePos());
            QGraphicsRectItem *item = new QGraphicsRectItem(rect);
            item->setPen(QPen(QColor(Qt::yellow), 2));
            addItem(item);
            m_currentItem = item;
        }
        break;

    case Ceph: {
        if(m_currentItem != nullptr)
            delete m_currentItem;
        QLineF line(m_startPos, event->scenePos());
        QGraphicsLineItem *item = new QGraphicsLineItem(line);
        item->setPen(QPen(QColor(Qt::red), 1));
        addItem(item);
        m_currentItem = item;

        double length = qSqrt(qPow((m_startPos.x() - event->scenePos().x())*xRate, 2)
                               + qPow((m_startPos.y() - event->scenePos().y())*yRate, 2)) * 0.08733;
        emit SendMeasurement("Ceph", length);
    }
        break;

    case Pano: {
        if(m_currentItem != nullptr)
            delete m_currentItem;
        QLineF line(m_startPos, event->scenePos());
        QGraphicsLineItem *item = new QGraphicsLineItem(line);
        item->setPen(QPen(QColor(Qt::red), 1));
        addItem(item);
        m_currentItem = item;

        double length = qSqrt(qPow((m_startPos.x() - event->scenePos().x())*xRate, 2)
                               + qPow((m_startPos.y() - event->scenePos().y())*yRate, 2)) * 0.096358;
        emit SendMeasurement("Pano", length);
    }
        break;

    case Cursor: {
        //커서 모드일 때 선택된 원형 이동
        foreach(auto item, m_ellipseList) {
            if(item->isSelected()){
                item->setFlags(QGraphicsItem::ItemIsMovable);
            }
        }

        //커서 모드일 때 선택된 사각형 이동
        foreach(auto item, m_rectList) {
            if(item->isSelected()){
                item->setFlags(QGraphicsItem::ItemIsMovable);
            }
        }

        //커서 모드일 때 선택된 텍스트 이동
        foreach(auto item, m_textList) {
            if(item->isSelected()){
                item->setFlags(QGraphicsItem::ItemIsMovable);
            }
        }
    }
        break;

    default:
        break;
    }
    QGraphicsScene::mouseMoveEvent(event);
}


void ImageScene::addEllipseItem(QPointF stPos, QPointF edPos)
{
    QRectF rect(stPos, edPos);
    QGraphicsEllipseItem *ellipseItem = new QGraphicsEllipseItem(rect);
    ellipseItem->setBrush(m_penColor);
    ellipseItem->setBrush(Qt::NoBrush);
    ellipseItem->setFlags(QGraphicsItem::ItemIsSelectable);
    ellipseItem->setPen(QPen(ellipseItem->isSelected() ? Qt::red : m_penColor, ellipseItem->isSelected()? 3 : m_penThickness));
    addItem(ellipseItem);
    m_ellipseList.append(ellipseItem);
}

void ImageScene::addRectItem(QPointF stPos, QPointF edPos)
{
    QRectF rect(stPos, edPos);
    QGraphicsRectItem *rectItem = new QGraphicsRectItem(rect);
    rectItem->setFlags(QGraphicsItem::ItemIsSelectable);
    rectItem->setPen(QPen(rectItem->isSelected()?Qt::red : m_penColor, rectItem->isSelected()?3:m_penThickness));
    addItem(rectItem);
    m_rectList.append(rectItem);
}

void ImageScene::addTextItem(QPointF stPos)
{
    QGraphicsSimpleTextItem *textItem = new QGraphicsSimpleTextItem(inputText);
    QFont newFont("Courier", m_penThickness, QFont::Bold, false);
    textItem->setFlags(QGraphicsItem::ItemIsSelectable);
    textItem->setPos(stPos);
    textItem->setFont(newFont);
    textItem->setBrush(m_penColor);
    addItem(textItem);
    m_textList.append(textItem);
}

//void ImageScene::wheelEvent(QGraphicsSceneWheelEvent *event)
//{
//    qDebug("wheelEvent");
//    int numDegrees = /*event->angleDelta().y() / */8;
//    int numSteps = numDegrees / 15; // see QWheelEvent documentation
//    _numScheduledScalings += numSteps;
//    if (_numScheduledScalings * numSteps < 0) // if user moved the wheel in another direction, we reset previously scheduled scalings
//        _numScheduledScalings = numSteps;

//    QTimeLine *anim = new QTimeLine(350, this);
//    anim->setUpdateInterval(20);

//    connect(anim, SIGNAL (valueChanged(qreal)), SLOT (scalingTime(qreal)));
//    connect(anim, SIGNAL (finished()), SLOT (animFinished()));
//    anim->start();
//}

//void ImageScene::scalingTime(qreal x)
//{
//    Q_UNUSED(x);
//    qreal factor = 1.0 + qreal(_numScheduledScalings) / 300.0;
////    scale(factor, factor);
//}

//void ImageScene::animFinished()
//{
//    (_numScheduledScalings > 0)?_numScheduledScalings--:_numScheduledScalings++;
//    //delete sender();     //    sender()->~QObject();
//}

//bool ImageScene::event(QEvent *event)
//{
//    if (event->type() == QEvent::Gesture)
//        return gestureEvent(static_cast<QGestureEvent*>(event));
//    return QGraphicsScene::event(event);
//}

//bool ImageScene::gestureEvent(QGestureEvent *event)
//{
//    qDebug("gestureEvent");
//    if (QGesture *pinch = event->gesture(Qt::PinchGesture))
//        pinchTriggered(static_cast<QPinchGesture *>(pinch));
//    return true;
//}

//void ImageScene::pinchTriggered(QPinchGesture *gesture)
//{
//    qDebug("pinchTriggered");
//    QPinchGesture::ChangeFlags changeFlags = gesture->changeFlags();
//    if (changeFlags & QPinchGesture::RotationAngleChanged) {
//        qreal rotationDelta = gesture->rotationAngle() - gesture->lastRotationAngle();
//        _rotationAngle += rotationDelta;
//    }
//    if (changeFlags & QPinchGesture::ScaleFactorChanged) {
//        _currentStepScaleFactor = gesture->totalScaleFactor();
//    }
//    if (gesture->state() == Qt::GestureFinished) {
//        _scaleFactor *= _currentStepScaleFactor;
//        _currentStepScaleFactor = 1;
//    }
//    update();
//}
