#ifndef TODOLIST_H
#define TODOLIST_H

#include <QWidget>
#include <QStandardItemModel>

namespace Ui {
class ToDoList;
}

class ToDoList : public QWidget
{
    Q_OBJECT

public:
    explicit ToDoList(QWidget *parent = nullptr);
    ~ToDoList();

public slots:
    void saveTasks();  // 保存待办文件

private slots:
    void on_actionAdd_triggered();
    void on_actionDel_triggered();
    void on_actionClear_triggered();
    void on_actionUpdate_triggered();

private:
    Ui::ToDoList *ui;

    QString m_dataPath;
    QStandardItemModel* m_toDoListModel;  // 储存代办事项
    QStringList m_defaultItems;  // 默认展示

    QStandardItem* newTodoItem(QString text = "新建代办", bool checked = false);  // 新建代办项

    // 加载保存数据相关的
    bool loadTasks();
    QString getTaskFilePath();
    bool isModified;   // 发生修改
};

#endif // TODOLIST_H
