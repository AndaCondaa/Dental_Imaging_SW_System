/*프로그램명 : ImageViewerSW
파일명 : imagescene.cpp
설명 : graphicsview, graphicsscene을 활용하여 원, 사각형, 텍스트, 임플란트 식립, 길이 측정,
각도 측정 등의 기능을 담고있는 클래스
작성자 : 이정연
최종 수정 날짜 : 2023.02.11*/


#include "imagescene.h"
#include <QMouseEvent>
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

    // 펜의 색상, 두께의 초깃값 설정
    m_penColor = Qt::red;
    m_penThickness = 6;

    // 그리기 타입 초깃값 설정(레이저 그리기)
    m_drawType = Cursor;
    m_currentItem = nullptr;
    m_fontSize = 10;
    point = 0;
    m_implantImage = ":/Icon/Icon/ImplantImage.png";
    update();
}

// ImageAlbum 클래스에서 버튼 클릭 시 type 전송
void ImageScene::ReceiveType(int type)
{
    // 각도를 위한 삼각형 데이터는 다른 버튼 클릭 시 바로 삭제
    if(!m_angleList.empty()){
        foreach(auto item, m_angleList)
            removeItem(item);
        update();
    }
    m_angleList.clear();

    // 길이를 위한 데이터는 다른 버튼 클릭 시 바로 삭제
    if(!m_lengthList.empty()){
        foreach(auto item, m_lengthList)
            removeItem(item);
        update();
    }
    m_lengthList.clear();

    // 레이저를 위한 데이터는 다른 버튼 클릭 시 바로 삭제
    if(!m_laserList.empty()){
        foreach(auto item, m_laserList)
            removeItem(item);
        update();
    }
    m_laserList.clear();

    // 임플란트를 위한 데이터는 다른 버튼 클릭 시 바로 삭제
    if(!m_implantList.empty()){
        foreach(auto item, m_implantList)
            removeItem(item);
        update();
    }
    m_implantList.clear();
    point = 0;

    // Lines : 자유곡선
    if(type == DrawType::Lines){
        m_drawType = Lines;
    }

    // FreeHand : 폐곡선
    else if(type == DrawType::FreeHand){
        m_drawType = FreeHand;
    }

    // Laser : 레이저 모드 (그렸던 데이터가 저장되지 않음)
    else if(type == DrawType::Laser){
        m_drawType = Laser;
    }

    // Cursor : 삽입된 도형, 텍스트 이동 모드
    else if(type == DrawType::Cursor){
        m_drawType = Cursor;
    }

    // Ellipse : 원형 삽입
    else if(type == DrawType::Ellipse){
        m_drawType = Ellipse;
    }

    // Rectangle : 사각형 삽입
    else if(type == DrawType::Rectangle){
        m_drawType = Rectangle;
    }

    // Text : 간단한 텍스트 입력
    else if(type == DrawType::Text){
        m_drawType = Text;
    }

    // Delete : 선택된 아이템 삭제
    else if(type == DrawType::Delete){
        m_drawType = Delete;
        foreach(auto item, selectedItems())
            removeItem(item);
        update();
    }

    // Clear : 원본을 눌렀을 때 저장되어있던 원, 사각형, 텍스트, 이미지 데이터 삭제
    else if(type == DrawType::Clear){
        m_ellipseList.clear();
        m_rectList.clear();
        m_textList.clear();
        m_angleList.clear();
        m_lengthList.clear();
        m_implantImageList.clear();
        m_pathList.clear();
    }

    // Length : Ceph, Pano 길이 측정 기능
    else if(type == DrawType::Length){
        m_drawType = Length;
    }

    // Angle : 각도 측정 기능
    else if(type == DrawType::Angle){
        m_drawType = Angle;
    }

    // Capture : 원하는 영역 캡쳐 기능
    else if(type == DrawType::Capture){
        m_drawType = Capture;
    }

    // Implant : 임플란트 식립 기능
    else if(type == DrawType::Implant){
        m_drawType = Implant;
    }

    // 원본을 누르지 않았으면 다시 이동이 불가하도록 하고 선택만 가능하도록 설정
    if(!m_ellipseList.empty()){
        foreach(auto item, m_ellipseList) {
            item->setFlag(QGraphicsItem::ItemIsMovable, false);
            item->setFlag(QGraphicsItem::ItemIsSelectable, true);
        }
    }

    // 원본을 누르지 않았으면 다시 이동이 불가하도록 하고 선택만 가능하도록 설정
    if(!m_rectList.empty()){
        foreach(auto item, m_rectList) {
            item->setFlag(QGraphicsItem::ItemIsMovable, false);
            item->setFlag(QGraphicsItem::ItemIsSelectable, true);
        }
    }

    // 원본을 누르지 않았으면 다시 이동이 불가하도록 하고 선택만 가능하도록 설정
    if(!m_textList.empty()){
        foreach(auto item, m_textList) {
            item->setFlag(QGraphicsItem::ItemIsMovable, false);
            item->setFlag(QGraphicsItem::ItemIsSelectable, true);
        }
    }

    // 원본을 누르지 않았으면 다시 이동이 불가하도록 하고 선택만 가능하도록 설정
    if(!m_implantImageList.empty()){
        foreach(auto item, m_implantImageList) {
            item->setFlag(QGraphicsItem::ItemIsMovable, false);
            item->setFlag(QGraphicsItem::ItemIsSelectable, true);
        }
    }

    // 원본을 누르지 않았으면 다시 이동이 불가하도록 하고 선택만 가능하도록 설정
    if(!m_pathList.empty()){
        foreach(auto item, m_pathList) {
            item->setFlag(QGraphicsItem::ItemIsMovable, false);
            item->setFlag(QGraphicsItem::ItemIsSelectable, true);
        }
    }
}



