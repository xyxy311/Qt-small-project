#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QElapsedTimer>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public slots:
    void on_Slider_valueChanged(int value);
    void on_Slider_sliderPressed();
    void on_Slider_sliderReleased();

private slots:
    void on_refresh_btn_clicked();

private:
    Ui::MainWindow *ui;
    QElapsedTimer m_elapsedTimer;
};
#endif // MAINWINDOW_H
