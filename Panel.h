#ifndef PANEL_H
#define PANEL_H

#include <QMainWindow>
#include <QPalette>
#include <QPixmap>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsLineItem>
#include <QMediaPlayer>

#include "PieceBox.h"
#include "OnePiece.h"

namespace Ui {
class Panel;
}

class Panel : public QMainWindow
{
    Q_OBJECT

public:
    explicit Panel(QWidget *parent = 0);
    ~Panel();

public:
    void setBackground(const int &Lv);
    void initControlWidget();
    void initGraphicsView();

    /**游戏控制项**/
    void startGame();
    void pauseGame();
    void restartGame();
    void stopGame();

    void updateScore(const int fullRows = 0);

private slots:
    void onStartGame();
    void onPauseGame();
    void onRestartGame();
    void onStopGame();
    void onClearFullRows();
    void onGameOver();
    void onMoveBox();

private:
    Ui::Panel *ui;
    QPalette m_palette;
    QPixmap m_pixmap;
    int m_currentLv;
    int m_currentLVSpeed;
    bool m_isPauseGame;
    bool m_isGameOver;
    QGraphicsScene *m_pScene;

    //四条边界线
    QGraphicsLineItem *m_topLine;
    QGraphicsLineItem *m_leftLine;
    QGraphicsLineItem *m_rightLine;
    QGraphicsLineItem *m_bottomLine;

    PieceBox *m_currentBox;
    PieceBox *m_nextBox;

    QList<int> m_rowList;
    QMediaPlayer *m_pMediaPlayer;
};

#endif // PANEL_H
