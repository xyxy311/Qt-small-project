#include "mainwindow.h"
#include "globle.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    AppData::dataDir = MainWindow::getDataDir();
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
