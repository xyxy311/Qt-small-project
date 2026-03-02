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
    int totalseconds();  // 计时时长
    unsigned char state{0};  // 计时状态

signals:
    void timeout();   // 计时结束时发送

private:
    Ui::TimerWidget *ui;
    int m_seconds{0};   // 储存设置的时长
    int m_cursecs{0};   // 当前时长
    QTimer *m_timer;

    void update_display();  // 更新计时器显示
};

#endif // TIMERWIDGET_H
