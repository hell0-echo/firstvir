#include "tg_bullet_entity.h"
#include <QVector2D>

void TgBulletEntity::update(float dt)
{
    if (!isActive() || m_target == nullptr || !m_target->isActive()) return;
    QVector2D dir(m_target->m_pos.x() - m_pos.x(), m_target->m_pos.y() - m_pos.y());
    float dist = dir.length();
    if (dist < 5.0f) { release(); return; }
    dir.normalize();
    m_pos.rx() += dir.x() * m_speed * dt;
    m_pos.ry() += dir.y() * m_speed * dt;
}

void TgBulletEntity::setTarget(TgEntityBase* target) { m_target = target; }
void TgBulletEntity::setStartPosition(const QPointF& start) { m_pos = start; }

//bool TgBulletEntity::isOffScreen() const 
//{
//    return m_pos.y() < -50.0f || m_pos.y() > 750.0f || m_pos.x() < -50.0f || m_pos.x() > 1010.0f;
//}