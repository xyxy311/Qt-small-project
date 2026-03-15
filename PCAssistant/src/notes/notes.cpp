#include "notes.h"
#include "ui_notes.h"
#include "globle.h"
#include <QFile>

Notes::Notes(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Notes)
{
    ui->setupUi(this);

    m_dataPath = AppData::dataDir + "/notes";
    qDebug() << m_dataPath;

    loadData();
}

Notes::~Notes()
{
    delete ui;
}

void Notes::on_actionedit_triggered()
{
    emit edit();
}

void Notes::saveData()
{
    if (!ui->plainTextEdit->document()->isModified())
        return;
    QFile file(m_dataPath);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        return;
    }
    QTextStream writeIn(&file);

    QString data = ui->plainTextEdit->toPlainText();

    writeIn << data;
    qDebug() << "Save Notes Suc";
}

void Notes::loadData()
{
    QFile file(m_dataPath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        return;
    }

    QString data = file.readAll();
    file.close();
    ui->plainTextEdit->setPlainText(data);

    qDebug() << "Load Notes Suc";
}

