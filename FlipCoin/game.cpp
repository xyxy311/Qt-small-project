#include "game.h"
#include "MyPushButton.h"
#include "coin.h"
#include "qpropertyanimation.h"
#include "vector"
#include "dataconfig.h"
#include <QMenuBar>
#include <QPixmap>
#include <QTimer>
#include <QPainter>
#include <QDebug>
#include <QLabel>
#include <QSoundEffect>

// Game::Game(QWidget *parent)
//     : QMainWindow{parent}
// {}

Game::Game(int l) : level(l) {
    qDebug() << l;

    // 窗口
    setFixedSize(320, 588);   // 固定大小
    setWindowIcon(QIcon(":/coins/img/coin0.png"));
    setWindowTitle(QString("第%1关").arg(level));

    // 菜单
    QMenuBar *bar = menuBar();
    setMenuBar(bar);
    QMenu *startMenu = bar->addMenu("开始");
    QAction *quitAction = startMenu->addAction("退出");
    connect(quitAction, &QAction::triggered, [=](){
        emit quit();
        close();
    });

    // 返回按钮
    MyPushButton *backBtn = new MyPushButton(":/other/img/back1.png", ":/other/img/back2.png");
    backBtn->setParent(this);
    backBtn->move(this->width() / 2 - backBtn->width() / 2, this->height() * 0.8);
    connect(backBtn, &MyPushButton::clicked, [=](){
        emit this->backSignal();
    });

    // 显示关卡标签
    QLabel *levelLabel = new QLabel(this);
    levelLabel->setText(QString("Level %1").arg(this->level));
    levelLabel->setGeometry(30, 538, 120, 50);
    levelLabel->setFont(QFont("华文新魏", 20));

    QSoundEffect* sound2 = new QSoundEffect(this);
    sound2->setSource(QUrl("qrc:/other/sound/sound2.wav"));

    // 关卡配置
    dataConfig data(this->level);
    config = data.getData();
    success = false;
    flipping = false;

    // 显示金币背景（i:x j:y）
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            // 金币背景板
            QLabel *board = new QLabel(this);
            board->setGeometry(57 + i * 50, 200 + j * 50, 50, 50);
            board->setPixmap(QPixmap(":/other/img/board.png"));

            // 金币
            bool isgold = (config[j][i] == 1);
            Coin *coin = new Coin(isgold, this);
            coin->move(59 + i * 50, 202 + j * 50);
            coin->posX = 57 + i * 50;
            coin->posY = 200 + j * 50;
            coin->isgold = isgold;
            coins[j][i] = coin;

            connect(coin, &Coin::clicked, [=](){
                if (success | flipping)
                    return;
                sound2->play();
                flipping = true;
                coin->changeFlag();
                config[j][i] = 1 - config[j][i];

                QTimer::singleShot(300, this, [=](){
                    if (j > 0) {
                        coins[j - 1][i]->changeFlag();
                        config[j - 1][i] = 1 - config[j - 1][i];
                    }
                    if (j < 3) {
                        coins[j + 1][i]->changeFlag();
                        config[j + 1][i] = 1 - config[j + 1][i];
                    }
                    if (i > 0) {
                        coins[j][i - 1]->changeFlag();
                        config[j][i - 1] = 1 - config[j][i -1 ];
                    }
                    if (i < 3) {
                        coins[j][i + 1]->changeFlag();
                        config[j][i + 1] = 1 - config[j][i + 1];
                    }

                    success = true;
                    for (int i = 0; i < 16; i++) {
                        if (coins[i / 4][i % 4]->isgold != true) {
                            success = false;
                            break;
                        }
                    }
                    flipping = false;

                    if (success) {
                        success_scene();
                    }
                });
            });
        }
    }
}

void Game::success_scene() {
    qDebug("胜利");

    QLabel *label = new QLabel(this);
    label->setPixmap(QPixmap(":/other/img/succ.png"));
    label->setGeometry(50, -110, 145, 110);
    label->setStyleSheet("color: blue; background-color: white; border: 2px solid red;");
    label->show();

    QPropertyAnimation* animation = new QPropertyAnimation(label, "geometry");
    animation->setDuration(1000);
    animation->setStartValue(QRect(label->x(), label->y(), label->width(), label->height()));
    animation->setEndValue(QRect(label->x(), label->y()+140, label->width(), label->height()));
    animation->setEasingCurve(QEasingCurve::OutBounce);
    animation->start();
}

void Game::paintEvent(QPaintEvent *) {
    // 背景
    QPainter bacgnd(this);
    bacgnd.drawPixmap(0, 0, this->width(), this->height(), QPixmap(":/other/img/backgnd2.png"));
    bacgnd.drawRect(0, 0, this->width(), 25);

    // 标题
    QPixmap title(":/other/img/title.png");
    title = title.scaled(title.width() * 1.2, title.height() * 1.2);
    bacgnd.drawPixmap(10, 30, title);
}
