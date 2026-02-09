#ifndef SCENE_H
#define SCENE_H
#include <QMainWindow>
class Game;

class Scene : public QMainWindow
{
    Q_OBJECT

public:
    Scene(QWidget *);
    void paintEvent(QPaintEvent *);

    // 游戏场景
    Game *gameScene;
signals:
    // 返回信号
    void backSignal();
    void quit();
};

#endif // SCENE_H
