#include "timer.h"
#include "ui_timer.h"

Timer::Timer(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Timer)
{
    ui->setupUi(this);

    // 定时结束
    connect(ui->timerWidget, &TimerWidget::timeout, [this](){
        ui->btn_startpause->setText("开始");
        qDebug("定时结束");
    });
}

Timer::~Timer()
{
    delete ui;
}



void Timer::on_btn_startpause_clicked()
{
    if (ui->timerWidget->state != TimerWidget::running)
    {
        if (ui->timerWidget->state == TimerWidget::unactivate && ui->timerWidget->totalseconds() == 0)
            return;
        ui->timerWidget->start();
        ui->btn_startpause->setText("暂停");
    }
    else {
        ui->timerWidget->pause();
        ui->btn_startpause->setText("开始");
    }
}


void Timer::on_btn_reset_clicked()
{
    ui->timerWidget->reset();
    ui->btn_startpause->setText("开始");
}
