#include "tg_game_canvas.h"
#include "tg_apple_logic_orchestrator.h" 
#include <QPainter>

TgGameCanvas::TgGameCanvas(TgAppleLogicOrchestrator* logic, QWidget* parent)
    : QWidget(parent), m_logic(logic)
{
    setAttribute(Qt::WA_OpaquePaintEvent);
    setAttribute(Qt::WA_NoSystemBackground);
}

void TgGameCanvas::paintEvent(QPaintEvent* event)
{
    Q_UNUSED(event);
    if (m_backBuffer.isNull() || m_backBuffer.size() != size())
        m_backBuffer = QPixmap(size());

    QPainter painter(&m_backBuffer);
    painter.fillRect(rect(), Qt::darkGreen);

    if (!m_logic) return;

    const TgFrameSnapshot& snap = m_logic->getSnapshot();

    painter.setPen(Qt::white);
    painter.setFont(QFont("Consolas", 16, QFont::Bold));
    painter.drawText(10, 30, tr("Score: %1").arg(snap.m_score));
    painter.drawText(10, 55, tr("Fails: %1").arg(snap.m_fails));

    for (const auto& ent : snap.m_entities)
    {
        if (!ent.m_isActive) continue;
        if (ent.m_isBroken)
            painter.fillRect(ent.m_rect, Qt::red);
        else
            painter.fillRect(ent.m_rect, Qt::green);
        painter.drawText(ent.m_rect, Qt::AlignCenter, QString(ent.m_char));
    }

    QPainter screenPainter(this);
    screenPainter.drawPixmap(0, 0, m_backBuffer);
}