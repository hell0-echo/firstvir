#ifndef _TG_ENEMY_ENTITY_H_
#define _TG_ENEMY_ENTITY_H_
#include "tg_entity_base.h"
class TgEnemyEntity : public TgEntityBase
{
public:
    TgEnemyEntity() = default;
    ~TgEnemyEntity() override = default;
    TgEnemyEntity(const TgEnemyEntity&) = delete;
    TgEnemyEntity& operator=(const TgEnemyEntity&) = delete;
    void update(float dt) override;
    bool isOffScreen() const override;
    bool isFullyOnScreen() const { return m_pos.y() >= 0.0f; }
private:
    float m_swayAmp = 50.0f;
    float m_swayFreq = 2.0f;
    float m_swayOffset = 0.0f;
};
#endif