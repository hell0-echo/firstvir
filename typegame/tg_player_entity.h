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
    bool isOffScreen() const override { return false; } // 玩家不离开屏幕
    void takeDamage();
    void heal();
    int getHP() const { return m_hp; }
    void setHP(int hp) { m_hp = hp; }
    bool isAlive() const { return m_hp > 0; }
private:
    int m_hp = 18;
    float m_direction = 1.0f;
    float m_speed = 150.0f;
};
#endif