#ifndef _TG_APPLE_LOGIC_ORCHESTRATOR_H_
#define _TG_APPLE_LOGIC_ORCHESTRATOR_H_

#include <QObject>
#include "tg_common_types.h"
#include "tg_memory_pool.h"
#include "tg_apple_entity.h"
#include "tg_config_manager.h"
#include <QVector>

class TgStateInterface;

class TgAppleLogicOrchestrator : public QObject
{
    Q_OBJECT
public:
    explicit TgAppleLogicOrchestrator(QObject* parent = nullptr);
    ~TgAppleLogicOrchestrator() override = default;

    void setState(TgStateInterface* state);
    void step(float dt);
    bool handleInput(QChar key);
    void reset();
    void applyConfig(const TgAppleConfig& config);
    void setLevel(int level);
    void setTarget(int target);
    void setMaxFails(int maxFails);
    void setMaxOnScreen(int maxOnScreen);
    void spawnApple();

    int getScore() const { return m_score; }
    int getFails() const { return m_fails; }
    bool isVictory() const { return m_score >= m_target; }
    bool isDefeat() const { return m_fails >= m_maxFails; }
    TgGameState getCurrentState() const { return m_currentStateEnum; }

    // ✅ 新增无锁快照读取
    const TgFrameSnapshot& getSnapshot() const { return m_snapshot; }

signals:
    void stateChanged(TgGameState state);
    void victoryTriggered();
    void defeatTriggered();

private:
    void checkCollisions();
    void updateSnapshot();

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

    TgFrameSnapshot m_snapshot; 
};
#endif // _TG_APPLE_LOGIC_ORCHESTRATOR_H_