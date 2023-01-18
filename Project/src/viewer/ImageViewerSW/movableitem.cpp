#include "movableitem.h"

#include <QGraphicsSceneMouseEvent>
#include <QBrush>

MovableItem::MovableItem(QGraphicsItem *parent) : QGraphicsEllipseItem(parent), m_isMovable(false)
{
    QBrush brush(QColor(Qt::black), Qt::SolidPattern);
    setBrush(brush);
    setRect(0, 0, 16, 16);
}

void MovableItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    qDebug() << "mousePressEvent";
    m_isMovable = true;
    m_offset = event->pos();
    setPos(mapToScene(event->pos().rx()-8, event->pos().ry()-8));
    QGraphicsItem::mousePressEvent(event);
    event->accept();
}

void MovableItem::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    qDebug() << "mouseMoveEvent";
    if(m_isMovable) {
        setPos(mapToScene(event->pos().rx()-8, event->pos().ry()-8));
    }
    QGraphicsItem::mouseMoveEvent(event);
    event->ignore();
}

void MovableItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    qDebug() << "mouseReleaseEvent";
    if(event->button() == Qt::LeftButton && m_isMovable) {
        m_isMovable = false;
        update();
    }
    QGraphicsItem::mouseReleaseEvent(event);
    event->ignore();
}

