#include "ToDoList.h"
#include "ui_ToDoList.h"
#include "customdelegate.h"
#include <QStringList>
#include <QMessageBox>
#include <QCoreApplication>
#include <QFile>
#include <QStandardPaths>
#include <QDir>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonDocument>
#include <QCloseEvent>

ToDoList::ToDoList(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::ToDoList)
    , m_toDoListModel(new QStandardItemModel(this))
{
    ui->setupUi(this);
    isModified = false;
    m_dataPath = getTaskFilePath();
    qDebug() << m_dataPath;

    ui->toDolistView->setItemDelegate(new CustomDelegate(ui->toDolistView));

    ui->btn_add->setDefaultAction(ui->actionAdd);
    ui->btn_cl->setDefaultAction(ui->actionClear);
    ui->btn_del->setDefaultAction(ui->actionDel);
    ui->btn_upd->setDefaultAction(ui->actionUpdate);

    if (!loadTasks())
    {
        // 创建默认项
        qDebug() << "将创建默认项";
        m_defaultItems << "默认项1" << "默认项2" << "默认项3";

        for (auto item : m_defaultItems) {
            QStandardItem* aItem = new QStandardItem(item);
            aItem->setFlags(Qt::ItemIsEnabled
                            | Qt::ItemIsSelectable
                            | Qt::ItemIsEditable
                            | Qt::ItemIsDragEnabled
                            | Qt::ItemNeverHasChildren
                            | Qt::ItemIsUserCheckable);
            aItem->setCheckState(Qt::Unchecked);
            m_toDoListModel->appendRow(aItem);
        }
    }

    ui->toDolistView->setModel(m_toDoListModel);
    ui->toDolistView->setDragEnabled(true);    // 启用拖拽
    ui->toDolistView->setAcceptDrops(true);    // 接受放置
    ui->toDolistView->setDragDropMode(QAbstractItemView::InternalMove); // 内部移动
    ui->toDolistView->setDefaultDropAction(Qt::MoveAction);

    connect(m_toDoListModel, &QStandardItemModel::itemChanged, [=](){
        isModified = true;
        qDebug() << "发生修改";
    });
    connect(m_toDoListModel, &QStandardItemModel::rowsInserted, [=](const QModelIndex &parent, int first, int last){
        isModified = true;
        qDebug() << "添加了行";
    });

    connect(m_toDoListModel, &QStandardItemModel::rowsRemoved, [=](const QModelIndex &parent, int first, int last){
        isModified = true;
        qDebug() << "删除了行";
    });
    connect(m_toDoListModel, &QStandardItemModel::modelReset, [=](){
        isModified = true;
        qDebug() << "清空";
    });
}

ToDoList::~ToDoList()
{
    delete ui;
}


// 创建项
QStandardItem* ToDoList::newTodoItem(QString text, bool checked)
{
    QStandardItem* aItem = new QStandardItem(text);
    aItem->setFlags(Qt::ItemIsEnabled
                    | Qt::ItemIsSelectable
                    | Qt::ItemIsEditable
                    | Qt::ItemIsDragEnabled
                    | Qt::ItemNeverHasChildren
                    | Qt::ItemIsUserCheckable);
    aItem->setCheckState(checked ? Qt::Checked : Qt::Unchecked);

    return aItem;
}


// 保存数据
void ToDoList::saveTasks()
{
    if (!isModified)
        return;

    // 获取数据
    QJsonArray data_array;

    for (int i = 0; i < m_toDoListModel->rowCount(); ++i) {
        QStandardItem *model_item = m_toDoListModel->item(i);

        QJsonObject item_data;
        item_data["text"] = model_item->text();
        item_data["checked"] = (model_item->checkState() == Qt::Checked);

        data_array.append(item_data);
    }

    QJsonDocument doc(data_array);

    // 写入
    QFile file(m_dataPath);
    if (file.open(QIODevice::WriteOnly)) {
        file.write(doc.toJson());
        file.close();
    }
    isModified = false;

    qDebug() << "todolist已保存";
}


// 加载
bool ToDoList::loadTasks()
{
    QFile file(m_dataPath);
    if (!file.exists() || !file.open(QIODevice::ReadOnly))
        return false;

    QByteArray data = file.readAll();
    file.close();

    QJsonDocument doc = QJsonDocument::fromJson(data);
    if (!doc.isArray())
        return false;

    QJsonArray data_array = doc.array();
    for (const auto &item : data_array) {
        QJsonObject obj = item.toObject();
        QString text = obj["text"].toString();
        bool checked = obj["checked"].toBool();

        auto model_item = newTodoItem(text, checked);
        m_toDoListModel->appendRow(model_item);
    }

    qDebug() << "加载todo成功";
    return true;
}


// 获取保存数据文件的位置
QString ToDoList::getTaskFilePath()
{
    QString appDir = QCoreApplication::applicationDirPath();
    QString filePath = appDir + "/tasks.json";

    // 检查应用程序目录是否可写（简单测试）
    QFileInfo dirInfo(appDir);
    if (!dirInfo.isWritable()) {
        // 不可写，回退到用户数据目录
        QString fallbackPath = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
        QDir().mkpath(fallbackPath); // 确保目录存在
        filePath = fallbackPath + "/tasks.json";
    }
    return filePath;
}


// 添加项到model
void ToDoList::on_actionAdd_triggered()
{
    // 新建并添加到模型中
    QStandardItem* aItem = newTodoItem();
    m_toDoListModel->appendRow(aItem);

    // 聚焦
    auto index = m_toDoListModel->indexFromItem(aItem);
    ui->toDolistView->setCurrentIndex(index);
    ui->toDolistView->edit(index);
}


// 从model中删除
void ToDoList::on_actionDel_triggered()
{
    auto curIndex = ui->toDolistView->currentIndex();
    if (curIndex.isValid())
        m_toDoListModel->removeRow(curIndex.row());
}


// 清空
void ToDoList::on_actionClear_triggered()
{
    if (m_toDoListModel->rowCount() == 0)
        return;

    if (QMessageBox::question(this, "ToDoList", "确认清空列表？", QMessageBox::Yes | QMessageBox::No, QMessageBox::No)
        == QMessageBox::Yes)
    {
        m_toDoListModel->clear();
    }
}


// 整理（删除勾选的项）
void ToDoList::on_actionUpdate_triggered()
{
    for (int i = m_toDoListModel->rowCount()-1; i >= 0; --i) {
        if (m_toDoListModel->item(i)->checkState() == Qt::Checked) {
            m_toDoListModel->removeRow(i);
        }
    }
}
