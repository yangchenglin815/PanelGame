#include "OnePiece.h"
#include "enumheader.h"

#include <QPainter>

/****
 * 为了避免同一个方块组内小方块发生碰撞，
 * 小方块的大小实际为19.5*19.5，但是小方块加上笔刷的宽度后为20*20
 * 这样看起来是四个小方块连在一起的
 * **/

OnePiece::OnePiece(const QColor &brushColor)
    : m_pBrushColor(brushColor)
{

}

OnePiece::~OnePiece()
{

}

QRectF OnePiece::boundingRect() const
{
    qreal penWidth = PEN_WIDTH;
    return QRectF(-(PIECE_DIAMETER - penWidth) / 2, -(PIECE_DIAMETER - penWidth) / 2, PIECE_DIAMETER - penWidth, PIECE_DIAMETER - penWidth);
}

void OnePiece::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    //背景贴图
    painter->drawPixmap(-PIECE_DIAMETER/2, -PIECE_DIAMETER/2, PIECE_DIAMETER, PIECE_DIAMETER, QPixmap(":/Resource/box.png"));
    painter->setBrush(m_pBrushColor);
    //将方块的边界透明度减小
    QColor penColor = m_pBrushColor;
    penColor.setAlpha(200);
    painter->setPen(penColor);
    painter->drawRect(-PIECE_DIAMETER/2, -PIECE_DIAMETER/2, PIECE_DIAMETER, PIECE_DIAMETER);
}

QPainterPath OnePiece::shape() const
{
    QPainterPath path;
    //去除笔刷的宽度，便于碰撞检测
    path.addRect(-(PIECE_DIAMETER - PEN_WIDTH)/2, -(PIECE_DIAMETER - PEN_WIDTH)/2, PIECE_DIAMETER - PEN_WIDTH, PIECE_DIAMETER - PEN_WIDTH);
    return path;
}
