#include "tg_player_entity.h"

void TgPlayerEntity::update(float dt)
{
    if (!isActive()) return; // 使用 Getter
    m_pos.rx() += m_speed * m_direction * dt;
    if (m_pos.x() <= 0.0f) { m_direction = 1.0f; m_pos.setX(0.0f); }
    else if (m_pos.x() >= 960.0f) { m_direction = -1.0f; m_pos.setX(960.0f); }
}

//bool TgPlayerEntity::isOffScreen() const { return false; }
void TgPlayerEntity::takeDamage() { if (m_hp > 0) m_hp--; }
void TgPlayerEntity::heal() { m_hp = 18; }