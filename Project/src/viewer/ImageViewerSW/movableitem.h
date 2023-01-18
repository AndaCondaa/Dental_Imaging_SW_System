#ifndef MOVABLEITEM_H
#define MOVABLEITEM_H

#include <QGraphicsEllipseItem>

class MovableItem : public QGraphicsEllipseItem
{
public:
    MovableItem(QGraphicsItem *parent = nullptr);

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;

private:
    QPointF m_offset;
    bool m_isMovable;
};

#endif // MOVABLEITEM_H
