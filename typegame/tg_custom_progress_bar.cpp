// tg_custom_progress_bar.cpp
#include "tg_custom_progress_bar.h"
#include <QPainter>

TgCustomProgressBar::TgCustomProgressBar(QWidget* parent)
    : QWidget(parent)
{
    setFixedHeight(24);
}

void TgCustomProgressBar::setCurrentValue(int value)
{
    m_currentValue = qBound(0, value, m_maxValue);
    update();
}

void TgCustomProgressBar::setMaxValue(int max)
{
    m_maxValue = max;
    update();
}

void TgCustomProgressBar::paintEvent(QPaintEvent* event)
{
    Q_UNUSED(event);
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    int segWidth = width() / m_segments;
    int filledSegs = m_currentValue; // 1HP = 1 segment

    for (int i = 0; i < m_segments; ++i) {
        QRect segRect(i * segWidth, 2, segWidth - 2, height() - 4);

        if (i < filledSegs) {
            // 满血段：绿色
            painter.fillRect(segRect, QColor(0, 200, 0));
        }
        else {
            // 空血段：深灰色
            painter.fillRect(segRect, QColor(50, 50, 50));
        }

        // 边框
        painter.setPen(QPen(QColor(200, 200, 200), 1));
        painter.drawRect(segRect);
    }
}