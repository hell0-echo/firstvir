#include "tg_space_game_logic.h"
#include <QRandomGenerator>
#include <QDebug>

TgSpaceGameLogic::TgSpaceGameLogic(QObject* parent)
    : QObject(parent), m_enemyPool(20), m_meteorPool(10), m_bulletPool(20)
{
    m_player.m_pos = QPointF(480, 600);
    m_player.acquire();
} 

void TgSpaceGameLogic::step(float dt)
{
    m_player.update(dt);
    updateEntities(dt);
    checkCollisions();
    spawnEnemies();
    handleRewardWord();
    updateDifficulty();
    updateSnapshot(); 
}

bool TgSpaceGameLogic::handleInput(QChar key)
{
    if (!key.isLetter()) return false;
    QChar upperKey = key.toUpper();

    if (m_rewardWord && m_rewardWord->isActive() && m_rewardWord->matchChar(upperKey)) {
        if (m_rewardWord->isCompleted()) {
            m_player.heal();
            m_rewardWord->release();
        }
        return true;
    }

    TgEntityBase* target = nullptr;
    for (auto* enemy : m_enemies) {
        if (enemy->isActive() && enemy->isFullyOnScreen() && !enemy->isOffScreen() && enemy->m_char == upperKey) {
            target = enemy;
            break;
        }
    }
    if (!target) {
        for (auto* meteor : m_meteors) {
            if (meteor->isActive() && meteor->isFullyOnScreen() && !meteor->isOffScreen() && meteor->m_char == upperKey) {
                target = meteor;
                break;
            }
        }
    }

    if (target) {
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
            m_bullets.removeAt(i);
        }
    }

    QRect playerRect(m_player.m_pos.x() - 20, m_player.m_pos.y() - 20, 40, 40);
    for (int i = m_enemies.size() - 1; i >= 0; --i) {
        auto* e = m_enemies[i];
        if (e && e->isActive() && !e->isOffScreen()) {
            if (playerRect.intersects(QRect(e->m_pos.x() - 20, e->m_pos.y() - 20, 40, 40))) {
                m_player.takeDamage();
                e->release();
                m_enemies.removeAt(i);
            }
        }
    }
    for (int i = m_meteors.size() - 1; i >= 0; --i) {
        auto* m = m_meteors[i];
        if (m && m->isActive() && !m->isOffScreen()) {
            if (playerRect.intersects(QRect(m->m_pos.x() - 20, m->m_pos.y() - 20, 40, 40))) {
                m_player.takeDamage();
                m->release();
                m_meteors.removeAt(i);
            }
        }
    }
    if (!m_player.isAlive()) emit gameOver();
}

void TgSpaceGameLogic::updateEntities(float dt) {
    for (auto* e : m_enemies) if (e->isActive()) e->update(dt);
    for (auto* m : m_meteors) if (m->isActive()) m->update(dt);
    for (auto* b : m_bullets) if (b->isActive()) b->update(dt);
}

void TgSpaceGameLogic::spawnEnemies() {
    // TODO: 按需补充敌机/陨石生成逻辑
}

void TgSpaceGameLogic::handleRewardWord() {
    // TODO: 按需补充奖励词生成逻辑
}

void TgSpaceGameLogic::updateDifficulty() {
    m_upgradeTimer += 0.016f;
    if (m_upgradeTimer >= m_upgradeInterval) {
        m_upgradeTimer = 0;
        m_difficultyLevel++;
        m_maxEnemyCount++;
    }
}

void TgSpaceGameLogic::updateSnapshot() {
    m_snapshot.m_score = m_score;
    m_snapshot.m_hp = m_player.getHP();
    m_snapshot.m_isDefeat = !m_player.isAlive();
    m_snapshot.m_entities.clear();

    if (m_player.isActive())
        m_snapshot.m_entities.append({ QRect(m_player.m_pos.x() - 20, m_player.m_pos.y() - 20, 40, 40), m_player.m_char, true, false, 1 });
    for (auto* b : m_bullets) if (b->isActive())
        m_snapshot.m_entities.append({ QRect(b->m_pos.x() - 5, b->m_pos.y() - 5, 10, 10), b->m_char, true, false, 4 });
    for (auto* e : m_enemies) if (e->isActive())
        m_snapshot.m_entities.append({ QRect(e->m_pos.x() - 20, e->m_pos.y() - 20, 40, 40), e->m_char, true, false, 2 });
    for (auto* m : m_meteors) if (m->isActive())
        m_snapshot.m_entities.append({ QRect(m->m_pos.x() - 20, m->m_pos.y() - 20, 40, 40), m->m_char, true, false, 3 });
}

void TgSpaceGameLogic::reset() {
    m_score = 0;
    m_difficultyLevel = 1;
    m_maxEnemyCount = 3;
    m_upgradeTimer = 0.0f;
    m_rewardTimer = 0.0f;

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
    updateSnapshot();
}