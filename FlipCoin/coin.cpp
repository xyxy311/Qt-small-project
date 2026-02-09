#include "coin.h"
#include <QPushButton>

Coin::Coin(bool isgold, QWidget *parent)
    : QPushButton{parent}
{
    QString str = isgold ? ":/coins/img/coin0.png" : ":/coins/img/coin5.png";
    setFixedSize(45, 45);
    setIcon(QPixmap(str));
    setIconSize(QSize(45, 45));
    setStyleSheet("QPushButton{border:0px;}");

    timer1 = new QTimer(this);
    timer2 = new QTimer(this);
    inAni = false;

    connect(timer1, &QTimer::timeout, [=](){
        QString ani = QString(":/coins/img/coin%1.png").arg(min++);
        QPixmap pix(ani);
        setIcon(pix);
        setIconSize(QSize(static_cast<int>(45.0 / pix.height() * pix.width()), 45));
        if (min > max) {
            min = 0;
            timer1->stop();
            inAni = false;
        }
    });

    connect(timer2, &QTimer::timeout, [=](){
        QString ani = QString(":/coins/img/coin%1.png").arg(max--);
        QPixmap pix(ani);
        setIcon(pix);
        setIconSize(QSize(static_cast<int>(45.0 / pix.height() * pix.width()), 45));
        if (min > max) {
            max = 5;
            timer2->stop();
            inAni = false;
        }
    });
}

// 重写鼠标事件
void Coin::mousePressEvent(QMouseEvent* e) {
    if (this->inAni) {
        return;
    } else {
        QPushButton::mousePressEvent(e);
    }

}

void Coin::changeFlag() {
    if (this->isgold) {
        inAni = true;
        this->isgold = false;
        this->timer1->start(30);

    } else {
        inAni = true;
        this->isgold = true;
        this->timer2->start(30);
    }

}
