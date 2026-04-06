/* ------------------------------------------------------------------
//  文件名    :   tg_player_entity.h
//  创建者    :   dev@project.cn
//  创建时间  :   2024-05-20
//  功能描述  :   玩家飞船实体：自动水平移动、边界反弹、生命值
// ----------------------------------------------------------------*/
#ifndef _TG_PLAYER_ENTITY_H_
#define _TG_PLAYER_ENTITY_H_

#include "tg_entity_base.h"

class TgPlayerEntity : public TgEntityBase
{
public:
    TgPlayerEntity() = default;
    ~TgPlayerEntity() override = default;

    TgPlayerEntity(const TgPlayerEntity&) = delete;
    TgPlayerEntity& operator=(const TgPlayerEntity&) = delete;

    void update(float dt) override;
    void takeDamage();
    void heal();
    int getHP() const { return m_hp; }
    bool isAlive() const { return m_hp > 0; }

private:
    int m_hp = 18;
    float m_direction = 1.0f; // 1.0 右，-1.0 左
    float m_speed = 150.0f;   // 水平移动速度
};

#endif // _TG_PLAYER_ENTITY_H_

// tg_player_entity.cpp
#include "tg_player_entity.h"
#include <algorithm> // std::clamp

void TgPlayerEntity::update(float dt)
{
    if (!m_isActive) return;

    // 自动水平移动
    m_pos.rx() += m_speed * m_direction * dt;

    // 边界检测与反弹 (使用 std::clamp 限制位置，防止穿墙)
    // 注意：m_pos 类型为 QPointF，rx() 返回 qreal
    qreal currentX = m_pos.x();
    qreal minX = 0.0;
    qreal maxX = 960.0; // 假设游戏宽度 960

    if (currentX <= minX) {
        m_direction = 1.0f;
        m_pos.setX(minX);
    }
    else if (currentX >= maxX) {
        m_direction = -1.0f;
        m_pos.setX(maxX);
    }
}

void TgPlayerEntity::takeDamage()
{
    if (m_hp > 0) m_hp--;
}

void TgPlayerEntity::heal()
{
    m_hp = 18; // 满血恢复
}