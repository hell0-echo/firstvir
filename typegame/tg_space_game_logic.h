#ifndef _TG_SPACE_GAME_LOGIC_H_
#define _TG_SPACE_GAME_LOGIC_H_

#include <QObject>
#include "tg_common_types.h"
#include "tg_memory_pool.h"
#include "tg_player_entity.h"
#include "tg_enemy_entity.h"
#include "tg_meteor_entity.h"
#include "tg_bullet_entity.h"
#include "tg_reward_word_entity.h"
#include <QVector>

class TgSpaceGameLogic : public QObject
{
    Q_OBJECT
public:
    explicit TgSpaceGameLogic(QObject* parent = nullptr);
    ~TgSpaceGameLogic() override = default;

    void step(float dt);
    bool handleInput(QChar key);
    void reset();
    int getScore() const { return m_score; }

    const TgFrameSnapshot& getSnapshot() const { return m_snapshot; }

signals:
    void gameOver();

private:
    void spawnEnemies();
    void updateEntities(float dt);
    void checkCollisions();
    void handleRewardWord();
    void updateDifficulty();
    void updateSnapshot(); 

    // 实体管理
    TgPlayerEntity m_player;
    QVector<TgEnemyEntity*> m_enemies;
    QVector<TgMeteorEntity*> m_meteors;
    QVector<TgBulletEntity*> m_bullets;
    TgRewardWordEntity* m_rewardWord = nullptr;

    // 内存池
    TgMemoryPool<TgEnemyEntity> m_enemyPool;
    TgMemoryPool<TgMeteorEntity> m_meteorPool;
    TgMemoryPool<TgBulletEntity> m_bulletPool;

    // 游戏状态
    int m_score = 0;
    int m_difficultyLevel = 1;
    int m_maxEnemyCount = 3;
    float m_upgradeTimer = 0.0f;
    int m_upgradeInterval = 120;
    bool m_rewardModeEnabled = false;
    float m_rewardTimer = 0.0f;

    TgFrameSnapshot m_snapshot; 
};
#endif // _TG_SPACE_GAME_LOGIC_H_