/* ------------------------------------------------------------------
//  文件名    :   tg_entity_base.h
//  创建者    :   dev@project.cn
//  创建时间  :   2024-05-20
//  功能描述  :   实体公共基类
// ----------------------------------------------------------------*/
#ifndef _TG_ENTITY_BASE_H_
#define _TG_ENTITY_BASE_H_

#include "tg_poolable_base.h"
#include <QPointF>
#include <QChar>

class TgEntityBase : public TgPoolableBase
{
public:
    TgEntityBase() = default;
    ~TgEntityBase() override = default;

    TgEntityBase(const TgEntityBase&) = delete;
    TgEntityBase& operator=(const TgEntityBase&) = delete;

    virtual void update(float dt) = 0;
    virtual bool isOffScreen() const = 0; // 纯虚函数，所有实体必须实现

    QPointF m_pos;
    float m_speed = 0.0f;
    QChar m_char;
};

#endif // _TG_ENTITY_BASE_H_