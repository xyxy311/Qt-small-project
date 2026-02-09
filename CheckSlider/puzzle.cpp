#include "puzzle.h"
#include <QPainter>
#include <QPainterPath>
#include <QRandomGenerator>

#define RAND(min, max) QRandomGenerator::global()->bounded(min, max)

Puzzle::Puzzle(QWidget *parent)
    : QWidget{parent}
{}

void Puzzle::setPixmap(const QPixmap& pixmap) {
    this->m_pixmap = pixmap;
    m_pos.rx() = RAND(60, this->width() - 5 - m_size.width());
    m_pos.ry() = RAND(0, this->height() - m_size.height());
    update();
}
QPixmap Puzzle::pixmap() {
    return this->m_pixmap;
}

void Puzzle::setValue(qreal value) {
    this->value = value * (this->width() - m_size.width() - 8) + 8;
    update();  // 更新绘图事件
}

void Puzzle::verify() {
    if (qAbs(m_pos.x() - value) < 3) {
        emit verifyFinished(true);
    }
    else {
        emit verifyFinished(false);
    }
}

void Puzzle::paintEvent(QPaintEvent* ev) {
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    // 绘制背景
    QPixmap background = m_pixmap.scaled(this->size(), Qt::KeepAspectRatioByExpanding);
    painter.drawPixmap(0, 0, background);

    // 裁剪路径
    QPainterPath cutPath;
    QRect rect(m_pos, m_size);
    cutPath.setFillRule(Qt::FillRule::WindingFill);
    cutPath.addRoundedRect(rect, 3, 3);
    cutPath.addEllipse(QPoint(rect.center().x(), rect.y()), 6, 6);  // 上方凸起
    QPainterPath rightPath;
    rightPath.addEllipse(QPoint(rect.right(), rect.center().y()), 6, 6); // 右边凹陷

    cutPath -= rightPath;

    // 绘制裁剪区域
    painter.setPen(Qt::NoPen);
    painter.setBrush(QColor(100, 100, 100, 120));
    painter.drawPath(cutPath);

    // 绘制拼图
    QPixmap puzzlePixmap(this->size());
    puzzlePixmap.fill(Qt::transparent);
    QPainter puzzlePainter(&puzzlePixmap);
    puzzlePainter.setRenderHint(QPainter::Antialiasing);
    puzzlePainter.setClipPath(cutPath);  // 只绘制裁剪区
    puzzlePainter.drawPath(cutPath);
    puzzlePainter.drawPixmap(0, 0, background);

    painter.drawPixmap(-m_pos.x() + value, 0, puzzlePixmap);
}
