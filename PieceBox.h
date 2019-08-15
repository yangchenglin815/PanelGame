#ifndef PIECEBOX_H
#define PIECEBOX_H

/*******
 * 方块组
 * 4*4
 *
 *
 * *******/

#include <QObject>
#include <QGraphicsItemGroup>
#include <QKeyEvent>
#include <QTimer>
#include <QTransform>
#include <QGraphicsItem>

#include "enumheader.h"
#include "OnePiece.h"

class PieceBox : public QObject, public QGraphicsItemGroup
{
    Q_OBJECT
public:
    PieceBox();

public:
    //颜色表
    static QColor colorTable[7];
    //绘制方块组的边框矩形
    virtual QRectF boundingRect() const;
    //是否发生碰撞
    bool isCollding() const;
    //获取当前的方块类型
    BoxType getCurrentBoxType() const;
    //创建方块组
    void createBox(const QPointF &point = QPointF(0, 0), BoxType currentBoxType = RandomShape);
    //消除方块组
    void clearBoxGroup(const bool &isClear = false);

protected:
    virtual void keyPressEvent(QKeyEvent *event);

signals:
    void sigOnNeedNewBox();
    void sigOnGameOver();

public slots:
    void onMoveOnStep();
    void onStartTimer(int timeSec);
    void onStopTimer();

private:
    QTimer *m_timer;
    BoxType m_currentBoxType;
    QTransform m_oldTransform;
    QList<OnePiece *> m_pieceBoxList;
};

#endif // PIECEBOX_H
