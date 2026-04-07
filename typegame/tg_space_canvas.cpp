#include "tg_space_canvas.h"
#include "tg_space_game_logic.h" // 必须包含，用于调用 getSnapshot()
#include <QPainter>

TgSpaceCanvas::TgSpaceCanvas(TgSpaceGameLogic* logic, QWidget* parent)
    : QWidget(parent), m_logic(logic)
{
    setAttribute(Qt::WA_OpaquePaintEvent);
    setAttribute(Qt::WA_NoSystemBackground);
}

void TgSpaceCanvas::paintEvent(QPaintEvent* event)
{
    Q_UNUSED(event);
    if (m_backBuffer.isNull() || m_backBuffer.size() != size())
        m_backBuffer = QPixmap(size());

    QPainter painter(&m_backBuffer);
    painter.fillRect(rect(), QColor(10, 15, 30)); // 深空背景

    if (!m_logic) return;

    const TgFrameSnapshot& snap = m_logic->getSnapshot();

    painter.setPen(Qt::white);
    painter.setFont(QFont("Consolas", 14, QFont::Bold));

    // 绘制HUD
    painter.drawText(10, 30, tr("Score: %1").arg(snap.m_score));
    painter.drawText(10, 55, tr("HP: %1/18").arg(snap.m_hp));

    // 绘制实体
    for (const auto& ent : snap.m_entities) {
        if (!ent.m_isActive) continue;
        switch (ent.m_type) {
        case 1: // Player
            painter.fillRect(ent.m_rect, QColor(0, 150, 255));
            break;
        case 2: // Enemy
            painter.fillRect(ent.m_rect, QColor(255, 80, 80));
            painter.drawText(ent.m_rect, Qt::AlignCenter, QString(ent.m_char));
            break;
        case 3: // Meteor
            painter.fillRect(ent.m_rect, QColor(150, 150, 150));
            painter.drawText(ent.m_rect, Qt::AlignCenter, QString(ent.m_char));
            break;
        case 4: // Bullet
            painter.fillRect(ent.m_rect, QColor(0, 255, 150));
            break;
        }
    }

    QPainter screenPainter(this);
    screenPainter.drawPixmap(0, 0, m_backBuffer);
}