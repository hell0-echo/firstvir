#include "tg_space_canvas.h"
#include <QPainter>
#include <QTimer>

TgSpaceCanvas::TgSpaceCanvas(TgThreadSync& sync, QWidget* parent)
    : QWidget(parent), m_sync(sync)
{
    setAttribute(Qt::WA_OpaquePaintEvent);
    setAttribute(Qt::WA_NoSystemBackground);
    QTimer* timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &TgSpaceCanvas::scheduleUpdate);
    timer->start(16);
}

void TgSpaceCanvas::scheduleUpdate() { update(); }

void TgSpaceCanvas::paintEvent(QPaintEvent* event)
{
    Q_UNUSED(event);
    if (m_backBuffer.isNull() || m_backBuffer.size() != size())
        m_backBuffer = QPixmap(size());

    QPainter painter(&m_backBuffer);
    painter.fillRect(rect(), QColor(10, 15, 30)); // 深空背景
    painter.setPen(Qt::white);
    painter.setFont(QFont("Consolas", 14, QFont::Bold));

    m_sync.lockForRead();
    const TgFrameSnapshot& snap = m_sync.snapshot();

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
    m_sync.unlockRead();

    QPainter screenPainter(this);
    screenPainter.drawPixmap(0, 0, m_backBuffer);
}