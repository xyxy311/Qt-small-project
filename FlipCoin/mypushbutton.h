#ifndef MYPUSHBUTTON_H
#define MYPUSHBUTTON_H

#include <QPushButton>

class MyPushButton : public QPushButton
{
    Q_OBJECT
public:
    // explicit MyPushButton(QWidget *parent = nullptr);
    MyPushButton(QString normal_img, QString press_img = "");

    // 动画效果
    void zoom1();
    void zoom2();

    // 重写鼠标按下释放的事件
    void mousePressEvent(QMouseEvent *ev);
    void mouseReleaseEvent(QMouseEvent *ev);
private:
    QString normal_img, press_img;

signals:
};

#endif // MYPUSHBUTTON_H
