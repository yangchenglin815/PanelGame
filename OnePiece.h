#ifndef ONEPIECE_H
#define ONEPIECE_H

/**************
 * 一块方块的绘制
 *
 *
 * *************/

#include <QGraphicsObject>
#include <QColor>

class OnePiece : public QGraphicsObject
{
public:
    OnePiece(const QColor &brushColor = Qt::red);
    virtual ~OnePiece(void);

public:
    //提供一个外围的边界矩形
    virtual QRectF boundingRect() const;
    //本地坐标系绘制项
    virtual void paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget = 0);
    //重新定义项的形状
    virtual QPainterPath shape() const;

private:
    QColor m_pBrushColor;
};

#endif // ONEPIECE_H
