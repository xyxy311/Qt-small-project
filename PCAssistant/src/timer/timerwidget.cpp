#include "timerwidget.h"
#include "ui_timerwidget.h"

TimerWidget::TimerWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::TimerWidget)
    , m_timer(new QTimer)
{
    ui->setupUi(this);

    ui->h_spin->setRange(0, 99);
    ui->m_spin->setRange(0, 59);
    ui->s_spin->setRange(0, 59);

    m_timer->setInterval(1000);

    connect(m_timer, &QTimer::timeout, [this](){
        --m_cursecs;
        update();  // 每秒更新
        qDebug() << m_cursecs;
    });
}


TimerWidget::~TimerWidget()
{
    delete ui;
}


void TimerWidget::start()
{

    m_seconds = ui->s_spin->value() + ui->m_spin->value() * 60 +
                ui->h_spin->value() * 3600;
    m_cursecs = m_seconds;

    m_timer->start();
    update();
    state = running;

    deactivateSet();
}


void TimerWidget::reset()
{
    m_timer->stop();
    m_cursecs = m_seconds;
    update();
    state = unactivate;
    activateSet();
}


void TimerWidget::pause()
{
    m_timer->stop();
    state = ispaused;
    activateSet();
}


qint64 TimerWidget::displayseconds()
{
    return ui->s_spin->value() + ui->m_spin->value() * 60 +
           ui->h_spin->value() * 3600;
}

qint64 TimerWidget::settedseconds()
{
    return m_seconds;
}


void TimerWidget::setTimer(qint64 s)
{
    m_seconds = s;
    m_cursecs = s;
    update();
}


void TimerWidget::update()
{
    ui->s_spin->setValue(m_cursecs % 60);
    ui->m_spin->setValue(m_cursecs / 60 % 60);
    ui->h_spin->setValue(m_cursecs / 3600);
    if (m_cursecs == 0 && state == running) {
        emit timeout();
        m_timer->stop();
        state = unactivate;
        activateSet();
    }
}


void TimerWidget::activateSet()
{
    ui->h_spin->setEnabled(true);
    ui->m_spin->setEnabled(true);
    ui->s_spin->setEnabled(true);
}


void TimerWidget::deactivateSet()
{
    ui->h_spin->setEnabled(false);
    ui->m_spin->setEnabled(false);
    ui->s_spin->setEnabled(false);
}

