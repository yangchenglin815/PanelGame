#include "view.h"
#include <QPainter>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsScene>

View::View(QGraphicsRectItem *parent) :QGraphicsRectItem(parent)
{
    // 画笔 - 边框色
    QPen p = pen();
    p.setWidth(2);
    p.setColor(QColor(0, 160, 230));

    setPen(p);
    // 画刷 - 背景色
    setBrush(QColor(247, 160, 57));

    setFlags(QGraphicsItem::ItemIsSelectable | QGraphicsItem::ItemIsMovable);
}

View::~View()
{

}

void View::mouseMoveEvent(QGraphicsSceneMouseEvent *ev)
{
    scene()->collidingItems(this).isEmpty() ?  setBrush(QBrush(QColor(247, 160, 57))) :
                                               setBrush(QBrush(QColor(255,215,0)));
    QGraphicsItem::mouseMoveEvent(ev);
}

