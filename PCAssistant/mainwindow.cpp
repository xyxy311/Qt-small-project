#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect(this, &MainWindow::save_toDoList, ui->widget_toDoList, &ToDoList::saveTasks);
    connect(qApp, &QApplication::applicationStateChanged, [this](Qt::ApplicationState state){
        if (state == Qt::ApplicationInactive) {
            emit save_toDoList();  // 失焦则保存
        }
    });
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::closeEvent(QCloseEvent *e)
{
    emit save_toDoList();
}

