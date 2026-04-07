#include "tg_apple_state_playing.h"
#include "tg_apple_logic_orchestrator.h"
TgAppleStatePlaying::TgAppleStatePlaying(TgAppleLogicOrchestrator& logic) : m_logic(logic) {}
void TgAppleStatePlaying::onEnter() { m_spawnAcc = 0.0f; m_logic.spawnApple(); }
void TgAppleStatePlaying::onUpdate(float dt) {
    m_spawnAcc += dt;
    if (m_spawnAcc >= 0.8f) { m_logic.spawnApple(); m_spawnAcc = 0.0f; }
}
void TgAppleStatePlaying::onExit() {}