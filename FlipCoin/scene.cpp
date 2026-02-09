#include "scene.h"
#include "mypushbutton.h"
#include "game.h"
#include <QMenuBar>
#include <QPainter>
#include <QLabel>
#include <QDebug>
#include <QPushButton>

Scene::Scene(QWidget *parent)
    : QMainWindow(parent) {
    this->setFixedSize(320, 588);
    this->setWindowIcon(QPixmap(":/coins/img/coin0.png"));
    this->setWindowTitle("选择关卡场景");

    // 菜单栏
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

    // 关卡选择按钮
    for (int i = 0; i < 20; i++) {  // 单层循环创建矩阵
        MyPushButton *btn = new MyPushButton(":/coins/img/coin5.png");
        btn->setParent(this);
        btn->move(25 + i % 4 * 70, 130 + i / 4 * 70);

        // 响应按钮点击
        connect(btn, &MyPushButton::clicked, [=](){
            qDebug() << QString("点击了第%1关").arg(i + 1);
            this->gameScene = new Game(i +1 );  // 必须要先创建对象，再连接，连接空指针会崩溃
            this->gameScene->move(this->pos());
            this->hide();
            this->gameScene->show();

            // 监听游戏场景返回信号
            connect(gameScene, &Game::backSignal, [=](){
                gameScene->hide();
                this->move(gameScene->pos());
                this->show();
                delete gameScene;   // 自动断开连接
                gameScene = nullptr;
            });
            connect(gameScene, &Game::quit, [=](){
                emit quit();
                close();
            });

        });

        // 文字
        QLabel *label = new QLabel(this);
        label->move(25 + i % 4 * 70, 130 + i / 4 * 70);
        label->setFixedSize(btn->size());
        label->setText(QString::number(i+1));
        label->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);  // 对齐方式
        label->setAttribute(Qt::WA_TransparentForMouseEvents);  // 鼠标穿透
    }
}

void Scene::paintEvent(QPaintEvent *) {
    // 背景
    QPainter bacgnd(this);
    bacgnd.drawPixmap(0, 0, this->width(), this->height(), QPixmap(":/other/img/backgnd2.png"));
    bacgnd.drawRect(0, 0, this->width(), 25);
}
