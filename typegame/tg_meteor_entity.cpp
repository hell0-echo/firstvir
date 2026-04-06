#include "tg_meteor_entity.h"

void TgMeteorEntity::update(float dt)
{
    if (!isActive()) return; // 使用 Getter
    m_pos.ry() += m_speed * dt;
}

bool TgMeteorEntity::isOffScreen() const
{
    return m_pos.y() > 700.0f;
}