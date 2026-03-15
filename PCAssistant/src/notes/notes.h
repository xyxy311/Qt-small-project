#ifndef NOTES_H
#define NOTES_H

#include <QWidget>

class MainWindow;

namespace Ui {
class Notes;
}

class Notes : public QWidget
{
    Q_OBJECT
    friend MainWindow;

public:
    explicit Notes(QWidget *parent = nullptr);
    ~Notes();

signals:
    void edit();

private slots:
    void on_actionedit_triggered();
    void saveData();

private:
    Ui::Notes *ui;
    QString m_dataPath;

    void loadData();
};

#endif // NOTES_H
