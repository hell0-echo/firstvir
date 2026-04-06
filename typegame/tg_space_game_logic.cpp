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
    // 1. 更新实体
    m_player.update(dt);
    updateEntities(dt);

    // 2. 碰撞检测
    checkCollisions();

    // 3. 生成逻辑
    spawnEnemies();
    handleRewardWord();
    updateDifficulty();

    // 4. 同步快照到 UI
    // ... (同步 m_player, enemies, bullets, score, hp 到 m_sync)
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
// ... 其他辅助函数实现