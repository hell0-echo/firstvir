// tg_space_game_logic.cpp
#include "tg_space_game_logic.h"
#include <QRandomGenerator>
#include <QDebug>


void TgSpaceGameLogic::updateEntities(float dt) {}
void TgSpaceGameLogic::handleRewardWord() {}

TgSpaceGameLogic::TgSpaceGameLogic(TgThreadSync& sync, QObject* parent)
    : QObject(parent), m_sync(sync), m_enemyPool(20), m_meteorPool(10), m_bulletPool(20)
{
    //m_player.m_isActive = true;
    m_player.m_pos = QPointF(480, 600); // 玩家初始位置
}


void TgSpaceGameLogic::step(float dt)
{
    m_player.update(dt);
    updateEntities(dt);
    checkCollisions();
    spawnEnemies();
    handleRewardWord();
    updateDifficulty();

    // 同步快照到 UI
    m_sync.lockForWrite();
    TgFrameSnapshot& snap = m_sync.mutableSnapshot();
    snap.m_score = m_score;
    snap.m_hp = m_player.getHP();
    snap.m_isDefeat = !m_player.isAlive();
    snap.m_entities.clear();

    // 玩家
    if (m_player.isActive()) {
        snap.m_entities.append({ QRect(m_player.m_pos.x() - 20, m_player.m_pos.y() - 20, 40, 40), m_player.m_char, true, false, 1 });
    }
    // 子弹
    for (auto* b : m_bullets) if (b->isActive())
        snap.m_entities.append({ QRect(b->m_pos.x() - 5, b->m_pos.y() - 5, 10, 10), b->m_char, true, false, 4 });
    // 敌机
    for (auto* e : m_enemies) if (e->isActive())
        snap.m_entities.append({ QRect(e->m_pos.x() - 20, e->m_pos.y() - 20, 40, 40), e->m_char, true, false, 2 });
    // 陨石
    for (auto* m : m_meteors) if (m->isActive())
        snap.m_entities.append({ QRect(m->m_pos.x() - 20, m->m_pos.y() - 20, 40, 40), m->m_char, true, false, 3 });

    m_sync.unlockWrite();
}

bool TgSpaceGameLogic::handleInput(QChar key)
{
    if (!key.isLetter()) return false;
    QChar upperKey = key.toUpper();

    // 1. 检查奖励单词
    if (m_rewardWord && m_rewardWord->isActive() && m_rewardWord->matchChar(upperKey)) {
        if (m_rewardWord->isCompleted()) {
            m_player.heal();
            m_rewardWord->release(); // 单词消失
            return true;
        }
        return true;
    }

    // 2. 检查敌机/陨石匹配 (发射子弹)
    // 逻辑：寻找屏幕上存在的、完全进入屏幕的、且字母匹配的敌人
    TgEntityBase* target = nullptr;

    // 优先匹配敌机
    for (auto* enemy : m_enemies) {
        if (enemy->isActive() && enemy->isFullyOnScreen() && !enemy->isOffScreen() && enemy->m_char == upperKey) {
            target = enemy;
            break;
        }
    }
    // 若无敌机，匹配陨石
    if (!target) {
        for (auto* meteor : m_meteors) {
            if (meteor->isActive() && meteor->isFullyOnScreen() && !meteor->isOffScreen() && meteor->m_char == upperKey) {
                target = meteor;
                break;
            }
        }
    }

    if (target) {
        // 发射追踪子弹
        TgBulletEntity* bullet = m_bulletPool.allocate();
        bullet->setStartPosition(m_player.m_pos);
        bullet->setTarget(target);
        m_bullets.append(bullet);
        return true;
    }

    return false;
}

void TgSpaceGameLogic::checkCollisions()
{
    // 1. 子弹击中检测
    for (int i = m_bullets.size() - 1; i >= 0; --i) {
        TgBulletEntity* b = m_bullets[i];
        if (!b->isActive()) continue;

        TgEntityBase* target = b->getTarget();
        if (target != nullptr && target->isActive()) {
            QPointF diff = target->m_pos - b->m_pos;
            if (diff.manhattanLength() < 10.0f) {
                b->release();
                target->release();
                m_score += 1500;
            }
        }
        if (!b->isActive()) {
            //m_pool.deallocate(b); // 假设逻辑层持有池引用，或改为从 vector 移除
            m_bullets.removeAt(i);
        }
    }

    // 2. 玩家撞击检测 (分别处理敌机与陨石，避免类型转换错误)
    QRect playerRect(m_player.m_pos.x() - 20, m_player.m_pos.y() - 20, 40, 40);

    auto handleCrash = [&](TgEntityBase* e, int& idx, auto& list) {
        if (e && e->isActive() && !e->isOffScreen()) {
            QRect enemyRect(e->m_pos.x() - 20, e->m_pos.y() - 20, 40, 40);
            if (playerRect.intersects(enemyRect)) {
                m_player.takeDamage();
                e->release();
                list.removeAt(idx);
            }
        }
        };

    for (int i = m_enemies.size() - 1; i >= 0; --i) handleCrash(m_enemies[i], i, m_enemies);
    for (int i = m_meteors.size() - 1; i >= 0; --i) handleCrash(m_meteors[i], i, m_meteors);

    // 3. 游戏结束判定
    if (!m_player.isAlive()) emit gameOver();
}

void TgSpaceGameLogic::spawnEnemies()
{
    // 生成规则：<6 仅敌机，>=6 敌机 + 陨石
    // 实现逻辑：控制 m_enemies 和 m_meteors 的数量不超过设定值
    // 随机生成新敌人...
}

void TgSpaceGameLogic::updateDifficulty()
{
    m_upgradeTimer += 0.016f; // 假设 60fps
    if (m_upgradeTimer >= m_upgradeInterval) {
        m_upgradeTimer = 0;
        m_difficultyLevel++;
        m_maxEnemyCount++;
        // 重置计时器或保持循环
    }
}

// 实现头文件中声明但未定义的函数
void TgSpaceGameLogic::reset()
{
    m_score = 0;
    m_difficultyLevel = 1;
    m_maxEnemyCount = 3;
    m_upgradeTimer = 0.0f;
    m_rewardTimer = 0.0f;

    // 清理所有实体
    for (auto* e : m_enemies) if (e) m_enemyPool.deallocate(e);
    for (auto* m : m_meteors) if (m) m_meteorPool.deallocate(m);
    for (auto* b : m_bullets) if (b) m_bulletPool.deallocate(b);
    if (m_rewardWord) m_rewardWord->release();

    m_enemies.clear();
    m_meteors.clear();
    m_bullets.clear();
    m_rewardWord = nullptr;

    m_player.m_pos = QPointF(480, 600);
    m_player.setHP(18);
    m_player.acquire();
}

void TgSpaceGameLogic::setDifficultyLevel(int level) {
    m_difficultyLevel = qBound(1, level, 10);
}
void TgSpaceGameLogic::setEnemyCount(int count) {
    m_maxEnemyCount = qBound(1, count, 10);
}
void TgSpaceGameLogic::setUpgradeInterval(int seconds) {
    m_upgradeInterval = qBound(30, seconds, 300);
}
void TgSpaceGameLogic::setRewardModeEnabled(bool enabled) {
    m_rewardModeEnabled = enabled;
}

// ... 其他辅助函数实现