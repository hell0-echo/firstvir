// tg_game_canvas.cpp
#include "tg_game_canvas.h"
#include <QPainter>
#include <QTimer>

TgGameCanvas::TgGameCanvas(TgThreadSync& sync, QWidget* parent)
    : QWidget(parent), m_sync(sync)
{
    setAttribute(Qt::WA_OpaquePaintEvent);
    setAttribute(Qt::WA_NoSystemBackground);
    QTimer* timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &TgGameCanvas::scheduleUpdate);
    timer->start(16);
}

void TgGameCanvas::scheduleUpdate() { update(); }

void TgGameCanvas::paintEvent(QPaintEvent* event)
{
    Q_UNUSED(event);
    if (m_backBuffer.isNull() || m_backBuffer.size() != size())
        m_backBuffer = QPixmap(size());

    QPainter painter(&m_backBuffer);
    painter.fillRect(rect(), Qt::darkGreen); // Placeholder background

    m_sync.lockForRead();
    const TgFrameSnapshot& snap = m_sync.snapshot();
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
    m_sync.unlockRead();

    QPainter screenPainter(this);
    screenPainter.drawPixmap(0, 0, m_backBuffer);
}