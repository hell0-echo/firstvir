#include "tg_enemy_entity.h"
#include <qmath.h>

void TgEnemyEntity::update(float dt)
{
    if (!isActive()) return; // 使用 Getter
    m_pos.ry() += m_speed * dt;
    m_swayOffset += m_swayFreq * dt;
    m_pos.rx() += qSin(m_swayOffset) * m_swayAmp * dt;
}

bool TgEnemyEntity::isOffScreen() const 
{
    return m_pos.y() > 700.0f;
}