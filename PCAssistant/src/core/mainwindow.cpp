#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "globle.h"
#include <QFileInfo>
#include <QDir>
#include <QStandardPaths>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect(qApp, &QApplication::applicationStateChanged, [this](Qt::ApplicationState state){
        if (state == Qt::ApplicationInactive) {
            ui->widget_toDoList->saveTasks();
            ui->widget_timer->saveData();
            ui->widget_notes->saveData();
        }
    });
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::closeEvent(QCloseEvent *e)
{
    ui->widget_toDoList->saveTasks();
    ui->widget_timer->saveData();
    ui->widget_notes->saveData();
}


// 获取保存数据文件的位置
QString MainWindow::getDataDir()
{
    QString appDir = QCoreApplication::applicationDirPath();
    QString dirPath = appDir + "/data";

    // 检查应用程序目录是否可写（简单测试）
    QFileInfo dirInfo(appDir);
    if (!dirInfo.isWritable()) {
        // 不可写，回退到用户数据目录
        QString fallbackPath = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
        dirPath = fallbackPath + "/PCAssistant/data";
    }
    QDir().mkpath(dirPath); // 确保目录存在
    return dirPath;
}


