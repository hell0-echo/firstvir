#ifndef _TG_APPLE_ENTITY_H_
#define _TG_APPLE_ENTITY_H_
#include "tg_entity_base.h"
class TgAppleEntity : public TgEntityBase
{
public:
    TgAppleEntity() = default;
    ~TgAppleEntity() override = default;
    TgAppleEntity(const TgAppleEntity&) = delete;
    TgAppleEntity& operator=(const TgAppleEntity&) = delete;
    void update(float dt) override;
    bool isOffScreen() const override;
    bool m_isBroken = false;
};
#endif