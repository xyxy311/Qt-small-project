#ifndef TIMERWIDGET_H
#define TIMERWIDGET_H

#include <QWidget>
#include <QTimer>

namespace Ui {
class TimerWidget;
}

class TimerWidget : public QWidget
{
    Q_OBJECT


public:
    enum timer_state{unactivate, running, ispaused};

    explicit TimerWidget(QWidget *parent = nullptr);
    ~TimerWidget();

    void start();   // 开始计时
    void reset();   // 复位
    void pause();  // 暂停
    qint64 displayseconds();  // 显示时长
    qint64 settedseconds();   // 计时器设置时长
    unsigned char state{0};  // 计时状态
    void setTimer(qint64 s = 0);  // 设置时间

signals:
    void timeout();   // 计时结束时发送

private:
    Ui::TimerWidget *ui;
    qint64 m_seconds{0};   // 储存设置的时长
    qint64 m_cursecs{0};   // 当前时长
    QTimer *m_timer;

    void update();  // 更新计时器
    void activateSet();  // 允许编辑
    void deactivateSet();
};

#endif // TIMERWIDGET_H
