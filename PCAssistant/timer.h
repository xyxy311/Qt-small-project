#ifndef TIMER_H
#define TIMER_H

#include <QWidget>

namespace Ui {
class Timer;
}

class Timer : public QWidget
{
    Q_OBJECT

public:
    explicit Timer(QWidget *parent = nullptr);
    ~Timer();

private slots:
    void on_btn_startpause_clicked();
    void on_btn_reset_clicked();

private:
    Ui::Timer *ui;
};

#endif // TIMER_H
