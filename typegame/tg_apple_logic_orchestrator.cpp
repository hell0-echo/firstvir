#include "tg_apple_logic_orchestrator.h"
#include "tg_state_interface.h"
#include <QSet>
#include <QRandomGenerator>

TgAppleLogicOrchestrator::TgAppleLogicOrchestrator(QObject* parent)
    : QObject(parent), m_pool(32)
{
}

void TgAppleLogicOrchestrator::setState(TgStateInterface* state)
{
    if (m_currentState) m_currentState->onExit();
    m_currentState = state;
    if (m_currentState) m_currentState->onEnter();
}

void TgAppleLogicOrchestrator::step(float dt)
{
    if (!m_currentState) return;
    m_currentState->onUpdate(dt);
    for (TgAppleEntity* apple : m_activeApples) {
        apple->update(dt);
    }
    checkCollisions();
    updateSnapshot(); 
    if (isVictory()) emit victoryTriggered();
    if (isDefeat()) emit defeatTriggered();
}

bool TgAppleLogicOrchestrator::handleInput(QChar key)
{
    if (!m_currentState) return false;
    for (int i = m_activeApples.size() - 1; i >= 0; --i) {
        TgAppleEntity* apple = m_activeApples[i];
        if (apple->isActive() && apple->m_char == key) {
            apple->release();
            m_activeApples.removeAt(i);
            m_pool.deallocate(apple);
            m_score++;
            return true;
        }
    }
    return false;
}

void TgAppleLogicOrchestrator::spawnApple()
{
    if (m_activeApples.size() >= m_maxOnScreen) return;
    QSet<QChar> existingChars;
    for (const auto* a : m_activeApples) existingChars.insert(a->m_char);
    QChar newChar;
    do {
        newChar = QChar('A' + QRandomGenerator::global()->bounded(26));
    } while (existingChars.contains(newChar));

    TgAppleEntity* apple = m_pool.allocate();
    apple->m_char = newChar;
    apple->m_pos.setX(QRandomGenerator::global()->bounded(50, 950));
    apple->m_pos.setY(-50);
    apple->m_speed = kAppleFallSpeedBase * (m_level + 1);
    m_activeApples.append(apple);
}

void TgAppleLogicOrchestrator::checkCollisions()
{
    float groundY = 700.0f * 0.7f;
    for (int i = m_activeApples.size() - 1; i >= 0; --i) {
        TgAppleEntity* apple = m_activeApples[i];
        if (apple->m_pos.y() >= groundY && !apple->m_isBroken) {
            apple->m_isBroken = true;
            m_fails++;
            m_activeApples.removeAt(i);
            m_pool.deallocate(apple);
        }
    }
}

// ✅ 无锁快照更新
void TgAppleLogicOrchestrator::updateSnapshot()
{
    m_snapshot.m_score = m_score;
    m_snapshot.m_fails = m_fails;
    m_snapshot.m_isVictory = isVictory();
    m_snapshot.m_isDefeat = isDefeat();
    m_snapshot.m_entities.resize(m_activeApples.size());
    for (int i = 0; i < m_activeApples.size(); ++i) {
        const auto* a = m_activeApples[i];
        m_snapshot.m_entities[i] = {
            QRect(static_cast<int>(a->m_pos.x()), static_cast<int>(a->m_pos.y()), 40, 40),
            a->m_char, true, a->m_isBroken, 0
        };
    }
}

void TgAppleLogicOrchestrator::reset()
{
    m_score = 0;
    m_fails = 0;
    for (TgAppleEntity* apple : m_activeApples) {
        m_pool.deallocate(apple);
    }
    m_activeApples.clear();
    m_spawnTimer = 0.0f;
    updateSnapshot();
}

void TgAppleLogicOrchestrator::applyConfig(const TgAppleConfig& config)
{
    setLevel(config.level);
    setTarget(config.target);
    setMaxFails(config.maxFails);
    setMaxOnScreen(config.maxOnScreen);
}

void TgAppleLogicOrchestrator::setLevel(int level) { m_level = qBound(0, level, 9); }
void TgAppleLogicOrchestrator::setTarget(int target) { m_target = qBound(100, target, 900); }
void TgAppleLogicOrchestrator::setMaxFails(int maxFails) { m_maxFails = qBound(5, maxFails, 50); }
void TgAppleLogicOrchestrator::setMaxOnScreen(int maxOnScreen) { m_maxOnScreen = qBound(1, maxOnScreen, 5); }