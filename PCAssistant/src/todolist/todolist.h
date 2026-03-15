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

    bool isModified;   // 发生修改
    QString m_dataPath;
    QStandardItemModel* m_toDoListModel;  // 储存代办事项

    QStandardItem* newTodoItem(QString text = "新建代办", bool checked = false);  // 新建代办项
    bool loadTasks();  // 加载数据

};

#endif // TODOLIST_H
