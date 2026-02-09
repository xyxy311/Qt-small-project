#include "mypushbutton.h"
#include <QString>
#include <QDebug>
#include <QPropertyAnimation>

// MyPushButton::MyPushButton(QWidget *parent)
//     : QPushButton{parent}
// {}

MyPushButton::MyPushButton(QString normal_img, QString press_img)
    : normal_img(normal_img), press_img(press_img) {

    QPixmap pix;
    bool ret = pix.load(normal_img);
    if (!ret) {
        qDebug() << "图片加载失败";
        return;
    }

    // 通过图片设置大小，不规则图片样式
    this->setFixedSize(QSize(40, 40));
    this->setStyleSheet("QPushButton{border:0px;}");
    this->setIcon(pix);
    this->setIconSize(QSize(40, 40));
}

void MyPushButton::zoom1() {
    // 创建动态对象
    QPropertyAnimation *ani = new QPropertyAnimation(this, "geometry");
    ani->setDuration(200);  // 时间间隔
    ani->setStartValue(QRect(this->x(), this->y(), this->width(), this->height()));  // 起始位置
    ani->setEndValue(QRect(this->x(), this->y()+10, this->width(), this->height()));
    ani->setEasingCurve(QEasingCurve::OutCurve);
    ani->start();
}

void MyPushButton::zoom2() {
    // 创建动态对象
    QPropertyAnimation *ani = new QPropertyAnimation(this, "geometry");
    ani->setDuration(200);  // 时间间隔
    ani->setStartValue(QRect(this->x(), this->y()+10, this->width(), this->height()));  // 起始位置
    ani->setEndValue(QRect(this->x(), this->y(), this->width(), this->height()));
    ani->setEasingCurve(QEasingCurve::OutCurve);
    ani->start();
}

void MyPushButton::mousePressEvent(QMouseEvent *ev) {
    if (press_img != "") {
        this->setIcon(QIcon(press_img));
        this->setIconSize(QSize(40, 40));
    }
    return QPushButton::mousePressEvent(ev);
}
void MyPushButton::mouseReleaseEvent(QMouseEvent *ev) {
    if (press_img != "") {
        this->setIcon(QIcon(normal_img));
        this->setIconSize(QSize(40, 40));
    }
    return QPushButton::mouseReleaseEvent(ev);
}
