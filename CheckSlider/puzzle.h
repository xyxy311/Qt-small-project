#ifndef PUZZLE_H
#define PUZZLE_H

#include <QWidget>

class Puzzle : public QWidget
{
    Q_OBJECT
public:
    explicit Puzzle(QWidget *parent = nullptr);
    void setPixmap(const QPixmap& m_pixmap);
    QPixmap pixmap();
    void setValue(qreal value);
    void verify();

protected:
    void paintEvent(QPaintEvent* ev) override;

private:
    QPixmap m_pixmap;
    QPoint m_pos{200, 100};  // 裁剪位置
    QSize m_size{50, 50};   // 裁剪大小
    int value{8};  // 指示拼图的位置

signals:
    void verifyFinished(bool success);
};

#endif // PUZZLE_H
