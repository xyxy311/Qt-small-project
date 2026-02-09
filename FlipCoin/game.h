#ifndef GAME_H
#define GAME_H

#include <QMainWindow>
class Coin;

class Game : public QMainWindow
{
    Q_OBJECT
public:
    // explicit Game(QWidget *parent = nullptr);
    Game(int l);

    void paintEvent(QPaintEvent *);

private:
    int level;
    std::vector<std::vector<unsigned char>> config;
    Coin* coins[4][4];
    bool success, flipping;
    void success_scene();
signals:
    void backSignal();
    void quit();
};

#endif // GAME_H
