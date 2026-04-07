/* ------------------------------------------------------------------
//  文件名    :   tg_thread_sync.h
//  创建者    :   dev@project.cn
//  创建时间  :   2024-05-20
//  功能描述  :   逻辑线程与UI线程帧快照同步封装
// ----------------------------------------------------------------*/
#ifndef _TG_THREAD_SYNC_H_
#define _TG_THREAD_SYNC_H_

#include <QReadWriteLock>
#include <QVector>
#include <QChar>
#include <QRect>

struct TgEntitySnapshot
{
    QRect m_rect;
    QChar m_char;
    bool m_isActive = false;
    bool m_isBroken = false;
    int m_type = 0; // 0:Apple, 1:Player, 2:Enemy, 3:Meteor, 4:Bullet
};

struct TgFrameSnapshot
{
    int m_score = 0;
    int m_fails = 0;
    bool m_isVictory = false;
    bool m_isDefeat = false;
    QVector<TgEntitySnapshot> m_entities;
    int m_hp = 18; // 新增：太空模式玩家生命值
};

class TgThreadSync
{
public:
    TgThreadSync() = default;
    ~TgThreadSync() = default;

    TgThreadSync(const TgThreadSync&) = delete;
    TgThreadSync& operator=(const TgThreadSync&) = delete;

    void lockForRead() { m_lock.lockForRead(); }
    void unlockRead() { m_lock.unlock(); }

    void lockForWrite() { m_lock.lockForWrite(); }
    void unlockWrite() { m_lock.unlock(); }

    const TgFrameSnapshot& snapshot() const { return m_snapshot; }
    TgFrameSnapshot& mutableSnapshot() { return m_snapshot; }

private:
    QReadWriteLock m_lock;
    TgFrameSnapshot m_snapshot;
};

#endif // _TG_THREAD_SYNC_H_