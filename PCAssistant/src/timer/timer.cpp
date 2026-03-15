#include "timer.h"
#include "ui_timer.h"
#include "globle.h"
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonDocument>
#include <QFile>

Timer::Timer(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Timer)
{
    ui->setupUi(this);

    m_dataPath = AppData::dataDir + "/timer.json";
    qDebug() << m_dataPath;
    loadData();
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
        if (ui->timerWidget->state == TimerWidget::unactivate && ui->timerWidget->displayseconds() == 0)
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


bool Timer::loadData()
{
    QFile data(m_dataPath);
    if (!data.open(QIODevice::ReadOnly)) {
        qDebug() << "Load Time fail";
        return false;
    }
    QJsonDocument doc = QJsonDocument::fromJson(data.readAll());
    data.close();

    QJsonObject data_ob = doc.object();
    qint64 s = data_ob["time"].toInteger();
    ui->timerWidget->setTimer(s);

    qDebug() << "Load Time Suc: " << s;
    return true;
}


void Timer::saveData() const
{
    QJsonObject data_ob;
    if (ui->timerWidget->state == TimerWidget::unactivate)
        data_ob["time"] = ui->timerWidget->displayseconds();
    else
        data_ob["time"] = ui->timerWidget->settedseconds();

    QJsonDocument doc(data_ob);

    QFile data(m_dataPath);
    if (data.open(QIODevice::WriteOnly)) {
        data.write(doc.toJson());
        data.close();
    }
    qDebug() << "Save Time Success: " << data_ob["time"].toInteger();
}
