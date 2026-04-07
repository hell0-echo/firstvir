#ifndef _TG_METEOR_ENTITY_H_
#define _TG_METEOR_ENTITY_H_
#include "tg_entity_base.h"
class TgMeteorEntity : public TgEntityBase
{
public:
    TgMeteorEntity() = default;
    ~TgMeteorEntity() override = default;
    TgMeteorEntity(const TgMeteorEntity&) = delete;
    TgMeteorEntity& operator=(const TgMeteorEntity&) = delete;
    void update(float dt) override;
    bool isOffScreen() const override;
    bool isFullyOnScreen() const { return m_pos.y() >= 0.0f; }
};
#endif