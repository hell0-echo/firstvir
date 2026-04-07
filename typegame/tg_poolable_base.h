/* ------------------------------------------------------------------
//  文件名    :   tg_poolable_base.h
//  创建者    :   dev@project.cn
//  创建时间  :   2024-05-20
//  功能描述  :   内存池对象基类接口
// ----------------------------------------------------------------*/
#ifndef _TG_POOLABLE_BASE_H_
#define _TG_POOLABLE_BASE_H_

class TgPoolableBase
{
public:
    TgPoolableBase() = default;
    virtual ~TgPoolableBase() = default;

    TgPoolableBase(const TgPoolableBase&) = delete;
    TgPoolableBase& operator=(const TgPoolableBase&) = delete;

    virtual void acquire() { m_isActive = true; }
    virtual void release() { m_isActive = false; }
    bool isActive() const { return m_isActive; } // 公开只读访问器，替代直接访问 protected 成员

protected:
    bool m_isActive = false;
};

#endif // _TG_POOLABLE_BASE_H_