// lineEdit에서 텍스트 내용을 받아오면 해당 텍스트 내용을 출력
void ImageScene::ReceiveText(QString Text)
{
    inputText = Text;
}

// ColorDialog에서 색상값을 받아오면 선택된 도형이나 텍스트의 색상 변경
void ImageScene::ReceiveBrushColor(QColor paintColor)
{
    m_penColor = paintColor;

    // 색상값을 받아오면 선택된 텍스트 색상 변경
    if(!m_textList.empty()){
        foreach(auto item, m_textList) {
            if(item->isSelected()){
                item->setBrush(m_penColor);
            }
        }
    }

    // 색상값을 받아오면 선택된 원형 색상 변경
    if(!m_ellipseList.empty()){
        foreach(auto item, m_ellipseList) {
            if(item->isSelected()){
                QPen pen(m_penColor, item->pen().width());
                item->setPen(pen);
            }
        }
    }

    // 색상값을 받아오면 선택된 선의 색상 변경
    if(!m_pathList.empty()){
        foreach(auto item, m_pathList) {
            if(item->isSelected()){
                QPen pen(m_penColor, item->pen().width());
                item->setPen(pen);
            }
        }
    }

    // 색상값을 받아오면 선택된 사각형 색상 변경
    if(!m_rectList.empty()){
        foreach(auto item, m_rectList) {
            if(item->isSelected()){
                QPen pen(m_penColor, item->pen().width(), Qt::SolidLine, Qt::SquareCap, Qt::MiterJoin);
                item->setPen(pen);
            }
        }
    }
}

// spinBox에서 펜의 사이즈를 받아오면 해당 도형이나 텍스트의 두께 변경
void ImageScene::ReceiveThickness(int Thickness)
{
    m_penThickness = Thickness;

    // 두께값을 받아오면 선택된 원형의 두께 변경
    if(!m_ellipseList.empty()){
        foreach(auto item, m_ellipseList) {
            if(item->isSelected()){
                QPen pen(item->pen().color(), m_penThickness);
                item->setPen(pen);
            }
        }
    }

    // 두께값을 받아오면 선택된 선의 두께 변경
    if(!m_pathList.empty()){
        foreach(auto item, m_pathList) {
            if(item->isSelected()){
                QPen pen(item->pen().color(), m_penThickness);
                item->setPen(pen);
            }
        }
    }

    // 두께값을 받아오면 선택된 사각형의 두께 변경
    if(!m_rectList.empty()){
        foreach(auto item, m_rectList) {
            if(item->isSelected()){
                QPen pen(item->pen().color(), m_penThickness, Qt::SolidLine, Qt::SquareCap, Qt::MiterJoin);
                item->setPen(pen);
            }
        }
    }

    // spinBox에서 텍스트 폰트 사이즈를 받아오면 해당 텍스트의 폰트 사이즈 변경
    m_fontSize = Thickness;
    if(!m_textList.empty()){
        foreach(auto item, m_textList) {
            if(item->isSelected()){
                QFont Font;
                Font.setPointSize(Thickness);
                Font.setBold(true);
                Font.setStyleHint(QFont::Courier);
                item->setFont(Font);
            }
        }
    }
}

// 이미지 원본 사이즈와 scaled된 이미지 사이즈를 통해 길이 측정
void ImageScene::ReceiveLength(int origWidth, int origHeight, int sceneWidth, int sceneHeight, QString type)
{
    xRate =  (double)origWidth / sceneWidth;
    yRate =  (double)origHeight / sceneHeight;
    imageType = type;
}

