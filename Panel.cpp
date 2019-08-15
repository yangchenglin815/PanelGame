#include "Panel.h"
#include "ui_Panel.h"
#include "enumheader.h"

#include <QDebug>
#include <QGraphicsBlurEffect>
#include <QPropertyAnimation>
#include <QMessageBox>
#include <QMediaContent>

Panel::Panel(QWidget *parent) : QMainWindow(parent)
  , ui(new Ui::Panel)
  , m_isPauseGame(false)
  , m_isGameOver(false)
{
    ui->setupUi(this);

    m_pMediaPlayer = new QMediaPlayer;
    m_pMediaPlayer->setMedia(QUrl::fromLocalFile(QString("*.mp3")));
    m_pMediaPlayer->setVolume(100);

    connect(ui->btn_start, SIGNAL(clicked()), this, SLOT(onStartGame()));
    connect(ui->btn_stop, SIGNAL(clicked()), this, SLOT(onStopGame()));
    connect(ui->btn_pause, SIGNAL(clicked()), this, SLOT(onPauseGame()));
    connect(ui->btn_restart, SIGNAL(clicked()), this, SLOT(onRestartGame()));

    //设置背景
    setBackground(LV0);
    initControlWidget();

    initGraphicsView();
}

Panel::~Panel()
{
    delete ui;
}

