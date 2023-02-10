#include "imagescene.h"
#include <QMouseEvent>
#include <QScrollBar>
#include <QWheelEvent>
#include <QTimeLine>
#include <QPinchGesture>
#include <QGraphicsItem>
#include <QTimer>

#define DotWidth   5

ImageScene::ImageScene(QWidget *parent)
    : QGraphicsScene(parent)
{
    QPainterPath path;
    m_pathItem = addPath(path);

    //펜의 색상, 두께의 초깃값 설정
    m_penColor = Qt::red;
    m_penThickness = 6;

    //그리기 타입 초깃값 설정(레이저 그리기)
    m_drawType = Laser;
    m_currentItem = nullptr;
    m_fontSize = 10;
    point = 0;
    m_implantImage = "./Icon/ImplantImage.png";
    update();
}

void ImageScene::ReceiveType(int type)
{
    //각도를 위한 삼각형 데이터는 다른 버튼 클릭 시 바로 삭제
    if(!m_angleList.empty()){
        foreach(auto item, m_angleList)
            removeItem(item);
        update();
    }
    m_angleList.clear();

    //길이를 위한 데이터는 다른 버튼 클릭 시 바로 삭제
    if(!m_lengthList.empty()){
        foreach(auto item, m_lengthList)
            removeItem(item);
        update();
    }
    m_lengthList.clear();

    //레이저를 위한 데이터는 다른 버튼 클릭 시 바로 삭제
    if(!m_laserList.empty()){
        foreach(auto item, m_laserList)
            removeItem(item);
        update();
    }
    m_laserList.clear();

    //임플란트를 위한 데이터는 다른 버튼 클릭 시 바로 삭제
    if(!m_implantList.empty()){
        foreach(auto item, m_implantList)
            removeItem(item);
        update();
    }
    m_implantList.clear();
    point = 0;

    if(type == DrawType::Lines){
        m_drawType = Lines;
    }

    else if(type == DrawType::FreeHand){
        m_drawType = FreeHand;
    }

    else if(type == DrawType::Laser){
        m_drawType = Laser;
    }

    else if(type == DrawType::Cursor){
        m_drawType = Cursor;
    }

    else if(type == DrawType::Ellipse){
        m_drawType = Ellipse;
    }

    else if(type == DrawType::Rectangle){
        m_drawType = Rectangle;
    }

    else if(type == DrawType::Text){
        m_drawType = Text;
    }

    //Delete : 선택된 아이템 삭제
    else if(type == DrawType::Delete){
        m_drawType = Delete;
        foreach(auto item, selectedItems())
            removeItem(item);
        update();
    }

    //Clear : 원본을 눌렀을 때 저장되어있던 원, 사각형, 텍스트, 이미지 데이터 삭제
    else if(type == DrawType::Clear){
        m_ellipseList.clear();
        m_rectList.clear();
        m_textList.clear();
        m_angleList.clear();
        m_lengthList.clear();
        m_implantImageList.clear();
        m_pathList.clear();
    }

    //Ceph, Pano 길이 측정 기능
    else if(type == DrawType::Length){
        m_drawType = Length;
    }

    //각도 측정 기능
    else if(type == DrawType::Angle){
        m_drawType = Angle;
    }

    //캡쳐 기능
    else if(type == DrawType::Capture){
        m_drawType = Capture;
    }

    else if(type == DrawType::Implant){
        m_drawType = Implant;
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

    //원본을 누르지 않았으면 다시 이동이 불가하도록 하고 선택만 가능하도록 설정
    if(!m_implantImageList.empty()){
        foreach(auto item, m_implantImageList) {
            item->setFlag(QGraphicsItem::ItemIsMovable, false);
            item->setFlag(QGraphicsItem::ItemIsSelectable, true);
        }
    }

    //원본을 누르지 않았으면 다시 이동이 불가하도록 하고 선택만 가능하도록 설정
    if(!m_pathList.empty()){
        foreach(auto item, m_pathList) {
            item->setFlag(QGraphicsItem::ItemIsMovable, false);
            item->setFlag(QGraphicsItem::ItemIsSelectable, true);
        }
    }
}

void ImageScene::ReceiveFontSize(int size)
{
    m_fontSize = size;
    if(!m_textList.empty()){
        foreach(auto item, m_textList) {
            if(item->isSelected()){
                QFont Font;
                Font.setPointSize(size);
                Font.setBold(true);
                Font.setStyleHint(QFont::Courier);
                item->setFont(Font);
            }
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

    if(!m_textList.empty()){
        foreach(auto item, m_textList) {
            if(item->isSelected()){
                item->setBrush(m_penColor);
            }
        }
    }

    if(!m_ellipseList.empty()){
        foreach(auto item, m_ellipseList) {
            if(item->isSelected()){
                QPen pen(m_penColor, item->pen().width());
                item->setPen(pen);
            }
        }
    }

    if(!m_pathList.empty()){
        foreach(auto item, m_pathList) {
            if(item->isSelected()){
                QPen pen(m_penColor, item->pen().width());
                item->setPen(pen);
            }
        }
    }

    if(!m_rectList.empty()){
        foreach(auto item, m_rectList) {
            if(item->isSelected()){
                QPen pen(m_penColor, item->pen().width(), Qt::SolidLine, Qt::SquareCap, Qt::MiterJoin);
                item->setPen(pen);
            }
        }
    }
}

void ImageScene::ReceiveThickness(int Thickness)
{
    m_penThickness = Thickness;

    if(!m_ellipseList.empty()){
        foreach(auto item, m_ellipseList) {
            if(item->isSelected()){
                QPen pen(item->pen().color(), m_penThickness);
                item->setPen(pen);
            }
        }
    }

    if(!m_pathList.empty()){
        foreach(auto item, m_pathList) {
            if(item->isSelected()){
                QPen pen(item->pen().color(), m_penThickness);
                item->setPen(pen);
            }
        }
    }

    if(!m_rectList.empty()){
        foreach(auto item, m_rectList) {
            if(item->isSelected()){
                QPen pen(item->pen().color(), m_penThickness, Qt::SolidLine, Qt::SquareCap, Qt::MiterJoin);
                item->setPen(pen);
            }
        }
    }
}

void ImageScene::ReceiveLength(int origWidth, int origHeight, int sceneWidth, int sceneHeight, QString type)
{
    xRate =  (double)origWidth / sceneWidth;
    yRate =  (double)origHeight / sceneHeight;
    imageType = type;
}

void ImageScene::ReceiveImplantType(int index)
{
    if(index == 0){
        m_implantImage = "./Icon/ImplantImage.png";
    }

    else {
        m_implantImage = "./Icon/implant3.png";
    }
}

int ImageScene::GetDistance(QPointF pt1, QPointF pt2)
{
    return (float)sqrt(pow((float)(pt1.x() - pt2.x()), 2)
                       + pow((float)(pt1.y() - pt2.y()), 2));
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
        m_isDrawable = true;

        //다른 그리기 수행 시 바로 삭제
        if(!m_laserList.empty()){
            foreach(auto item, m_laserList)
                removeItem(item);
            update();
        }
        m_laserList.clear();

        QPainterPath path;
        QGraphicsPathItem* item = addPath(path, QPen(Qt::red, m_penThickness,
                                                     Qt::DotLine, Qt::RoundCap));
        path = item->path();
        path.moveTo(event->scenePos());
        item->setPath(path);
        m_laserList.append(item);
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

    else if(m_drawType == Length) {

        switch (point) {
        case 0: {
            if(!m_lengthList.empty()){
                foreach(auto item, m_lengthList)
                    removeItem(item);
                update();
            }
            m_lengthList.clear();

            first = event->scenePos();
            QGraphicsEllipseItem* item1 = new QGraphicsEllipseItem();
            item1->setPen(QPen(Qt::black));
            item1->setBrush(QBrush(QColor(Qt::white), Qt::SolidPattern));
            item1->setRect(first.x()-DotWidth/2, first.y()-DotWidth/2, DotWidth, DotWidth);
            item1->setTransformOriginPoint(DotWidth/2, DotWidth/2);
            addItem(item1);
            m_lengthList.append(item1);
            break;
        }

        case 1: {
            second = event->scenePos();
            QGraphicsEllipseItem* item1 = new QGraphicsEllipseItem();
            item1->setPen(QPen(Qt::black));
            item1->setBrush(QBrush(QColor(Qt::white), Qt::SolidPattern));
            item1->setRect(second.x()-DotWidth/2, second.y()-DotWidth/2, DotWidth, DotWidth);
            item1->setTransformOriginPoint(DotWidth/2, DotWidth/2);
            addItem(item1);
            m_lengthList.append(item1);

            QPainterPath path(first);
            path.quadTo(first, second);
            QGraphicsPathItem* line = addPath(path, QPen(Qt::green, 2,
                                                         Qt::SolidLine, Qt::RoundCap));
            m_lengthList.append(line);
            break;
        }
        }

        point++;
        if(point >= 2)
        {
            double length = 0;
                   if(imageType == "CEPH"){
                       length = qSqrt(qPow((first.x() - second.x())*xRate, 2)
                                      + qPow((first.y() - second.y())*yRate, 2)) * 0.08733;
                   }
                   else if(imageType == "PANO"){
                       length = qSqrt(qPow((first.x() - second.x())*xRate, 2)
                                      + qPow((first.y() - second.y())*yRate, 2)) * 0.096358;
                   }

                   lengthItem = new QGraphicsSimpleTextItem();
                   QFont Font;
                   Font.setPointSize(15);
                   Font.setBold(true);
                   Font.setStyleHint(QFont::Courier);
                   lengthItem->setPos(first.x() + 5, first.y() - 10);
                   lengthItem->setFont(Font);
                   lengthItem->setBrush(Qt::green);
                   addItem(lengthItem);
                   m_lengthList.append(lengthItem);

                   QString Result = QString::number(length);
                   lengthItem->setText(Result);

                   emit SendMeasurement(imageType, length);

            point = 0;
        }
    }

    else if(m_drawType == Capture){
        m_startPos = event->scenePos();
    }

    else if(m_drawType == Implant){
        switch (point) {
        case 0: {
            if(!m_implantList.empty()){
                foreach(auto item, m_implantList)
                    removeItem(item);
                update();
            }
            m_implantList.clear();

            first = event->scenePos();
            QGraphicsEllipseItem* item1 = new QGraphicsEllipseItem();
            item1->setPen(QPen(Qt::black));
            item1->setBrush(QBrush(QColor(Qt::red), Qt::SolidPattern));
            item1->setRect(first.x()-DotWidth/2, first.y()-DotWidth/2, DotWidth, DotWidth);
            item1->setTransformOriginPoint(DotWidth/2, DotWidth/2);
            addItem(item1);
            m_implantList.append(item1);
            break;
        }

        case 1: {
            second = event->scenePos();
            QGraphicsEllipseItem* item1 = new QGraphicsEllipseItem();
            item1->setPen(QPen(Qt::black));
            item1->setBrush(QBrush(QColor(Qt::white), Qt::SolidPattern));
            item1->setRect(second.x()-DotWidth/2, second.y()-DotWidth/2, DotWidth, DotWidth);
            item1->setTransformOriginPoint(DotWidth/2, DotWidth/2);
            addItem(item1);
            m_implantList.append(item1);
            break;
        }

        case 2: {
            third = event->scenePos();
            QGraphicsEllipseItem* item1 = new QGraphicsEllipseItem();
            item1->setPen(QPen(Qt::black));
            item1->setBrush(QBrush(QColor(Qt::white), Qt::SolidPattern));
            item1->setRect(third.x()-DotWidth/2, third.y()-DotWidth/2, DotWidth, DotWidth);
            item1->setTransformOriginPoint(DotWidth/2, DotWidth/2);
            addItem(item1);
            m_implantList.append(item1);
            break;
        }
        }

        point++;
        if(point >= 3)
        {
            float delta_x = second.x() - first.x();
            float delta_y = first.y() - second.y();
            float diff_deg = qRadiansToDegrees(atan2f(delta_y, delta_x));
            diff_deg = diff_deg + 90;
            diff_deg = (diff_deg < 0) ? (360 + diff_deg) : diff_deg;

            //lenth1이 밑변의 길이가 되어야 함.
            int height = GetDistance(first, second);
            int lenth2 = GetDistance(second, third);
            int lenth3 = GetDistance(third, first);

            float p = (height+lenth2+lenth3)/2;
            float sss = p*(p-height)*(p-lenth2)*(p-lenth3);

            // 삼각형의 넓이.
            float ss = sqrt(sss);

            //삼각형의 높이
            int width = (int)((ss*2)/height);

            QRectF rect = QRectF(first, QSizeF(width, height)).normalized();
            QPixmap pixmap(m_implantImage);
            QGraphicsPixmapItem *pixItem = new QGraphicsPixmapItem(pixmap.scaled(rect.size().toSize(), Qt::IgnoreAspectRatio));
            QTransform transform;

            pixItem->setPos(first);
            pixItem->setTransformOriginPoint(first);
            pixItem->setFlags(QGraphicsItem::ItemIsSelectable);
            pixItem->setTransform(transform.rotate(-diff_deg));

            double inclination = (second.y() - first.y()) / (second.x() - first.x());
            QPointF vertical_point = QPointF(first.x(), inclination * (third.x() - first.x()) + first.y());

            if((first.x() > second.x()) && (third.y() < vertical_point.y())){
                pixItem->setTransform(transform.scale(-1, 1));
            }

            else if((first.x() < second.x()) && (third.y() > vertical_point.y())){
                pixItem->setTransform(transform.scale(-1, 1));
            }

            addItem(pixItem);
            m_implantImageList.append(pixItem);
            point = 0;
        }
    }

    else if(m_drawType == Angle){
        switch (point) {
        case 0: {

            if(!m_angleList.empty()){
                foreach(auto item, m_angleList)
                    removeItem(item);
                update();
            }
            m_angleList.clear();

            first = event->scenePos();
            QGraphicsEllipseItem* item1 = new QGraphicsEllipseItem();
            item1->setPen(QPen(Qt::black));
            item1->setBrush(QBrush(QColor(Qt::white), Qt::SolidPattern));
            item1->setRect(first.x()-DotWidth/2, first.y()-DotWidth/2, DotWidth, DotWidth);
            item1->setTransformOriginPoint(DotWidth/2, DotWidth/2);
            addItem(item1);
            m_angleList.append(item1);
            break;
        }

        case 1: {
            second = event->scenePos();
            QGraphicsEllipseItem* item1 = new QGraphicsEllipseItem();
            item1->setPen(QPen(Qt::black));
            item1->setBrush(QBrush(QColor(Qt::white), Qt::SolidPattern));
            item1->setRect(second.x()-DotWidth/2, second.y()-DotWidth/2, DotWidth, DotWidth);
            item1->setTransformOriginPoint(DotWidth/2, DotWidth/2);
            addItem(item1);
            m_angleList.append(item1);

            QPainterPath path(first);
            path.quadTo(first, second);
            QGraphicsPathItem* line = addPath(path, QPen(Qt::green, 2,
                                                         Qt::SolidLine, Qt::RoundCap));
            m_angleList.append(line);
            break;
        }

        case 2: {
            third = event->scenePos();
            QGraphicsEllipseItem* item1 = new QGraphicsEllipseItem();
            item1->setPen(QPen(Qt::black));
            item1->setBrush(QBrush(QColor(Qt::white), Qt::SolidPattern));
            item1->setRect(third.x()-DotWidth/2, third.y()-DotWidth/2, DotWidth, DotWidth);
            item1->setTransformOriginPoint(DotWidth/2, DotWidth/2);

            addItem(item1);
            m_angleList.append(item1);

            QPainterPath path(second);
            path.quadTo(second, third);
            QGraphicsPathItem* line = addPath(path, QPen(Qt::green, 2,
                                                         Qt::SolidLine, Qt::RoundCap));
            m_angleList.append(line);
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

            angleItem = new QGraphicsSimpleTextItem();
            QFont Font;
            Font.setPointSize(15);
            Font.setBold(true);
            Font.setStyleHint(QFont::Courier);
            angleItem->setPos(second.x() + 5, second.y() + 5);
            angleItem->setFont(Font);
            angleItem->setBrush(Qt::green);
            addItem(angleItem);
            m_angleList.append(angleItem);

            QString Result = QString::number(Degree);
            angleItem->setText(Result);

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

    case Capture:
        emit SendCapturePos(m_startPos, event->scenePos());
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
    }
        break;

    case Laser: {
        if(m_isDrawable) {
            QGraphicsPathItem* item = m_laserList.last();
            if(item) {
                QPainterPath path = item->path();
                path.lineTo(event->scenePos());
                item->setPath(path);
            }
            m_isDrawable = false;
        }
    }
        break;

    case FreeHand: {
        if(m_isDrawable) {
            QGraphicsPathItem* item = m_pathList.last();
            if(item) {
                QPainterPath path = item->path();
                path.lineTo(event->scenePos());
                path.closeSubpath();
                item->setPath(path);
            }
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

    case Laser: {
        if(m_isDrawable) {
            QGraphicsPathItem* item = m_laserList.last();
            if(item) {
                QPainterPath path = item->path();
                path.lineTo(event->scenePos());
                item->setPath(path);
            }
        }
    }
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
        QRectF rect = QRectF(m_startPos, event->scenePos()).normalized();
        QGraphicsRectItem *item = new QGraphicsRectItem(rect);
        item->setPen(QPen(QColor(Qt::yellow), 2));
        addItem(item);
        m_currentItem = item;
    }
        break;

    case Ellipse: {
        if(m_currentItem != nullptr)
            delete m_currentItem;
        QRectF rect = QRectF(m_startPos, event->scenePos()).normalized();
        QGraphicsEllipseItem *item = new QGraphicsEllipseItem(rect);
        item->setPen(QPen(QColor(Qt::yellow), 2));
        addItem(item);
        m_currentItem = item;
    }
        break;

    case Capture: {
        if(m_currentItem != nullptr)
            delete m_currentItem;
        QRectF rect = QRectF(m_startPos, event->scenePos()).normalized();
        QGraphicsRectItem *item = new QGraphicsRectItem(rect);
        item->setPen(QPen(QColor(Qt::yellow), 2));
        addItem(item);
        m_currentItem = item;
    }
        break;

    case Text: {
            if(m_currentItem != nullptr)
                delete m_currentItem;
            QRectF rect = QRectF(m_startPos, event->scenePos()).normalized();
            QGraphicsRectItem *item = new QGraphicsRectItem(rect);
            item->setPen(QPen(QColor(Qt::yellow), 2));
            addItem(item);
            m_currentItem = item;
        }
        break;

    case Cursor: {
        //커서 모드일 때 선택된 원형 이동
        foreach(auto item, m_ellipseList) {
            if(item->isSelected()){
                item->setFlags(QGraphicsItem::ItemIsMovable| QGraphicsItem::ItemIsSelectable);
            }
        }

        //커서 모드일 때 선택된 사각형 이동
        foreach(auto item, m_rectList) {
            if(item->isSelected()){
                item->setFlags(QGraphicsItem::ItemIsMovable| QGraphicsItem::ItemIsSelectable);
            }
        }

        //커서 모드일 때 선택된 텍스트 이동
        foreach(auto item, m_textList) {
            if(item->isSelected()){
                item->setFlags(QGraphicsItem::ItemIsMovable | QGraphicsItem::ItemIsSelectable);
            }
        }

        //커서 모드일 때 선택된 임플란트 이동
        foreach(auto item, m_implantImageList) {
            if(item->isSelected()){
                item->setFlags(QGraphicsItem::ItemIsMovable | QGraphicsItem::ItemIsSelectable);
            }
        }

        //커서 모드일 때 선택된 곡선 이동
        foreach(auto item, m_pathList) {
            if(item->isSelected()){
                item->setFlags(QGraphicsItem::ItemIsMovable | QGraphicsItem::ItemIsSelectable);
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
    QRectF rect = QRectF(stPos, edPos).normalized();
    QGraphicsEllipseItem *ellipseItem = new QGraphicsEllipseItem(rect);

    QBrush brush(m_penColor, Qt::NoBrush);
    ellipseItem->setBrush(brush);
    ellipseItem->setFlags(QGraphicsItem::ItemIsSelectable);
    ellipseItem->setPen(QPen(m_penColor, m_penThickness));
    addItem(ellipseItem);
    m_ellipseList.append(ellipseItem);
}

void ImageScene::addRectItem(QPointF stPos, QPointF edPos)
{
    QRectF rect = QRectF(stPos, edPos).normalized();
    QGraphicsRectItem *rectItem = new QGraphicsRectItem(rect);
    rectItem->setFlags(QGraphicsItem::ItemIsSelectable);
    rectItem->setPen(QPen(m_penColor, m_penThickness, Qt::SolidLine, Qt::SquareCap, Qt::MiterJoin));
    addItem(rectItem);
    m_rectList.append(rectItem);
}

void ImageScene::addTextItem(QPointF stPos)
{
    QGraphicsSimpleTextItem *textItem = new QGraphicsSimpleTextItem(inputText);
    QFont Font;
    Font.setPointSize(m_fontSize);
    Font.setBold(true);
    Font.setStyleHint(QFont::Courier);

    textItem->setFlags(QGraphicsItem::ItemIsSelectable);
    textItem->setPos(stPos);
    textItem->setFont(Font);
    textItem->setBrush(m_penColor);
    addItem(textItem);
    m_textList.append(textItem);

    m_drawType = Cursor;
}
