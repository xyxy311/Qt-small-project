#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMetaEnum>
#include <QFileDialog>
#include <QFile>
#include <QMessageBox>
#include <QTextStream>
#include <QFileDialog>
#include <QSaveFile>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(ui->menuFile, &QMenu::triggered, this, &MainWindow::OnActionTriggered);
    connect(ui->menuEdit, &QMenu::triggered, this, &MainWindow::OnActionTriggered);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::OnActionTriggered(QAction *act)
{
    const QMetaEnum me = QMetaEnum::fromType<ActKind::ActionKind>();  // 获取枚举元对象
    bool ok = false;

    int k = me.keyToValue(act->text().toStdString().c_str(), &ok);  // 先转换格式，然后获取枚举值(value)
    if (!ok)
        return;

    switch (static_cast<ActKind::ActionKind>(k)) {  // 使用静态类型转换，更专业、安全
    case ActKind::ActionKind::NewFile:
        OnNewFile();
        break;
    case ActKind::ActionKind::OpenFile:
        OnOpenFile();
        break;
    case ActKind::ActionKind::SaveFile:
        OnSave();
        break;
    case ActKind::ActionKind::SaveAsFile:
        OnSaveAsFile();
        break;
    case ActKind::ActionKind::Quit:
        if (MaybeSave())
            QApplication::quit();
        break;
    case ActKind::ActionKind::Undo:
        ui->textEdit->undo();
        break;
    case ActKind::ActionKind::Cut:
        ui->textEdit->cut();
        break;
    case ActKind::ActionKind::Copy:
        ui->textEdit->copy();
        break;
    case ActKind::ActionKind::Paste:
        ui->textEdit->paste();
        break;
    case ActKind::ActionKind::SelectAll:
        ui->textEdit->selectAll();
        break;
    default:
        break;
    }
}

void MainWindow::OnNewFile()
{
    if (MaybeSave()) {   // 是否需要保存 判断成功
        ui->textEdit->clear();
        ui->textEdit->document()->setModified(false);
        cur_path = "";
    }
}

void MainWindow::OnOpenFile()
{
    if (MaybeSave()) {   // 是否需要保存 判断成功
        QString filename = QFileDialog::getOpenFileName();
        if (!filename.isEmpty()) {
            LoadFile(filename);
        }
    }
}

bool MainWindow::OnSave()
{
    if (cur_path.isEmpty())
        return OnSaveAsFile();
    else
        return SaveFile(cur_path);
}

bool MainWindow::OnSaveAsFile()
{
    QFileDialog dialog(this);
    dialog.setWindowModality(Qt::WindowModal);   // 设置模态：阻塞父类窗口
    dialog.setAcceptMode(QFileDialog::AcceptSave);  // 保存模式，可以新建/覆盖文件
    if (dialog.exec() != QDialog::Accepted)  // 获取返回值(Accepted用户保存 / Rejected用户取消或关闭)
        return false;
    return SaveFile(dialog.selectedFiles().first());
}

bool MainWindow::MaybeSave()
{
    if (!ui->textEdit->document()->isModified())  // 没有修改
        return true;

    const QMessageBox::StandardButton choice =
        QMessageBox::question(
            this, qApp->applicationName(), "是否保存？",
            QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel,
            QMessageBox::Cancel);

    switch(choice) {
    case QMessageBox::Yes:
        return OnSave();
    case QMessageBox::Cancel:
        return false;
    default:
        break;
    }

    return true;
}

void MainWindow::LoadFile(const QString &path)
{
    QFile file(path);
    if (!file.open(QFile::ReadOnly | QFile::Text)) {
        QMessageBox::warning(this, qApp->applicationName(), "无法打开文件");  // 使用qApp获取名称
        return;
    }

    QTextStream in(&file);
    ui->textEdit->setPlainText(in.readAll());   // 读取文件

    cur_path = path;
    ui->textEdit->document()->setModified(false);
    statusBar()->showMessage("打开成功", 3000);  // 状态栏显示
}

bool MainWindow::SaveFile(const QString &path)
{
    QSaveFile file(path);
    if (!file.open(QFile::WriteOnly | QFile::Text)) {
        return false;
    }

    QTextStream out(&file);
    out << ui->textEdit->toPlainText();  // 保存在临时文件中
    if (!file.commit()) {             // 原子操作，此时真正写入文件
        QMessageBox::warning(this, qApp->applicationName(), "保存失败");
        return false;
    }

    cur_path = path;
    ui->textEdit->document()->setModified(false);
    statusBar()->showMessage("保存成功", 3000);

    return true;
}
