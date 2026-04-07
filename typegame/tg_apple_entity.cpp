#include "tg_apple_entity.h"

void TgAppleEntity::update(float dt)
{
    if (!isActive()) return; // 使用 Getter
    m_pos.ry() += m_speed * dt;
}

bool TgAppleEntity::isOffScreen() const 
{
    return m_pos.y() > 700.0f;
}