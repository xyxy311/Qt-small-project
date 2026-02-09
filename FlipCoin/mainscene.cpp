#include "mainscene.h"
#include "ui_mainscene.h"
#include "mypushbutton.h"
#include "scene.h"
#include "QDebug"
#include <QPainter>
#include <QTimer>
#include <QSoundEffect>

MainScene::MainScene(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainScene)
{
    ui->setupUi(this);

    // 设置主场景
    setFixedSize(320, 588);   // 固定大小
    setWindowIcon(QIcon(":/coins/img/coin0.png"));  // 图标
    setWindowTitle("这是翻金币游戏，我写的第一个Qt项目，跟着视频做的");   // 标题

    // 退出功能
    connect(ui->actionquit, &QAction::triggered, [=](){
        this->close();
    });

    // 创建开始按钮
    MyPushButton* startBtn = new MyPushButton(":/other/img/start_btn.png");
    startBtn->setParent(this);
    startBtn->move(this->width() / 2 - startBtn->width() / 2, this->height() * 0.7);

    QSoundEffect *sound1 = new QSoundEffect(this);
    sound1->setSource(QUrl("qrc:/other/sound/sound1.wav"));
    sound1->setVolume(0.8f);

    // 场景对象
    chooseScene = new Scene(this);

    // 监听返回信号
    connect(chooseScene, &Scene::backSignal, [=](){
        chooseScene->hide();
        this->move(chooseScene->pos());
        this->show();
    });
    connect(chooseScene, &Scene::quit, [=](){
        this->close();
    });

    // 连接开始按钮信号
    connect(startBtn, &MyPushButton::clicked, [=](){
        qDebug() << "开始！！！";
        startBtn->zoom1();
        startBtn->zoom2();
        sound1->play();

        // 延时
        QTimer::singleShot(200, this, [=](){
            this->hide();
            chooseScene->move(this->pos());
            chooseScene->show();
        });
    });
}

// 绘制
void MainScene::paintEvent(QPaintEvent *) {
    // 背景
    QPainter bacgnd(this);
    bacgnd.drawPixmap(0, 0, this->width(), this->height(), QPixmap(":/other/img/backgnd.png"));
    bacgnd.drawRect(0, 0, this->width(), 25);

    // 标题
    QPixmap title(":/other/img/title.png");
    title = title.scaled(title.width() * 1.2, title.height() * 1.2);
    bacgnd.drawPixmap(10, 30, title);
}

MainScene::~MainScene()
{
    delete ui;
}
