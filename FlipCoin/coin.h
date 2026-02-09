#ifndef COIN_H
#define COIN_H

#include <QWidget>
#include <QPushButton>
#include <QTimer>

class Coin : public QPushButton
{
    Q_OBJECT
public:
    explicit Coin(bool isgold, QWidget *parent = nullptr);

    // 金币属性
    int posX;
    int posY;
    bool isgold;

    // 改变标志的方法
    void changeFlag();
    QTimer *timer1;  // 正面翻反面
    QTimer *timer2;  // 反面翻正面
    bool inAni;
    int min = 0;
    int max = 5;
    void mousePressEvent(QMouseEvent* e);

signals:
};

#endif // COIN_H
