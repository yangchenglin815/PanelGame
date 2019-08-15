#ifndef VIEW_H
#define VIEW_H

#include <QObject>
#include <QGraphicsRectItem>

class View : public QGraphicsRectItem
{
public:
    explicit View(QGraphicsRectItem *parent = Q_NULLPTR);
    virtual ~View();

protected:
        void mouseMoveEvent(QGraphicsSceneMouseEvent *ev);
};

#endif // VIEW_H
