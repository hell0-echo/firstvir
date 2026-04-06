/* ------------------------------------------------------------------
//  文件名    :   tg_apple_logic_orchestrator.h
//  创建者    :   dev@project.cn
//  创建时间  :   2024-05-20
//  功能描述  :   拯救苹果核心调度器（状态路由、实体管理、信号发射）
// ----------------------------------------------------------------*/
#ifndef _TG_APPLE_LOGIC_ORCHESTRATOR_H_
#define _TG_APPLE_LOGIC_ORCHESTRATOR_H_

#include <QObject>
#include "tg_common_types.h"
#include "tg_thread_sync.h"
#include "tg_memory_pool.h"
#include "tg_apple_entity.h"
#include "tg_config_manager.h"
#include <QVector>

class TgStateInterface;

class TgAppleLogicOrchestrator : public QObject
{
    Q_OBJECT

public:
    explicit TgAppleLogicOrchestrator(TgThreadSync& sync, QObject* parent = nullptr);
    ~TgAppleLogicOrchestrator() override = default;

    TgAppleLogicOrchestrator(const TgAppleLogicOrchestrator&) = delete;
    TgAppleLogicOrchestrator& operator=(const TgAppleLogicOrchestrator&) = delete;

    void setState(TgStateInterface* state);
    void step(float dt);
    bool handleInput(QChar key);

    void setLevel(int level);
    void setTarget(int target);
    void setMaxFails(int maxFails);
    void setMaxOnScreen(int maxOnScreen);

    void spawnApple();
    int getScore() const { return m_score; }
    int getFails() const { return m_fails; }
    bool isVictory() const { return m_score >= m_target; }
    bool isDefeat() const { return m_fails >= m_maxFails; }
    void reset();
    void applyConfig(const TgAppleConfig& config);
    TgGameState getCurrentState() const { return m_currentStateEnum; }

signals:
    void stateChanged(TgGameState state);
    void victoryTriggered();
    void defeatTriggered();

private:
    void checkCollisions();
    void updateSnapshot();

    TgThreadSync& m_sync;
    TgStateInterface* m_currentState = nullptr;
    TgMemoryPool<TgAppleEntity> m_pool;
    QVector<TgAppleEntity*> m_activeApples;

    int m_level = kAppleDefaultLevel;
    int m_target = kAppleDefaultTarget;
    int m_maxFails = kAppleDefaultMaxFails;
    int m_maxOnScreen = kAppleDefaultMaxOnScreen;
    int m_score = 0;
    int m_fails = 0;
    float m_spawnTimer = 0.0f;
    TgGameState m_currentStateEnum = TgGameState::Idle;
};

#endif // _TG_APPLE_LOGIC_ORCHESTRATOR_H_