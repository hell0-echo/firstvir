#ifndef _TG_BULLET_ENTITY_H_
#define _TG_BULLET_ENTITY_H_
#include "tg_entity_base.h"
#include <QPointF>
class TgBulletEntity : public TgEntityBase
{
public:
    TgBulletEntity() = default;
    ~TgBulletEntity() override = default;
    TgBulletEntity(const TgBulletEntity&) = delete;
    TgBulletEntity& operator=(const TgBulletEntity&) = delete;

    void update(float dt) override;
    bool isOffScreen() const override { return m_pos.y() < -50.0f || m_pos.y() > 750.0f || m_pos.x() < -50.0f || m_pos.x() > 1010.0f; }
    void setTarget(TgEntityBase* target);
    TgEntityBase* getTarget() const { return m_target; }
    void setStartPosition(const QPointF& start);
private:
    TgEntityBase* m_target = nullptr;
    float m_speed = 400.0f;
};
#endif