void Panel::setBackground(const int &Lv)
{
    setAutoFillBackground(true);
    switch (Lv) {
    case LV0:
        m_currentLv = LV0;
        m_currentLVSpeed = LV0_SPEED;
        ui->lbl_gameLevel->setText(QStringLiteral("俄罗斯方块"));
        m_pixmap.load(":/Resource/background.png");
        break;
    case LV1:
        m_currentLv = LV1;
        m_currentLVSpeed = LV1_SPEED;
        ui->lbl_gameLevel->setText(QStringLiteral("第一关"));
        m_pixmap.load(":/Resource/background.png");
        break;
    case LV2:
        m_currentLv = LV2;
        m_currentLVSpeed = LV2_SPEED;
        ui->lbl_gameLevel->setText(QStringLiteral("第二关"));
        m_pixmap.load(":/Resource/background.png");
        break;
    case LV3:
        m_currentLv = LV3;
        m_currentLVSpeed = LV3_SPEED;
        ui->lbl_gameLevel->setText(QStringLiteral("第三关"));
        m_pixmap.load(":/Resource/background.png");
        break;
    case LV4:
        m_currentLv = LV4;
        m_currentLVSpeed = LV4_SPEED;
        ui->lbl_gameLevel->setText(QStringLiteral("第四关"));
        m_pixmap.load(":/Resource/background.png");
        break;
    case LV5:
        m_currentLv = LV5;
        m_currentLVSpeed = LV5_SPEED;
        ui->lbl_gameLevel->setText(QStringLiteral("第五关"));
        m_pixmap.load(":/Resource/background.png");
        break;
    default:
        break;
    }

    m_palette.setBrush(QPalette::Window, QBrush(m_pixmap.scaled(size(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation)));
    setPalette(m_palette);
}

void Panel::initControlWidget()
{
    switch (m_currentLv) {
    case LV0:
        ui->btn_start->setEnabled(true);
        ui->btn_pause->setEnabled(false);
        ui->btn_restart->setEnabled(false);
        ui->btn_stop->setEnabled(false);
        break;
    case LV1:
    case LV2:
    case LV3:
    case LV4:
    case LV5:
        ui->btn_start->setEnabled(false);
        ui->btn_pause->setEnabled(true);
        ui->btn_restart->setEnabled(true);
        ui->btn_stop->setEnabled(true);
        break;
    default:
        break;
    }
}

void Panel::initGraphicsView()
{
    //设置抗锯齿
    ui->graphicsView->setRenderHint(QPainter::Antialiasing);
    //设置缓冲背景，加速渲染
    ui->graphicsView->setCacheMode(QGraphicsView::CacheBackground);

    m_pScene = new QGraphicsScene(this);
    m_pScene->setSceneRect(30, 30, 310, 410);
    ui->graphicsView->setScene(m_pScene);

    //方块可移动的四条线
    //向外扩展3像素，这样可以使方块组到达边界的时候再移动就会发生碰撞
    m_topLine = m_pScene->addLine(32, 32, 238, 32);
    m_leftLine = m_pScene->addLine(32, 32, 32, 438);
    m_bottomLine = m_pScene->addLine(32, 438, 238, 438);
    m_rightLine = m_pScene->addLine(238, 32, 238, 438);

    m_topLine->setPen(QPen(QColor(Qt::gray)));
    m_leftLine->setPen(QPen(QColor(Qt::gray)));
    m_bottomLine->setPen(QPen(QColor(Qt::gray)));
    m_rightLine->setPen(QPen(QColor(Qt::gray)));

    m_currentBox = new PieceBox;
    m_nextBox = new PieceBox;
    connect(m_currentBox, SIGNAL(sigOnNeedNewBox()), this, SLOT(onClearFullRows()));
    connect(m_currentBox, SIGNAL(sigOnGameOver()), this, SLOT(onGameOver()));
    m_pScene->addItem(m_currentBox);
    m_pScene->addItem(m_nextBox);
}

void Panel::startGame()
{
    m_currentBox->createBox(QPointF(135, 55));
    m_nextBox->createBox(QPointF(290, 55));
    //将键盘焦点给当前的方块组
    m_currentBox->setFocus();
    m_currentBox->onStartTimer(m_currentLVSpeed);
}

void Panel::pauseGame()
{
    if(m_isPauseGame)
    {
        return;
    }
    m_isPauseGame = true;
    m_currentBox->onStopTimer();
    ui->btn_start->setEnabled(true);
    QMessageBox::information(this, QStringLiteral("提示"), QStringLiteral("暂停游戏！"), QMessageBox::Yes);
}

void Panel::restartGame()
{
    m_currentBox->onStopTimer();
    m_currentBox->clearBoxGroup();
    m_nextBox->clearBoxGroup(true);
    //先将当前的小正方形组移出游戏框，防止下面的清除item将该方块组清除了
    m_currentBox->setPos(290,55);
    ui->lcd_gameScore->display(0);

    //清空视图中所有的小方块
    foreach (QGraphicsItem *item, m_pScene->items(34, 34, 204, 404, Qt::ContainsItemShape,Qt::AscendingOrder)) {
        // 先从场景中移除小方块，因为使用deleteLater()是在返回主事件循环后才销毁
        // 小方块的，为了在出现新的方块组时不发生碰撞，所以需要先从场景中移除小方块
        m_pScene->removeItem(item);
        OnePiece *piece = (OnePiece*) item;
        piece->deleteLater();
    }
    m_isPauseGame = false;
    onStartGame();
}

void Panel::stopGame()
{
    m_currentBox->onStopTimer();
    onGameOver();
}

void Panel::updateScore(const int fullRows)
{
    int score = ui->lcd_gameScore->value() + fullRows * ROWSCORE;

    ui->lcd_gameScore->display(score);
    if(score < 1000)         //第一关
    {

    }else if(score < 2000)  //第二关
    {
        setBackground(LV2);
        initControlWidget();
        m_currentBox->onStopTimer();
        m_currentBox->onStartTimer(m_currentLVSpeed);
    }else if(score < 4000)  //第三关
    {
        setBackground(LV3);
        initControlWidget();
        m_currentBox->onStopTimer();
        m_currentBox->onStartTimer(m_currentLVSpeed);
    }else if(score < 8000) //第四关
    {
        setBackground(LV4);
        initControlWidget();
        m_currentBox->onStopTimer();
        m_currentBox->onStartTimer(m_currentLVSpeed);
    }else if(score < 16000)  //第五关
    {
        setBackground(LV5);
        initControlWidget();
        m_currentBox->onStopTimer();
        m_currentBox->onStartTimer(m_currentLVSpeed);
    }else   //从第一关重新开始
    {
        setBackground(LV1);
        initControlWidget();
        m_currentBox->onStopTimer();
        m_currentBox->onStartTimer(m_currentLVSpeed);
    }
}

void Panel::onStartGame()
{
    m_isGameOver = false;
    if (m_isPauseGame)
    {
        ui->btn_start->setEnabled(false);
        m_isPauseGame = false;
        m_currentBox->onStartTimer(m_currentLVSpeed);
        return;
    }

    //默认等级为LV1
    setBackground(LV1);
    initControlWidget();

    startGame();
}

void Panel::onPauseGame()
{
    if(m_isPauseGame)
    {
        return;
    }
    m_isPauseGame = true;
    m_currentBox->onStopTimer();
    ui->btn_start->setEnabled(true);
    QMessageBox::information(this, QStringLiteral("提示"), QStringLiteral("暂停游戏！"), QMessageBox::Yes);
}

void Panel::onRestartGame()
{
    m_currentBox->onStopTimer();
    m_currentBox->clearBoxGroup();
    m_nextBox->clearBoxGroup(true);
    //先将当前的小正方形组移出游戏框，防止下面的清除item将该方块组清除了
    m_currentBox->setPos(290,55);
    ui->lcd_gameScore->display(0);

    //清空视图中所有的小方块
    foreach (QGraphicsItem *item, m_pScene->items(34, 34, 204, 404, Qt::ContainsItemShape,Qt::AscendingOrder)) {
        // 先从场景中移除小方块，因为使用deleteLater()是在返回主事件循环后才销毁
        // 小方块的，为了在出现新的方块组时不发生碰撞，所以需要先从场景中移除小方块
        m_pScene->removeItem(item);
        OnePiece *piece = (OnePiece*) item;
        piece->deleteLater();
    }
    m_isPauseGame = false;
    onStartGame();
}

void Panel::onStopGame()
{
    m_currentBox->onStopTimer();
    onGameOver();
}

void Panel::onClearFullRows()
{
    m_rowList.clear();
    //获取比一行方格多行的所有小方块,不包含最高的一行
    for(int i = 414; i > 35; i -= 20)
    {
        //返回可视区域(202*22)内所有的完全可视的item
        QList<QGraphicsItem *> itemList = m_pScene->items(34, i, 204, 22, Qt::ContainsItemShape, Qt::AscendingOrder);
        //        qDebug() << "可是区域内的item数：" << itemList.count();
        //已满
        if(itemList.count() == 10)
        {
            //遍历列表删除小方块
            foreach (QGraphicsItem *item, itemList) {
                OnePiece *piece = (OnePiece *)item;

                //模糊效果，先放大再缩小
                QGraphicsBlurEffect *blurEffect = new QGraphicsBlurEffect;
                piece->setGraphicsEffect(blurEffect);
                QPropertyAnimation *animation = new QPropertyAnimation(piece, "scale");
                animation->setDuration(250);
                animation->setEasingCurve(QEasingCurve::OutBounce);
                animation->setStartValue(4);
                animation->setEndValue(0.25);
                animation->start(QAbstractAnimation::DeleteWhenStopped);
                connect(animation,SIGNAL(finished()), piece, SLOT(deleteLater()));
            }
            m_pMediaPlayer->play();
            //记录满行的行地址
            m_rowList.append(i);
        }
    }
    //    qDebug() << "满行的行数：" << m_rowList.size();
    //存在满行，删除后将上面的方块下移
    if(!m_rowList.isEmpty())
    {
        //等待所有的小正方形都销毁后再将上方的小正方形向下移动
        QTimer::singleShot(300, this, SLOT(onMoveBox()));
    }else   //直接创建新的方块组
    {
        m_currentBox->createBox(QPointF(135, 55), m_nextBox->getCurrentBoxType());
        // 清空并销毁提示方块组中的所有小方块
        m_nextBox->clearBoxGroup(true);
        if(!m_isGameOver)
        {
            m_nextBox->createBox(QPointF(290, 55));
        }
    }
}

void Panel::onGameOver()
{
    m_isGameOver = true;
    QMessageBox::information(this, QStringLiteral("提示"), QStringLiteral("您的游戏得分为%1!").arg(ui->lcd_gameScore->value()),QMessageBox::Yes);
    ui->lcd_gameScore->display(0);
    m_currentBox->clearFocus();

    setBackground(LV0);
    initControlWidget();
    m_isPauseGame = false;
    //初始化界面
    m_currentBox->clearBoxGroup(true);
    m_nextBox->clearBoxGroup(true);
    //先将当前的小正方形组移出游戏框，防止下面的清除item将该方块组清除了
    m_currentBox->setPos(290,55);
    //清空视图中所有的小方块
    foreach (QGraphicsItem *item, m_pScene->items(34, 34, 204, 404, Qt::ContainsItemShape, Qt::AscendingOrder)) {
        // 先从场景中移除小方块，因为使用deleteLater()是在返回主事件循环后才销毁
        // 小方块的，为了在出现新的方块组时不发生碰撞，所以需要先从场景中移除小方块
        m_pScene->removeItem(item);
        OnePiece *piece = (OnePiece*) item;
        piece->deleteLater();
    }
}

void Panel::onMoveBox()
{
    // 从位置最靠上的满行开始
    for (int i = m_rowList.count(); i > 0; i--)
    {
        foreach (QGraphicsItem *item, m_pScene->items(34, 34, 206, m_rowList.at(i-1) - 32, Qt::ContainsItemShape,Qt::AscendingOrder)) {
            item->moveBy(0, 20);
        }
    }
    // 更新分数
    updateScore(m_rowList.count());
    // 将满行列表清空为0
    m_rowList.clear();

    // 等所有行下移以后再出现新的方块组
    m_currentBox->createBox(QPointF(135, 55), m_nextBox->getCurrentBoxType());
    // 清空并销毁提示方块组中的所有小方块
    m_nextBox->clearBoxGroup(true);
    m_nextBox->createBox(QPointF(290, 55));
}
