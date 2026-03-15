#include "customdelegate.h"

CustomDelegate::CustomDelegate(QObject *parent)
    : QStyledItemDelegate{parent}
{}

void CustomDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    // 1. 复制 option，避免修改原样式
    QStyleOptionViewItem opt = option;
    initStyleOption(&opt, index);

    // 2. 获取当前 item 的勾选状态
    Qt::CheckState state = static_cast<Qt::CheckState>(
        index.data(Qt::CheckStateRole).value<int>()
        );

    // 3. 如果勾选了，修改字体
    if (state == Qt::Checked) {
        QFont font = opt.font;
        font.setStrikeOut(true);
        font.setItalic(true);
        opt.font = font;
        opt.palette.setColor(QPalette::Text, Qt::gray);
    }

    // 4. 调用父类 paint —— 它会绘制文本、图标、复选框等
    QStyledItemDelegate::paint(painter, opt, index);
}
