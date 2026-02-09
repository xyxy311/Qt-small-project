#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE


// 动作的Qt元对象枚举类型
namespace ActKind {
Q_NAMESPACE

enum ActionKind{
    NewFile,
    OpenFile,
    SaveFile,
    SaveAsFile,
    Quit,
    Undo,
    Cut,
    Copy,
    Paste,
    SelectAll
};
Q_ENUM_NS(ActionKind)  // 注册
}
// /动作的Qt元对象枚举类型


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public slots:
    void OnActionTriggered(QAction *act);
    void OnNewFile();
    void OnOpenFile();
    bool OnSave();
    bool OnSaveAsFile();

private:
    bool MaybeSave();
    void LoadFile(const QString& path);
    bool SaveFile(const QString& path);
    QString cur_path;  // 当前路径
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
