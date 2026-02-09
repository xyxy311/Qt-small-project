#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->widget->setPixmap(QPixmap(":/img/img.png"));
    connect(ui->widget, &Puzzle::verifyFinished, [=](bool success){
        if (success) {
            qDebug() << "验证通过";
            float seconds = static_cast<float>(m_elapsedTimer.elapsed()) / 1000.0f;
            qDebug() << "用时：" << seconds << " 秒";
            ui->Slider->setEnabled(false);
        }
        else
            qDebug() << "验证失败";
    });
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_Slider_valueChanged(int value) {
    qDebug() << value;
    ui->widget->setValue((qreal)value / 999.0f);
}

void MainWindow::on_Slider_sliderPressed() {
    m_elapsedTimer.restart();
}

void MainWindow::on_Slider_sliderReleased() {
    ui->widget->verify();
}

void MainWindow::on_refresh_btn_clicked()
{
    ui->Slider->setValue(0);
    ui->Slider->setEnabled(true);
    static const char* names[] = {":/img/img.png", ":/img/img2.png", ":/img/img3.png"};
    static int i = 1;
    ui->widget->setPixmap(QPixmap(names[i]));
    i = (i + 1) % 3;
}