// 콤보박스에서 선택한 치아의 종류에 따라 임플란트 이미지 변경
void ImageScene::ReceiveImplantType(int index)
{
    // 어금니 이미지
    if(index == 0){
        m_implantImage = ":/Icon/Icon/ImplantImage.png";
    }

    // 앞니 이미지
    else {
        m_implantImage = ":/Icon/Icon/implant3.png";
    }
}

// 2점 사이의 길이를 반환
int ImageScene::GetDistance(QPointF pt1, QPointF pt2)
{
    return (float)sqrt(pow((float)(pt1.x() - pt2.x()), 2)
                       + pow((float)(pt1.y() - pt2.y()), 2));
}

// 마우스 클릭 시 type에 따라 수행될 함수 결정
void ImageScene::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    qDebug("mousePressEvent");

    // 그리기 모드 시 path 설정 및 List에 추가
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

    // Laser 모드 시 path 설정 및 List에 추가
    else if(m_drawType == Laser){
        m_isDrawable = true;

        // 데이터를 저장하지 않고 바로 삭제
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

    // FreeHand 모드 시 path 설정 및 List에 추가
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

    // 그리기 타입이 원형일 때 처음 좌표 저장
    else if(m_drawType == Ellipse){
        m_startPos = event->scenePos();
    }

    // 그리기 타입이 사각형일 때 처음 좌표 저장
    else if(m_drawType == Rectangle){
        m_startPos = event->scenePos();
    }

    // 그리기 타입이 텍스트일 때 처음 좌표 저장
    else if(m_drawType == Text) {
        m_startPos = event->scenePos();
    }

    // 그리기 타입이 길이 측정일 때
    else if(m_drawType == Length) {

        switch (point) {
        case 0: {
            // 선을 그리고 다음 좌표를 찍을 때 길이 측정 데이터(원, 선, 텍스트) 삭제
            if(!m_lengthList.empty()){
                foreach(auto item, m_lengthList)
                    removeItem(item);
                update();
            }
            m_lengthList.clear();

            // 처음 찍은 포인트 저장 및 원형으로 보여줌
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
            // 두번째 찍은 포인트 저장 및 원형으로 보여줌
            second = event->scenePos();
            QGraphicsEllipseItem* item1 = new QGraphicsEllipseItem();
            item1->setPen(QPen(Qt::black));
            item1->setBrush(QBrush(QColor(Qt::white), Qt::SolidPattern));
            item1->setRect(second.x()-DotWidth/2, second.y()-DotWidth/2, DotWidth, DotWidth);
            item1->setTransformOriginPoint(DotWidth/2, DotWidth/2);
            addItem(item1);
            m_lengthList.append(item1);

            // 처음과 두번째 점을 이어 선을 그려줌
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
            // Ceph일때와 Pano일 때 Detector의 길이와 화면의 비율에 맞게 길이를 측정
            if(imageType == "CEPH"){
                length = qSqrt(qPow((first.x() - second.x())*xRate, 2)
                               + qPow((first.y() - second.y())*yRate, 2)) * 0.08733;
            }
            else if(imageType == "PANO"){
                length = qSqrt(qPow((first.x() - second.x())*xRate, 2)
                               + qPow((first.y() - second.y())*yRate, 2)) * 0.096358;
            }

            // 출력된 길이를 text로 화면에 출력
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
            lengthItem->setText(Result + " mm");

            // 2점이 찍히면 다시 0으로 초기화
            point = 0;
        }
    }

    // 그리기 타입이 캡쳐 모드일 때
    else if(m_drawType == Capture){
        m_startPos = event->scenePos();
    }

    // 그리기 타입이 임플란트 식립일 때
    else if(m_drawType == Implant){

        // 3점의 좌표를 통해 구현
        switch (point) {
        case 0: {
            // 첫 좌표를 찍기 시작하면 기존 임플란트를 위한 원형 데이터 삭제
            if(!m_implantList.empty()){
                foreach(auto item, m_implantList)
                    removeItem(item);
                update();
            }
            m_implantList.clear();

            // 첫 좌표는 임플란트 이미지의 top-left
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
            // 두번째 좌표는 임플란트 이미지의 bottom-left
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
            // 세번째 좌표는 임플란트 이미지의 right, 즉 width를 결정
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
            // 첫 좌표와 두번째 좌표를 연결한 직선이 y축을 기준으로 얼마나 회전했는지 측정
            float delta_x = second.x() - first.x();
            float delta_y = first.y() - second.y();
            float degree = qRadiansToDegrees(atan2f(delta_y, delta_x)) + 90;
            degree = (degree < 0) ? (360 + degree) : degree;

            // height가 밑변의 길이가 되어야 함
            int height = GetDistance(first, second);
            int lenth2 = GetDistance(second, third);
            int lenth3 = GetDistance(third, first);
            float p = (height+lenth2+lenth3)/2;

            // 삼각형의 넓이
            float area = sqrt(p*(p-height)*(p-lenth2)*(p-lenth3));

            // 삼각형의 높이
            int width = (int)((area*2)/height);

            // 첫 좌표로 임플란트가 식립될 top-left 설정, 첫좌표~두번째 좌표의 길이로 높이 설정
            // 첫 좌표와 두번째 좌표를 포함하는 직선과 세번째 좌표에 수직하는 길이를 넓이로 설정
            QRectF rect = QRectF(first, QSizeF(width, height)).normalized();
            QPixmap pixmap(m_implantImage);
            QGraphicsPixmapItem *pixItem = new QGraphicsPixmapItem(pixmap.scaled(rect.size().toSize(), Qt::IgnoreAspectRatio));
            QTransform transform;
            pixItem->setPos(first);
            pixItem->setTransformOriginPoint(first);
            pixItem->setFlags(QGraphicsItem::ItemIsSelectable);
            pixItem->setTransform(transform.rotate(-degree));

            // 첫 좌표와 두번째 좌표를 이은 직선의 기울기
            double inclination = (second.y() - first.y()) / (second.x() - first.x());
            // 세번째 좌표에서 y축 방향으로 직선으로 내려오는 지점의 좌표 구함
            QPointF vertical_point = QPointF(first.x(), inclination * (third.x() - first.x()) + first.y());

            // y좌표 기준 직선의 좌표보다 3번째 좌표가 크면 first 좌표가 top-left가 아니게 되므로
            // 이미지를 뒤집어줌
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

    // 그리기 타입이 각도 측정일 때
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
            angleItem->setText(Result + "°");

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
        // 커서 모드일 때 선택된 원형 이동
        foreach(auto item, m_ellipseList) {
            if(item->isSelected()){
                item->setFlags(QGraphicsItem::ItemIsMovable| QGraphicsItem::ItemIsSelectable);
            }
        }

        // 커서 모드일 때 선택된 사각형 이동
        foreach(auto item, m_rectList) {
            if(item->isSelected()){
                item->setFlags(QGraphicsItem::ItemIsMovable| QGraphicsItem::ItemIsSelectable);
            }
        }

        // 커서 모드일 때 선택된 텍스트 이동
        foreach(auto item, m_textList) {
            if(item->isSelected()){
                item->setFlags(QGraphicsItem::ItemIsMovable | QGraphicsItem::ItemIsSelectable);
            }
        }

        // 커서 모드일 때 선택된 임플란트 이동
        foreach(auto item, m_implantImageList) {
            if(item->isSelected()){
                item->setFlags(QGraphicsItem::ItemIsMovable | QGraphicsItem::ItemIsSelectable);
            }
        }

        // 커서 모드일 때 선택된 곡선 이동
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

// GraphicsScene에 타원형을 그리기 위한 함수
void ImageScene::addEllipseItem(QPointF stPos, QPointF edPos)
{
    // 처음 클릭한 점과 마지막 점의 사이즈에 맞게 원 삽입
    QRectF rect = QRectF(stPos, edPos).normalized();
    QGraphicsEllipseItem *ellipseItem = new QGraphicsEllipseItem(rect);
    QBrush brush(m_penColor, Qt::NoBrush);
    ellipseItem->setBrush(brush);
    ellipseItem->setFlags(QGraphicsItem::ItemIsSelectable);
    ellipseItem->setPen(QPen(m_penColor, m_penThickness));
    addItem(ellipseItem);
    m_ellipseList.append(ellipseItem);
}

// GraphicsScene에 사각형을 그리기 위한 함수
void ImageScene::addRectItem(QPointF stPos, QPointF edPos)
{
    // 처음 클릭한 점과 마지막 점의 사이즈에 맞게 사각형 삽입
    QRectF rect = QRectF(stPos, edPos).normalized();
    QGraphicsRectItem *rectItem = new QGraphicsRectItem(rect);
    rectItem->setFlags(QGraphicsItem::ItemIsSelectable);
    rectItem->setPen(QPen(m_penColor, m_penThickness, Qt::SolidLine, Qt::SquareCap, Qt::MiterJoin));
    addItem(rectItem);
    m_rectList.append(rectItem);
}

// GraphicsScene에 텍스트를 입력하기 위한 함수
void ImageScene::addTextItem(QPointF stPos)
{
    // lineEdit에서 입력한 텍스트를 클릭한 위치에 삽입
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

    // 텍스트 삽입 후 바로 이동할 수 있도록 이동 모드로 설정
    m_drawType = Cursor;
}
